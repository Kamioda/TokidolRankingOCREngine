#include "ImageInfo.hpp"
#include "HttpException.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <functional>
#include <type_traits>
#include <mutex>
std::mutex mutex;

template<typename F, std::enable_if_t<std::is_invocable_v<F, std::string>, std::nullptr_t> = nullptr>
inline std::string SavePicture(const std::string& u8OutputTarget, const std::string& u8ScreenShotName, const std::string& SaveName, /*引数にstd::stringが来る関数がOnSave*/ F&& OnSave) {
	auto output = std::filesystem::u8path(u8OutputTarget),
		makefilename = std::filesystem::u8path(u8ScreenShotName);
	const std::string FileName = SaveName + makefilename.extension().string(); // imwriteの制約でUnicode使用不可
	const std::string FullPath = u8OutputTarget + "/" + FileName;
	if (!std::filesystem::exists(FullPath)) throw HttpException(404, FileName + " : Not found.");
	auto currentPath = std::filesystem::current_path();
	std::lock_guard<std::mutex> lock(mutex);
	std::filesystem::current_path(output);
	OnSave(FileName);
	std::filesystem::current_path(currentPath);
	return FullPath;
}

inline std::string TrimEngine(const std::string& ScreenShot, const std::string& SaveRoot, const std::string& SaveName,  const RECT& TrimInfo, const bool ResizeFlag) {
	const ImageInfo image(ScreenShot);
	ImageInfo TrimData = image.InvertColor()
		.trim(TrimInfo)
		.Binarization()
		.ToGrayScale();
	if (ResizeFlag) TrimData = TrimData.Resize(2.0);
	return SavePicture<std::function<void(std::string)>>(SaveRoot, ScreenShot, SaveName, [&TrimData](const std::string& SaveFullPath) { TrimData.WriteOut(SaveFullPath); });
}

std::string Trim(const std::string& json) {
	try {
		nlohmann::json req = nlohmann::json::parse(json);
		nlohmann::json res{};
		const std::string OutputFileFullPath = TrimEngine(
			req["image"].get<std::string>(),
			req["root"].get<std::string>(),
			req["savename"].get<std::string>(),
			{
				req["left-top-x"].get<int>(),
				req["left-top-y"].get<int>(),
				req["right-bottom-x"].get<int>(),
				req["right-bottom-y"].get<int>()
			},
			true
		);
		res["output"] = OutputFileFullPath;
		return res.dump();
	}
	catch (const std::exception& er) {
		throw HttpException(400, er.what());
	}
}

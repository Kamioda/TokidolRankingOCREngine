#include "OCREngine.hpp"
#include "../Trimming/HttpException.hpp"
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

int ToLevelID(const std::string& source) {
	const std::array<std::string, 4> LevelString = { "EA", "NO", "HA", "EX" };
	auto itr = std::find_if(LevelString.begin(), LevelString.end(), [&source](const std::string& s) { return source.find(s) != std::string::npos; });
	return static_cast<int>(itr == LevelString.end() ? 0 : (std::distance(LevelString.begin(), itr) + 1));
}

int OutputScore(const std::string& source) {
	try {
		return std::stoi(source);
	}
	catch (const std::invalid_argument&) {
		return -1;
	}
}

inline std::string FileExistCheck(const std::string& Path) {
	if (!std::filesystem::exists(Path)) throw HttpException(404, Path + " : Not found");
	return Path;
}

std::string OCR(const std::string& json) {
	nlohmann::json req{}, res{}, config{};
	try {
		req = nlohmann::json::parse(json);
	}
	catch (const std::exception& er) {
		throw HttpException(400, er.what());
	}
	try {
		config = nlohmann::json::parse(std::ifstream("./config.json"));
	}
	catch (const std::exception& er) {
		throw HttpException(500, er.what());
	}
	OCREngine Engine(config["langfile"].get<std::string>(), config["lang"].get<std::string>());
	auto ScoreImage = cv::imread(FileExistCheck(req["score"].get<std::string>()));
	auto LevelImage = cv::imread(FileExistCheck(req["level"].get<std::string>()));
	const auto ScoreInfo = Engine.Load(ScoreImage);
	const auto LevelInfo = Engine.Load(LevelImage);
	res["score"] = OutputScore(ScoreInfo.text);
	res["level"] = ToLevelID(ScoreInfo.text);
	return res.dump();
}

#include "ImageInfo.hpp"
#include "HttpException.hpp"
#include <filesystem>

ImageInfo::ImageInfo(const std::string& FilePath) {
	try {
		if (!std::filesystem::exists(FilePath)) throw HttpException(404);
		this->image = cv::imread(FilePath, 1);
		if (this->image.empty()) throw HttpException();
	}
	catch (const HttpException& hex) {
		throw hex;
	}
	catch (...) {
		throw HttpException(500);
	}
}

ImageInfo::ImageInfo(const cv::Mat& imagedata, const RECT& rc) {
	try {
		if (rc.left < 0 || rc.left > imagedata.cols) throw std::out_of_range("left is out of image.\n" + std::to_string(rc.left) + "-" + std::to_string(imagedata.rows));
		if (rc.right < 0 || rc.right > imagedata.cols) throw std::out_of_range("right is out of image.\n" + std::to_string(rc.right) + "-" + std::to_string(imagedata.rows));
		if (rc.top < 0 || rc.top > imagedata.rows) throw std::out_of_range("top is out of range.\n" + std::to_string(rc.top) + "-" + std::to_string(imagedata.cols));
		if (rc.bottom < 0 || rc.bottom > imagedata.rows) throw std::out_of_range("bottom is out of range.\n" + std::to_string(rc.bottom) + "-" + std::to_string(imagedata.cols));
		if (rc.right < rc.left) throw std::runtime_error("left is over right");
		if (rc.bottom < rc.top) throw std::runtime_error("bottom is over top");
		cv::Rect roi(cv::Point(rc.left, rc.top), cv::Size(rc.right - rc.left, rc.bottom - rc.top));
		this->image = imagedata(roi);
	}
	catch (const std::out_of_range& o) {
		throw HttpException(400, o.what());
	}
	catch (const std::runtime_error& r) {
		throw HttpException(400, r.what());
	}
}

ImageInfo::ImageInfo(cv::Mat&& imagedata) : image(std::move(imagedata)) {}

ImageInfo ImageInfo::operator ~ () { return ImageInfo(~this->image); }

ImageInfo ImageInfo::trim(const RECT rc) const {
	return ImageInfo(this->image, rc);
}

ImageInfo ImageInfo::ToGrayScale() const {
	cv::Mat dest;
	cv::cvtColor(this->image, dest, cv::COLOR_RGB2GRAY);
	return ImageInfo(std::move(dest));
}

ImageInfo ImageInfo::Binarization() const {
	const double threshold = 100.0;
	const double maxValue = 255.0;
	cv::Mat dest;
	cv::threshold(this->image, dest, threshold, maxValue, cv::THRESH_BINARY);
	return ImageInfo(std::move(dest));
}

ImageInfo ImageInfo::InvertColor() const {
	cv::Mat dest;
	cv::bitwise_not(this->image, dest);
	return ImageInfo(std::move(dest));
}

ImageInfo ImageInfo::Resize(const double Ratio) const {
	cv::Mat dest;
	cv::resize(this->image, dest, cv::Size(), Ratio, Ratio);
	return ImageInfo(std::move(dest));
}

cv::Mat& ImageInfo::ref() { return this->image; }

const cv::Mat& ImageInfo::ref() const { return this->image; }

cv::Size ImageInfo::GetSize() const { return cv::Size(this->image.cols, this->image.rows); }

void ImageInfo::WriteOut(const std::string& FileName) const {
	if (std::filesystem::exists(FileName.c_str())) return;
	cv::imwrite(FileName, this->image);
}

void ImageInfo::View(const std::string& WindowName) const {
	cv::imshow(WindowName, this->image);
}

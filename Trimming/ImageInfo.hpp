#ifndef __IMAGEINFO_HPP__
#define __IMAGEINFO_HPP__
#include <opencv2/opencv.hpp>
#include "Rect.hpp"

class ImageInfo {
private:
	cv::Mat image;
public:
	ImageInfo() = delete;
	ImageInfo(const std::string& FilePath);
private:
	ImageInfo(const cv::Mat& imagedata, const RECT& rc);
	ImageInfo(cv::Mat&& imagedata);
public:
	ImageInfo operator ~ ();
	ImageInfo trim(const RECT rc) const;
	ImageInfo ToGrayScale() const;
	ImageInfo Binarization() const;
	ImageInfo InvertColor() const;
	ImageInfo Resize(const double Ratio) const;
	cv::Mat& ref();
	const cv::Mat& ref() const;
	cv::Size GetSize() const;
	void WriteOut(const std::string& OutputFileName) const;
	void View(const std::string& WindowName) const;
};

#endif

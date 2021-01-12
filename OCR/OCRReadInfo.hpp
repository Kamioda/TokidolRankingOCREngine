#ifndef __OCRREADINFO_HPP__
#define __OCRREADINFO_HPP__
#include <opencv2/core/core.hpp>

struct OCRReadInfo {
	std::string text;
	std::vector<cv::Rect> boxes;
	std::vector<std::string> words;
	std::vector<float> confidences;
};
#endif

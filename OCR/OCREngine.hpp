#pragma once
#include "OCRReadInfo.hpp"
#include <opencv2/text.hpp>

class OCREngine {
private:
	cv::Ptr<cv::text::OCRTesseract> Engine;
public:
	OCREngine() = delete;
	OCREngine(const std::string& LangFileRootDir, const std::string& Language);
	OCREngine(const OCREngine&) = delete;
	OCREngine(OCREngine&&) = default;
	OCREngine& operator = (const OCREngine&) = delete;
	OCREngine& operator = (OCREngine&&) = default;
	OCRReadInfo Load(cv::Mat& ReadImage);
};


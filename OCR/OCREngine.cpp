#include "OCREngine.hpp"

OCREngine::OCREngine(const std::string& LangFileRootDir, const std::string& Language)
	: Engine(cv::text::OCRTesseract::create(LangFileRootDir.c_str(), Language.c_str())) {}

OCRReadInfo OCREngine::Load(cv::Mat& ReadImage) {
	OCRReadInfo dest{};
	this->Engine->run(ReadImage, dest.text, &dest.boxes, &dest.words, &dest.confidences);
	return dest;
}

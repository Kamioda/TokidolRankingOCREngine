#ifndef __HTTPEXCEPTION_HPP__
#define __HTTPEXCEPTION_HPP__
class HttpException {
private:
	int ErrorCode;
	std::string ErrorMessage;
public:
	HttpException(const int ServerErrorCode = 500, const std::string& ServerErrorMessage = "Internal Server Error")
		: ErrorCode(ServerErrorCode),  ErrorMessage(ServerErrorMessage) {}
	int GetError() const noexcept { return this->ErrorCode; }
	std::string what() const noexcept { return this->ErrorMessage; }
};
#endif

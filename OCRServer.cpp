#include "Trimming/TrimmingMain.hpp"
#include "Trimming/HttpException.hpp"
#include "OCR/OCRMain.hpp"
#include <httplib.h>
#include <iostream>
constexpr unsigned short port = 40;

inline void Process(const httplib::Request& req, httplib::Response& res, const std::function<std::string(const std::string&)>& ProcessFunc) {
	try {
		res.body = ProcessFunc(req.body);
		res.status = 200;
	}
	catch (const HttpException& hex) {
		res.status = hex.GetError();
		res.body = hex.what();
	}
	catch (const std::exception& er) {
		res.status = 500;
		res.body = er.what();
	}
}

inline void RunTrim(const httplib::Request& req, httplib::Response& res) { Process(req, res, Trim); }

inline void RunOCR(const httplib::Request& req, httplib::Response& res) { Process(req, res, OCR); }

inline bool CommandLineIsMatch(const std::string& arg, const std::string& comp) { return arg == comp; }

int main(int argc, char* argv[]) {
	try {
		unsigned short listenPort = port;
		if (argc >= 3 && (CommandLineIsMatch(argv[1], "--port") || CommandLineIsMatch(argv[1], "-p"))) listenPort = static_cast<unsigned short>(std::stoul(argv[2]));
		httplib::Server server{};
		static bool StopRequestIsReceived = false;
		server.Post("/trim", RunTrim);
		server.Post("/ocr", RunOCR);
		server.Get("/stop", [](const httplib::Request&, httplib::Response&) { StopRequestIsReceived = true; });
		std::cout << "Start Server" << std::endl;
		std::cout << "port: " << listenPort << std::endl;
		server.listen("localhost", listenPort, 0, 
			[&server]() {
				if (StopRequestIsReceived) {
					std::chrono::milliseconds interval(1000);
					std::this_thread::sleep_for(interval);
					server.stop();
				}
			}
		);
	}
	catch (const std::exception& er) {
		std::cerr << er.what() << std::endl << std::endl;
		std::cout << "------------Tokimeki Idol Score Ranking OCR Engine------------" << std::endl;
		std::cout << "tokidolocreng [--start|-s|--terminate|-t] --port [listen port]" << std::endl << std::endl;
		std::cout << "--start -s            Start OCR Server" << std::endl;
		std::cout << "--terminate -t        Terminate OCR Server" << std::endl;
		std::cout << "--port [listen port]  (option)Reserve server listen port" << std::endl;
	}
	return 0;
}


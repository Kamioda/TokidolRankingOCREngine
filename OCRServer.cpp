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
		if (argc < 2) throw std::runtime_error("command line arg is invalid");
		unsigned short listenPort = port;
		if (argc > 4 && (CommandLineIsMatch(argv[2], "--port") || CommandLineIsMatch(argv[2], "-p"))) listenPort = static_cast<unsigned short>(std::stoul(argv[3]));
		if (CommandLineIsMatch(argv[1], "--start") || CommandLineIsMatch(argv[1], "-s")) {
			httplib::Server server{};
			server.Post("/trim", RunTrim);
			server.Post("/ocr", RunOCR);
			server.Get("/stop", [&server](const httplib::Request&, httplib::Response&) { server.stop(); });
			server.listen("localhost", listenPort);
		}
		else if (CommandLineIsMatch(argv[1], "--terminate") || CommandLineIsMatch(argv[1], "-t")) {
			httplib::Client client("localhost", listenPort);
			client.set_connection_timeout(100);
			client.Get("/stop");
		}
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


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
	if (argc != 2) return 0;
	if (CommandLineIsMatch(argv[1], "--start") || CommandLineIsMatch(argv[1], "-s")) {
		try {
			httplib::Server server{};
			server.Post("/trim", RunTrim);
			server.Post("/ocr", RunOCR);
			server.Get("/stop", [&server](const httplib::Request&, httplib::Response&) { server.stop(); });
			server.listen("localhost", port);
		}
		catch (const std::exception& er) {
			std::cerr << er.what() << std::endl;
		}
	}
	else if (CommandLineIsMatch(argv[1], "--terminate") || CommandLineIsMatch(argv[1], "-t")) {
		httplib::Client client("localhost", port);
		client.Get("/stop");
	}
	return 0;
}


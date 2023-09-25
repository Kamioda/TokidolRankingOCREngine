#include <httplib.h>
#include <iostream>
constexpr unsigned short defaultPort = 40;

inline bool CommandLineIsMatch(const std::string& arg, const std::string& comp) { return arg == comp; }

int main(int argc, char* argv[]) {
	try {
		const unsigned short listenPort = (argc >= 3 && (CommandLineIsMatch(argv[1], "--port") || CommandLineIsMatch(argv[1], "-p"))) 
			? static_cast<unsigned short>(std::stoul(argv[2]))
			: defaultPort;
		httplib::Client client("localhost", listenPort);
		auto res = client.Get("/stop");
		if (res->status != 200) throw std::runtime_error(res->body);
		std::cout << "Send terminate request!" << std::endl;
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


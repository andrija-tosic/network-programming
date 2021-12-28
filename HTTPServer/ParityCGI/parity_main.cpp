#include <string>
#include <map>
#include <regex>
#include <iostream>
#include <fstream>
#include <thread>

#include <io.h>
#include <fcntl.h>

void returnResponse(const char* res) {
	std::cout << "Content-Type: text/html\n"
		<< "<html><body>\n"
		<< "<h1>Response: " << res << "</h1>\n"
		<< "</body></html>\n\n";
}

std::map<std::string, std::string> parse(std::string query_string) {
	

	std::map<std::string, std::string> retval;
	std::regex pattern("([\\w+%]+)=([^&]*)");
	auto begin = std::sregex_iterator(query_string.begin(), query_string.end(), pattern);
	auto end = std::sregex_iterator();
	for (std::sregex_iterator i = begin; i != end; i++)
	{
		std::string key = (*i)[1].str();
		std::string value = (*i)[2].str();
		retval[key] = value;;
	}

	return retval;
}

int main() {
	
	std::string query_string(getenv("QUERY_STRING"));
	
	if (query_string.empty()) {
		returnResponse("Query string is NULL");
		return -1;
	}
	else {
		std::map<std::string, std::string> map = parse(std::string(query_string));
		int number = atoi(map["num"].c_str());

		char* buf;

		const char* file_name;

		if (number % 2 == 0)
			file_name = "./Bruce_Willis.jpg";
		else
			file_name = "./wallpaper.jpg";

		std::ifstream file(file_name, std::ios::binary);

		file.seekg(0, std::ios::end);
		long size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::cout
			<< "Content-Type: image/jpeg\r\n"
			<< "Content-Length: " << size << "\r\n\r\n";


		buf = new char[size];

		file.read(buf, size);

		_setmode(_fileno(stdout), _O_BINARY);
		std::cout.write(buf, size);
		fflush(stdout);

		delete[] buf;

		file.close();
	}
	return 0;
}
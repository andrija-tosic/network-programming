#include <string>
#include <map>
#include <regex>
#include <iostream>

std::map<std::string, std::string> parse(std::string& query_string) {

	std::map<std::string, std::string> retval;
	std::regex pattern("([\\w.+%]+)=([^&]*)");
	auto begin = std::sregex_iterator(query_string.begin(), query_string.end(), pattern);
	auto end = std::sregex_iterator();
	for (std::sregex_iterator i = begin; i != end; i++)
	{
		std::string key = (*i)[1].str();
		std::string value = (*i)[2].str();
		retval[key] = value;
	}
	return retval;
}
void returnResponse(const char* res) {
	std::cout << "Content-Type: text/html\n\n"
		<< "<html><body>\n"
		<< "<h1>Response: " << res << "</h1>\n"
		<< "</body></html>\n";
}

int main() {

	std::string query = getenv("QUERY_STRING");

	std::map<std::string, std::string> map = parse(query);

	if (atoi(map["img.x"].c_str()) > 150) {
		returnResponse("<b>Desni</b> deo slike je kliknut");
	}
	else {
		returnResponse("<b>Levi</b> deo slike je kliknut");
	}

	return 0;
}
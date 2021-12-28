#include <string>
#include <map>
#include <regex>
#include <iostream>

std::map<std::string, std::string> parse(std::string& query_string) {

	std::map<std::string, std::string> retval;
	std::regex pattern("([\\w+%]+)=([^&]*)");
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


	std::string POSTdata;
	std::cin >> POSTdata;

	returnResponse(POSTdata.c_str());
	
	int content_length = atoi(getenv("CONTENT_LENGTH"));

	std::map<std::string, std::string> map = parse(POSTdata);


	int num1 = atoi(map["num1"].c_str()),
		num2 = atoi(map["num2"].c_str()),
		res = 123;

	std::string op = map["op"];

	if (op == "plus")
	{
		res = num1 + num2;
	}
	else if (op == "minus") {

		res = num1 - num2;
	}
	else if (op == "times") {
		res = num1 * num2;
	}

	std::cout << "Content-Type: text/html\n\n"
		<< "<html><body>\n"
		<< "<h1>Result is: " << std::to_string(num1) << " " << op << " " << std::to_string(num2) << " = " << std::to_string(res) << "</h1>\n"
		<< "</body></html>\n";

	return 0;
}
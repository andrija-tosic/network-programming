#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

#define SERVER_PORT 8888
#define BUF_SIZE 1024

void printError(const std::string& message) {
	std::cout << message << " " << "Error code:" << WSAGetLastError() << std::endl;
}
void printMessage(char* message) {
	std::cout << "Primljeno od servera: " << message << std::endl;
}

int main() {
	WSAData wsa;
	SOCKET listeningSocket;
	if (WSAStartup(0x0202, &wsa) != 0)
		printError("Startup failed.");


	sockaddr_in server_addr;
	int addr_length = sizeof(server_addr);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(SERVER_PORT);


		//std::cout << "Povezujem se na server_addr..." << std::endl;
		if ((listeningSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
			printError("Listening socket not created");
	while (true) {
		std::string echoBuf;
		do
		{
			std::cout << "Unos: ";
			std::getline(std::cin, echoBuf);
		} while (echoBuf.empty());

		if (sendto(listeningSocket, echoBuf.c_str(), echoBuf.length(), NULL, (sockaddr*)&server_addr, addr_length) < 0)
			printError("Error: send() failed");

		int msgSize;
		std::vector<char> buff(BUF_SIZE);
		do
		{
			std::cout << "Cekam recv()..." << std::endl;
			msgSize = recvfrom(listeningSocket, buff.data(), buff.size(), NULL, (sockaddr*)&server_addr, &addr_length);
			if (msgSize < 0)
				printError("Error: recv() = " + msgSize);
			else if (msgSize == 0)
				break;
			else
				printMessage(buff.data());


		} while (msgSize == BUF_SIZE);
	}
	
	closesocket(listeningSocket);
	WSACleanup();
}
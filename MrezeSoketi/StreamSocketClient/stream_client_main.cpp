#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

//#define SERVER_PORT 8888
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
	server_addr.sin_family = AF_INET;

	std::string adresa_servera;
	int server_port;

	std::cout << "Unos adrese servera: ";
	std::cin >> adresa_servera;
	server_addr.sin_addr.s_addr = inet_addr(adresa_servera.c_str());
	//server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	std::cout << "Unos porta: ";
	std::cin >> server_port;
	server_addr.sin_port = htons(server_port);
	//server_addr.sin_port = htons(8888);


	//std::cout << "Povezujem se na server..." << std::endl;
	if ((listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		printError("Listening socket not created");

		int connectionStatus;
		do
		{
			connectionStatus = connect(listeningSocket, (struct sockaddr*)&server_addr, sizeof(server_addr));

			if (connectionStatus == SOCKET_ERROR)
				printError("connect() failed!");

		} while (connectionStatus == SOCKET_ERROR);

	while (true) {
		std::string echoBuf;
		do
		{
			std::cout << "Unos poruke: ";
			std::getline(std::cin, echoBuf);
		} while (echoBuf.empty());
		
		if (send(listeningSocket, echoBuf.c_str(), echoBuf.length(), NULL) < 0)
			printError("Error: send() failed");

		int msgSize;
		std::vector<char> buff(BUF_SIZE);
		do
		{
			std::cout << "Cekam recv()..." << std::endl;
			msgSize = recv(listeningSocket, buff.data(), BUF_SIZE, NULL);
			if (msgSize < 0)
				printError("Error: recv() = " + msgSize);
			else if (msgSize == 0)
				break;
			else
				printMessage(buff.data());

			std::cout << "Cekam recv()..." << std::endl;
			msgSize = recv(listeningSocket, buff.data(), BUF_SIZE, NULL);
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
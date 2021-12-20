#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

//#define SERVER_PORT 8888
#define BUF_SIZE 1024

void printError(const std::string& message) {
	std::cout << message << std::endl << "Error code:" << WSAGetLastError() << std::endl;
}
void printMessage(char* message) {
	std::cout << "Primljeno od klijenta: " << message << std::endl;
}

int main() {
	// Inicijalizacija winsoketa
	WSAData wsa;
	SOCKET listensock;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printError("Startup failed.");

	// Kreiranje listen soketa
	if ((listensock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		printError("Listening socket not created");

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	int server_port;

	std::cout << "Unos porta: ";
	std::cin >> server_port;

	server.sin_port = htons(server_port);

	// Bind-ovanje na adresu i port
	if (bind(listensock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		printError("bind() failed!");

	//Osluskivanje
	if (listen(listensock, 3) < 0)
		printError("listen() failed!");

		sockaddr_in client_addr;
		int cLen = sizeof(struct sockaddr_in);
		
		SOCKET clientSock;
	while (true) {
		//std::cout << "Cekam vezu od klijenta..." << std::endl;
		clientSock = accept(listensock, (struct sockaddr*)&client_addr, &cLen);
		
		if (clientSock == INVALID_SOCKET) {
			printError("Accept failed");
		}
		else {
			int msgSize;
			do
			{
				// Primanje poruke od klijenta
				std::vector<char> echoBuf(BUF_SIZE);

				msgSize = recv(clientSock, echoBuf.data(), echoBuf.size(), NULL);
				if (msgSize < 0) {
					printError("Error: recv() = " + msgSize);
				}
				else if (msgSize == 0)
					break;
				else
					printMessage(echoBuf.data());

				std::string poruka1 = "Prva poruka: " + std::string(echoBuf.data());
				std::string poruka2 = "Druga poruka: " + std::string(echoBuf.data());
				
				std::cout << "Salje se " << poruka1.data() << std::endl;

				std::cout << "sizeof(poruka1) = " << sizeof(poruka1) << std::endl;
				std::cout << "sizeof(poruka2) = " << sizeof(poruka2) << std::endl;


				if (send(clientSock, poruka1.c_str(), sizeof(poruka1), NULL) < 0) {
					printError("Error: Prvi send() failed");
				}

				std::cout << "Salje se " << poruka2.data() << std::endl;;

				if (send(clientSock, poruka2.c_str(), sizeof(poruka2), NULL) < 0) {
					printError("Error: Drugi send() failed");
				}

			} while (msgSize > 0);
		}
		closesocket(clientSock);

	}
	closesocket(listensock);
	WSACleanup();
	return 0;
}
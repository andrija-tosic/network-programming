#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

#define SERVER_PORT 8888
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
	SOCKET serversock;
	if (WSAStartup(0x0202, &wsa) != 0)
		printError("Startup failed.");

	// Kreiranje listen soketa
	if ((serversock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		printError("Listening socket not created");

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERVER_PORT);

	// Bind-ovanje na adresu i port
	if (bind(serversock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		printError("bind() failed!");

	// Nema osluskivanja

		sockaddr_in client_addr;
		int addr_length = sizeof(sockaddr);

	while (true) {
		//std::cout << "Cekam vezu od klijenta..." << std::endl;
			int msgSize;
			do
			{
				// Primanje poruke od klijenta
				std::vector<char> echoBuf(BUF_SIZE);

				msgSize = recvfrom(serversock, echoBuf.data(), echoBuf.size(), NULL, (sockaddr*)&client_addr, &addr_length);
				if (msgSize < 0) {
					printError("Error: recv() = " + msgSize);
				}
				else if (msgSize == 0)
					break;
				else
					printMessage(echoBuf.data());

				if (sendto(serversock, echoBuf.data(), msgSize, NULL, (sockaddr*)&client_addr, addr_length) < 0) {
					printError("Error: send() failed");
				}
			} while (msgSize > 0);

		closesocket(serversock);

	}
	closesocket(serversock);
	WSACleanup();
	return 0;
}
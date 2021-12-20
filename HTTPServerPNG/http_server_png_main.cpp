#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <iostream>
#include <string>
#include <fstream>

#include "../socket_funkcije.h"

#define SERVER_PORT 80
#define BUF_SIZE 1024

WSAData wsa;
SOCKET listening_socket;
SOCKET client_socket;
sockaddr_in server;
sockaddr_in client_addr;
int cLen = sizeof(struct sockaddr_in);

int main() {
	init(wsa);
	create_socket(listening_socket);
	bind_socket(listening_socket, server, SERVER_PORT);
	listen_socket(listening_socket);
	accept_socket(listening_socket, client_socket, client_addr);

	char request[BUF_SIZE];
	memset(request, 0, BUF_SIZE);

	recv_data(client_socket, request, BUF_SIZE);

	printMessage(request);

	std::string req(request);
	std::string filePath;

	if (req.substr(0, 4) == "GET ") {
		filePath = req.substr(4, req.find(" ", 4) - 4);
	}
	else {
		std::string err400 =
			"HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n";

		std::cin.get();
		return -1;
	}

	if (req.find("HTTP/1.1") == -1) {
		std::string err505 =
			"HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Type: text/html\r\n";

		std::cin.get();
		return -1;
	}

	std::ifstream slika(filePath, std::ios::binary);
	if (!slika.is_open()) {
		std::cout << "failed to open file" << std::endl;

		std::string err404 =
			"HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n";

		std::cin.get();
		return -1;
	}

	slika.seekg(0, std::ios::end);
	size_t file_size = slika.tellg();
	slika.seekg(0, std::ios::beg);

	if (slika.fail()) {
		std::cout << "failed to get size of file" << std::endl;

		std::cin.get();
		return -1;
	}

	std::string ok = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(file_size)
		+ "\r\nConnection: close\r\nContent-Type: image/png\r\n\r\n";

	send_data(client_socket, ok.c_str(), ok.size() + 1);

	char buff[BUF_SIZE];

	std::cout << "filesize: " << file_size << std::endl;

	size_t sent = 0;
	do
	{
		if (!slika.read(buff, min(BUF_SIZE, file_size - sent))) {
			break;
		}
		int bytes = slika.gcount();
		if (send_data(client_socket, buff, bytes) == -1) {
			break;
		}
		sent += bytes;
		std::cout << "sent " << sent << " bytes" << std::endl;
	} while (sent < file_size);


	closesocket(client_socket);
	closesocket(listening_socket);
	WSACleanup();
	return 0;
}
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <iostream>
#include <string>
#include <vector>

#include "../socket_funkcije.h"

#define SERVER_PORT 80
#define BUF_SIZE 1024

WSAData wsa;
SOCKET listening_socket;
sockaddr_in server_addr;

int main() {
	init(wsa);
	create_socket(listening_socket);
	connect_socket(listening_socket, server_addr, "127.0.0.1", SERVER_PORT);

	std::string adr_objekta;
	std::cout << "Adresa objekta: ";
	std::cin >> adr_objekta;

	std::string zahtev;

	zahtev += "GET " + adr_objekta + " HTTP/1.1" + "\r\n"
		+ "Connection: close" + "\r\n"
		+ "User - agent: Mozilla / 4.0" + "\r\n"
		+ "Accept : image / png" + "\r\n"
		+ "If-modified-since: Mon, 10 Dec 2020 00:00:00" + "\r\n";

	send_data(listening_socket, zahtev.c_str(), zahtev.size() + 1);
	
	char recv_msg[BUF_SIZE];
	std::cout << "Cekam recv()..." << std::endl;
	recv_data(listening_socket, recv_msg, BUF_SIZE);
	printMessage(recv_msg);

	closesocket(listening_socket);
	WSACleanup();
}
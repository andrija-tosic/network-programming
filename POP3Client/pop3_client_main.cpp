#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <iostream>
#include <string>

#include "../socket_funkcije.h"

WSAData wsa;
SOCKET listening_socket;
sockaddr_in server_addr;


int main() {

	init(wsa);
	create_socket(listening_socket);
	connect_socket(listening_socket, server_addr, "127.0.0.1", 110);

	char recv_ready[80];
	recv_data(listening_socket, recv_ready, 80);
	printMessage(recv_ready);

	if (strcmp(recv_ready, "+OK POP3 server ready") == 0) {
		send_data(listening_socket, "user andrija", sizeof("user andrija"));

		char recv_ok[80];
		recv_data(listening_socket, recv_ok, 80);
		
		if (strcmp(recv_ok, "+OK") == 0) {
			send_data(listening_socket, "pass andrija", sizeof("pass andrija"));

			char recv_login[80];
			recv_data(listening_socket, recv_login, 80);

			if (strcmp(recv_login, "+OK user successfully logged on") == 0) {
				std::string input;	
				do
				{
					std::getline(std::cin, input);

					send_data(listening_socket, input.c_str(), input.size() + 1);

					char buf[BUF_SIZE];
					recv_data(listening_socket, buf, BUF_SIZE);
					printMessage(buf);
				
				} while (input != "quit");
			}
		}
	}

	closesocket(listening_socket);
	WSACleanup();
}
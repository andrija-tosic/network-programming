#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <iostream>
#include <string>

#include "../socket_funkcije.h"
#include <vector>

#define POP3_MSG_SIZE 1024

WSAData wsa;
SOCKET listening_socket;

sockaddr_in server;
sockaddr_in client_addr;
SOCKET client_socket;


int main() {

	init(wsa);
	create_stream_socket(listening_socket);
	bind_socket(listening_socket, server, 110);
	listen_socket(listening_socket);
	accept_socket(listening_socket, client_socket, client_addr);

	std::vector<std::string> poruke = {
		"prva 1",
		"druga 2",
		"treca 3"
	};

	const char* server_ready = "+OK POP3 server ready";


	send_data(client_socket, server_ready, strlen(server_ready) + 1);

	/*
	char recv_username[80];
	char recv_password[80];

	recv_data(client_socket, recv_username, 80);
	printMessage(recv_username);

	send_data(client_socket, "+OK", sizeof("+OK"));

	recv_data(client_socket, recv_password, 80);
	printMessage(recv_password);

	const char* login_success = "+OK user successfully logged on";

	send_data(client_socket, login_success, strlen(login_success) + 1);

	*/

	while (true)
	{
		char recv_msg[1024];
		recv_data(client_socket, recv_msg, 1024);
		printMessage(recv_msg);

		std::string recv_string(recv_msg);

		if (recv_string.substr(0, 4) == "list") {
			int size_sum = 0;

			for (auto& msg : poruke) {
				size_sum += msg.size();
			}
			
			std::string list_response =
				"+OK "
				+ std::to_string(poruke.size())
				+ " messages ("
				+ std::to_string(size_sum)
				+ " octets)"
				+ "\n";

			std::string msg_list;

			for (int i = 0; i < poruke.size(); i++) {
				msg_list += std::to_string(i+1) + " " + std::to_string(poruke[i].size()) + "\n";
			}

			msg_list += ".";

			list_response += msg_list;

			send_data(client_socket, list_response.c_str(), list_response.size() + 1);
		}
		else if (recv_string.substr(0, 4) == "retr") {
			int msg_number = std::stoi(recv_string.substr(5, 6));

			int index = msg_number - 1;

			if (index < 0 || index >= poruke.size()) {
				std::string msg = "-ERR No such message\n";
				send_data(client_socket, msg.c_str(), msg.size() + 1);
			}
			else {
				std::string msg = "+OK " + std::to_string(poruke[index].size()) + " octets\n";

				msg += poruke[index] + "\n.\n";

				msg += "+OK message deleted\n";

				poruke.erase(poruke.begin() + index);

				send_data(client_socket, msg.c_str(), msg.size() + 1);
			}

		}
		else if (recv_string.substr(0, 4) == "dele") {
			send_data(client_socket, "+OK message deleted", sizeof("+OK message deleted"));
		}
		else if (recv_string.substr(0, 4) == "quit") {
			send_data(client_socket, "+OK POP3 server closing connection",
				sizeof("+OK POP3 server closing connection"));

			std::cin.get();
			break;
		}
	}

	closesocket(client_socket);
	closesocket(listening_socket);
	WSACleanup();
	return 0;
}
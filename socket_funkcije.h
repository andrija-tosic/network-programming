#define BUF_SIZE 1024

void printError(const char* message) {
	std::cout << message << std::endl << "Error code:" << WSAGetLastError() << std::endl;
	std::cin.get();
}
void printMessage(const char* message) {
	std::cout << "Primljeno: " << message << std::endl;
}

int send_data(SOCKET s, const char* data, int size) {
	int sent = 0;
	do {
		int send_size = send(s, data + sent, size - sent, NULL);

		if (send_size <= 0) {
			printError("send() failed");
			return -1;
		}

		sent += send_size;

	} while (sent < size);
}

int recv_data(SOCKET s, char* data, int size) {
	int recieved = 0;
	int recv_size = 0;
	do
	{
		int recv_size = recv(s, data + recieved, size - recieved, NULL);

		if (recv_size <= 0) {
			printError("recv() failed");
			return -1;
		}

		recieved += recv_size;

	} while (recv_size > 0);
}

void init(WSAData& wsa) {
	// Inicijalizacija winsoketa
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printError("Startup failed.");
}

void create_stream_socket(SOCKET& listening_socket) {
	// Kreiranje listen soketa
	if ((listening_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		printError("Listening socket not created");
}

void bind_socket(SOCKET& listening_socket, sockaddr_in& server, int server_port) {
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(server_port);

	// Bind-ovanje na adresu i port
	if (bind(listening_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		printError("bind() failed!");
}

void listen_socket(SOCKET& listening_socket) {
	//Osluskivanje
	if (listen(listening_socket, 3) < 0)
		printError("listen() failed!");
}

void connect_socket(SOCKET& listening_socket, sockaddr_in& server, const char* addr, int port) {
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(addr);
	server.sin_port = htons(port);


	int conn_status = connect(listening_socket, (struct sockaddr*)&server, sizeof(server));

	if (conn_status == SOCKET_ERROR)
		printError("connect() failed!");

}


void accept_socket(SOCKET& listening_socket, SOCKET& client_socket, sockaddr_in& client) {

	int cLen = sizeof(struct sockaddr_in);
	client_socket = accept(listening_socket, (struct sockaddr*)&client, &cLen);

	if (client_socket == INVALID_SOCKET) {
		printError("Accept failed");
	}
}

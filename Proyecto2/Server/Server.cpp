#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOMINMAX

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#undef byte

#include <iostream>
#include "Routes.h"
#include "Server.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void Server::start(int port) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    bind(server_fd, (sockaddr*)&server, sizeof(server));
    listen(server_fd, 5);

    cout << "Server corriendo en puerto " << port << endl;

    while (true) {
        SOCKET client;
        sockaddr_in client_addr;
        int c = sizeof(sockaddr_in);

        client = accept(server_fd, (sockaddr*)&client_addr, &c);

        char buffer[3000] = {0};
        recv(client, buffer, 3000, 0);

        string request(buffer);

        cout << "\n Request recibido:\n" << request << endl;

        string response = Routes::handle(request);

        send(client, response.c_str(), response.size(), 0);

        closesocket(client);
    }

    closesocket(server_fd);
    WSACleanup();
}
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOMINMAX

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#undef byte

#include <iostream>
#include <string>

#include "Routes.h"
#include "Server.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

/* Lectura del Http */
string recibirRequestCompleto(SOCKET client) {

    string request;
    char buffer[8192];
    int bytes = 0;

    cout << "\n========== NUEVA PETICIÓN ==========\n";

    // 1. leer headers
    while ((bytes = recv(client, buffer, sizeof(buffer), 0)) > 0) {

        request.append(buffer, bytes);

        cout.write(buffer, bytes);
        cout << "\n-----------------------------------\n";

        if (request.find("\r\n\r\n") != string::npos) {
            break;
        }
    }

    // 2. buscar content length
    size_t pos = request.find("Content-Length: ");

    if (pos == string::npos) {
        cout << ">>> SIN BODY\n";
        return request;
    }

    size_t end = request.find("\r\n", pos);
    int contentLength = stoi(request.substr(pos + 16, end - (pos + 16)));

    cout << ">>> Content-Length: " << contentLength << endl;

    size_t bodyStart = request.find("\r\n\r\n") + 4;
    int actual = request.size() - bodyStart;

    cout << ">>> Body inicial: " << actual << endl;

    // 3. completar body
    while (actual < contentLength) {

        bytes = recv(client, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;

        request.append(buffer, bytes);
        actual += bytes;

        cout << "\n--- BODY CHUNK ---\n";
        cout.write(buffer, bytes);
        cout << "\n------------------\n";
    }

    cout << "\n========== REQUEST FINAL ==========\n";
    cout << request << endl;

    return request;
}

/*Inicio del server*/
void Server::start(int port) {

    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "WSAStartup fallo\n";
        return;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    bind(server_fd, (sockaddr*)&server, sizeof(server));
    listen(server_fd, 10);

    cout << "Server corriendo en puerto " << port << endl;

    while (true) {

        SOCKET client;
        sockaddr_in client_addr{};
        int c = sizeof(client_addr);

        client = accept(server_fd, (sockaddr*)&client_addr, &c);

        if (client == INVALID_SOCKET) continue;

        string request = recibirRequestCompleto(client);

        if (request.empty()) {
            cout << "REQUEST VACÍO\n";
            closesocket(client);
            continue;
        }

        string resp = Routes::handle(request);

        send(client, resp.c_str(), (int)resp.size(), 0);

        shutdown(client, SD_BOTH);
        closesocket(client);
    }

    closesocket(server_fd);
    WSACleanup();
}
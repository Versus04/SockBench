#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr;
    int opt = 1;
    int addrLen = sizeof(serverAddr);
    char recvBuffer[BUFFER_SIZE] = {0};

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddr, (socklen_t *)&addrLen);
    if (clientSocket < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    while (true) {
        int bytesRead = read(clientSocket, recvBuffer, BUFFER_SIZE);
        if (bytesRead <= 0) break;
        send(clientSocket, recvBuffer, bytesRead, 0);
    }

    close(clientSocket);
    close(serverSocket);
    return 0;
}

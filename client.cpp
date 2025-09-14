#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>

#define PORT 8080
#define BUFFER_SIZE 4096
#define NUM_MESSAGES 1000
#define PAYLOAD_SIZE 64

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char recvBuffer[BUFFER_SIZE] = {0};

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Failed to create socket\n";
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        return -1;
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed\n";
        return -1;
    }

    std::string payload(PAYLOAD_SIZE, 'A');

    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_MESSAGES; i++) {
        send(clientSocket, payload.c_str(), PAYLOAD_SIZE, 0);
        read(clientSocket, recvBuffer, PAYLOAD_SIZE);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    double avgLatencyMs = (elapsed.count() * 1000) / NUM_MESSAGES;
    double throughputMBs = (NUM_MESSAGES * PAYLOAD_SIZE) / (elapsed.count() * 1024 * 1024);

    std::cout << "Sent " << NUM_MESSAGES << " messages of " << PAYLOAD_SIZE << " bytes\n";
    std::cout << "Average Latency: " << avgLatencyMs << " ms\n";
    std::cout << "Throughput: " << throughputMBs << " MB/s\n";

    close(clientSocket);
    return 0;
}

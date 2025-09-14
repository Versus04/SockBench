#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <fstream>

#define PORT 8080
#define BUFFER_SIZE 65536

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./client <message_size> <num_messages>\n";
        return -1;
    }

    int payloadSize = std::stoi(argv[1]);
    int numMessages = std::stoi(argv[2]);

    if (payloadSize > BUFFER_SIZE) {
        std::cerr << "Message size too large (max " << BUFFER_SIZE << ")\n";
        return -1;
    }

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

    std::string payload(payloadSize, 'A');

    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numMessages; i++) {
        send(clientSocket, payload.c_str(), payloadSize, 0);
        read(clientSocket, recvBuffer, payloadSize);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    double avgLatencyMs = (elapsed.count() * 1000) / numMessages;
    double throughputMBs = (numMessages * payloadSize) / (elapsed.count() * 1024 * 1024);

    std::cout << "Sent " << numMessages << " messages of " << payloadSize << " bytes\n";
    std::cout << "Average Latency: " << avgLatencyMs << " ms\n";
    std::cout << "Throughput: " << throughputMBs << " MB/s\n";

    std::ofstream csv("results.csv", std::ios::app);
    if (csv.tellp() == 0) {
        csv << "message_size,num_messages,avg_latency_ms,throughput_MBps\n";
    }
    csv << payloadSize << "," << numMessages << "," << avgLatencyMs << "," << throughputMBs << "\n";
    csv.close();

    close(clientSocket);
    return 0;
}

#include <iostream>
#include <string>
#include <cstdint>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int runClient(const char* serverIp, std::uint16_t port) {
    // Create TCP socket
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::perror("500 Internal Server Error");
        return 1;
    }

    // Build server address
    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port   = htons(port);
    if (::inet_pton(AF_INET, serverIp, &servAddr.sin_addr) <= 0) {
        std::perror("500 Internal Server Error");
        ::close(sock);
        return 1;
    }

    // Connect to server (single TCP connection for whole run)
    if (::connect(sock,
                  reinterpret_cast<sockaddr*>(&servAddr),
                  sizeof(servAddr)) < 0) {
        std::perror("500 Internal Server Error");
        ::close(sock);
        return 1;
    }

    // Main loop: one command -> one response
    while (true) {
        // 1. Read one command from user (console)
        std::string line;
        if (!std::getline(std::cin, line)) {
            // EOF on stdin -> exit client
            break;
        }

        // Append newline as required by the protocol
        line.push_back('\n');

        // 2. Send the entire command to the server (handle partial sends)
        const char* data = line.data();
        std::size_t left = line.size();
        while (left > 0) {
            ssize_t sent = ::send(sock, data, left, 0);
            if (sent < 0) {
                std::perror("500 Internal Server Error");
                ::close(sock);
                return 1;
            }
            data += sent;
            left -= static_cast<std::size_t>(sent);
        }

        // 3. Wait for server response and print it.
        //    We read until we see that the last byte we got is '\n',
        //    as specified: each server reply ends with newline.
        std::string reply;
        while (true) {
            char buf[4096];
            ssize_t n = ::recv(sock, buf, sizeof(buf), 0);
            if (n < 0) {
                std::perror("400 Bad Request");
                ::close(sock);
                return 1;
            }
            if (n == 0) {
                ::close(sock);
                return 0;
            }

            reply.append(buf, static_cast<std::size_t>(n));

            // Stop when the server's reply (for this command) ends with '\n'
            if (!reply.empty() && reply.back() == '\n') {
                break;
            }
        }

        // Print the full reply from the server before reading next command
        std::cout << reply << std::flush;
    }

    ::close(sock);
    return 0;
}

// main receives server IP and port, as required by the assignment
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::perror("400 Bad Request");
        return 1;
    }

    const char* serverIp = argv[1];
    std::uint16_t port =
        static_cast<std::uint16_t>(std::stoi(argv[2]));

    return runClient(serverIp, port);
}

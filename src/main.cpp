#include <iostream>
#include <string>
#include <cstdlib>

#include "TcpServer.h"
#include "FileStorage.h"
#include "RLECompressor.h"
#include "RLEDecompressor.h"
#include "ThreadPerClientRunner.h"
#include "ThreadPoolRunner.h"

int main(int argc, char* argv[]) {
    // Requirement: port is passed as an argument to main 
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    // Parse port
    const std::uint16_t port =
        static_cast<std::uint16_t>(std::stoi(argv[1]));

    // read base directory from RLE_STORAGE_DIR
    const char* envPath = std::getenv("RLE_STORAGE_DIR");
    if (!envPath || std::string(envPath).empty()) {
        std::cerr << "500 Internal Server Error";
        return 1;
    }
    const std::string baseDir = envPath;

    try {
        // Shared core objects
        FileStorage      storage(baseDir);
        RLECompressor    compressor;
        RLEDecompressor  decompressor;

        // Create a runner that spawns one thread per client
        ThreadPerClientRunner runner;
        ThreadPoolRunner runner(10);

        // Start multi-threaded TCP server with injected runner
        TcpServer server(port, storage, compressor, decompressor, runner);
        server.run();   // blocking loop: accept + runner_.submit(clientFd)
    } catch (const std::exception& ex) {
        std::cerr << "500 Internal Server Error" << ex.what() << '\n';
        return 1;
    }

    return 0;
}

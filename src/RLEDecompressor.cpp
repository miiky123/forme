#include "RLEDecompressor.h"
#include <string>
#include <stdexcept>
#include <cctype>

using std::string;
using std::invalid_argument;

string RLEDecompressor::decompress(const string& compressed_data) {
    // 1. Empty input - empty output
    if (compressed_data.empty()) {
        return "";
    }

    // 2. Must be an even length: <char><digit> pairs
    if (compressed_data.length() % 2 != 0) {
        throw invalid_argument("400 Bad Request");
    }

    string decompressed_result;

    // 3. Process pairs: [i] is the character, [i+1] is the count digit
    for (std::size_t i = 0; i < compressed_data.length(); i += 2) {
        char character  = compressed_data[i];
        char count_char = compressed_data[i + 1];

        // Count must be a digit
        if (!std::isdigit(static_cast<unsigned char>(count_char))) {
            throw invalid_argument("400 Bad Request");
        }

        // Count must not be zero
        if (count_char == '0') {
            throw invalid_argument("400 Bad Request");
        }

        int count = count_char - '0';

        // Append character count times
        decompressed_result.append(static_cast<std::size_t>(count), character);
    }

    return decompressed_result;
}

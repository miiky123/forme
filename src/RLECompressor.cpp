#include "RLECompressor.h"

using std::string;

string RLECompressor::compress(const string& input) const {
    if (input.empty()) {
        return "";
    }

    string encoded;

    char current = input[0];
    int run_length = 1;

    for (std::size_t i = 1; i < input.size(); ++i) {
        char c = input[i];

        if (c == current) {
            ++run_length;
        } else {
            // flush run of current with length run_length
            int remaining = run_length;
            while (remaining > 9) {
                encoded.push_back(current);
                encoded.push_back('9');
                remaining -= 9;
            }
            if (remaining > 0) {
                encoded.push_back(current);
                encoded.push_back(static_cast<char>('0' + remaining));
            }

            current = c;
            run_length = 1;
        }
    }

    // flush the last run
    int remaining = run_length;
    while (remaining > 9) {
        encoded.push_back(current);
        encoded.push_back('9');
        remaining -= 9;
    }
    if (remaining > 0) {
        encoded.push_back(current);
        encoded.push_back(static_cast<char>('0' + remaining));
    }

    return encoded;
}

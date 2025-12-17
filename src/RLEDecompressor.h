#ifndef RLE_DECOMPRESSOR_H
#define RLE_DECOMPRESSOR_H

#include <string>
#include <stdexcept>

// Class responsible for decompressing strings encoded with simple RLE format.
// Expected format: <char><digit><char><digit>...
// Example: "A3B2" → "AAABB"
class RLEDecompressor {
public:
    // Default constructor – nothing to initialize
    RLEDecompressor() = default; 

    // Decompresses an RLE-encoded string.
    // Throws std::invalid_argument on malformed input.
    std::string decompress(const std::string& compressed_data);
};

#endif

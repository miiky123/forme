#ifndef RLECOMPRESSOR_H
#define RLECOMPRESSOR_H

#include <string>

class RLECompressor {
public:
    RLECompressor() = default;

    // Compress a plain string using RLE into <char><digit> pairs.
    std::string compress(const std::string& input) const;
};

#endif // RLECOMPRESSOR_H

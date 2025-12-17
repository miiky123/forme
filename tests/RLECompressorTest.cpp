// RLECompressorTest.cpp

#include <gtest/gtest.h>
#include "RLECompressor.h"
#include <string>

using namespace std;

TEST(RLECompressorTest, CompressSimpleString) {
    RLECompressor compressor;

    // 1. Compress a standard mixed sequence
    const string input    = "AAABBCDDDD";
    const string expected = "A3B2C1D4";

    EXPECT_EQ(compressor.compress(input), expected);
}

TEST(RLECompressorTest, CompressUncompressedString) {
    RLECompressor compressor;

    // 2. Compress a sequence with single occurrences
    const string input    = "ABCD";
    const string expected = "A1B1C1D1";

    EXPECT_EQ(compressor.compress(input), expected);
}

TEST(RLECompressorTest, CompressHomogeneousSequence) {
    RLECompressor compressor;

    // 3. Compress a single homogeneous sequence
    const string input(9, 'X');   // "XXXXXXXXX"
    const string expected = "X9";

    EXPECT_EQ(compressor.compress(input), expected);
}

TEST(RLECompressorTest, CompressStringContainingDigits) {
    RLECompressor compressor;

    // 4. Compress content where a digit is the character being repeated
    const string input    = "ABB9999CCCC";
    const string expected = "A1B294C4";

    EXPECT_EQ(compressor.compress(input), expected);
}

TEST(RLECompressorTest, HandleEmptyInput) {
    RLECompressor compressor;

    // 5. Empty input should result in an empty string
    const string input    = "";
    const string expected = "";

    EXPECT_EQ(compressor.compress(input), expected);
}

TEST(RLECompressorTest, LongRunIsSplitIntoChunksOfNine) {
    RLECompressor compressor;

    // 6. A long run should be split into multiple <char><digit> pairs
    const string input(25, 'Z');   // 
    const string expected = "Z9Z9Z7";

    EXPECT_EQ(compressor.compress(input), expected);
}

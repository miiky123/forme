// RLEDecompressorTest.cpp

#include <gtest/gtest.h>
#include "RLEDecompressor.h" 
#include <string>
#include <stdexcept> 

using namespace std;

TEST(RLEDecompressorTest, DecompressSimpleString) {
    RLEDecompressor processor;
    // 1. Decompress a standard mixed sequence
    const std::string compressed = "A3B2C1D4";
    const std::string expected = "AAABBCDDDD"; 
    
    EXPECT_EQ(processor.decompress(compressed), expected); 
}

TEST(RLEDecompressorTest, DecompressUncompressedString) {
    RLEDecompressor processor;
    // 2. Decompress sequence with single character occurrences
    const std::string compressed = "A1B1C1D1";
    const std::string expected = "ABCD"; 
    
    EXPECT_EQ(processor.decompress(compressed), expected); 
}

TEST(RLEDecompressorTest, DecompressHomogeneousSequence) {
    RLEDecompressor processor;
    // 3. Decompress a single homogeneous sequence
    const std::string compressed = "X9";
    // Using std::string(count, char) constructor to create expected output
    const std::string expected(9, 'X'); 
    
    EXPECT_EQ(processor.decompress(compressed), expected); 
}

TEST(RLEDecompressorTest, DecompressStringContainingDigits) {
    RLEDecompressor processor;
    // 4. Decompress content where a digit is the character being repeated
    const std::string compressed = "A1B294C4";
    const std::string expected = "ABB9999CCCC"; 
    
    EXPECT_EQ(processor.decompress(compressed), expected); 
}

TEST(RLEDecompressorTest, HandleEmptyInput) {
    RLEDecompressor processor;
    // 5. Empty input should result in an empty string
    const std::string compressed = "";
    const std::string expected = ""; 
    
    EXPECT_EQ(processor.decompress(compressed), expected); 
}

TEST(RLEDecompressorTest, InvalidLengthThrowsException) {
    RLEDecompressor processor;
    // 6. Input must have an even length 
    EXPECT_THROW(processor.decompress("A1B"), std::invalid_argument);
}

TEST(RLEDecompressorTest, StartsWithDigitThrowsException) {
    RLEDecompressor processor;
    // 7. RLE must start with the character, not the count
    EXPECT_THROW(processor.decompress("3A"), std::invalid_argument);
}

TEST(RLEDecompressorTest, ZeroCountThrowsException) {
    RLEDecompressor processor;
    // 8. The count (digit) must be positive and non-zero
    EXPECT_THROW(processor.decompress("A0B1"), std::invalid_argument);
}


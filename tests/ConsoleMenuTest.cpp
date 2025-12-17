#include <gtest/gtest.h>
#include "ConsoleMenu.h"

#include <sstream>
#include <string>

using namespace std;

TEST(ConsoleMenuTest, ReadsSingleLineCorrectly) {
    // user types "get\n"
    istringstream input("get\n");
    ostringstream output;

    ConsoleMenu menu(input, output);

    std::string line;
    bool ok = menu.nextCommand(line);

    EXPECT_TRUE(ok);
    EXPECT_EQ("get", line);
    EXPECT_EQ("", output.str());  // no prompts / errors
}

TEST(ConsoleMenuTest, ReturnsFalseOnEOF) {
    istringstream input("");  // empty input  EOF 
    ostringstream output;

    ConsoleMenu menu(input, output);

    std::string line = "initial";
    bool ok = menu.nextCommand(line);

    EXPECT_FALSE(ok);
    // line may stay unchanged or become empty;  only care that ok equal false
    EXPECT_EQ("", output.str());  // still no prints
}

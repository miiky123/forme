#include "ConsoleOutputChannel.h"
#include "IOutputChannel.h"

#include <gtest/gtest.h>
#include <sstream>

TEST(ConsoleOutputChannelTest, WritesToStream) {
    std::ostringstream oss;
    ConsoleOutputChannel out(oss);

    out.write("Hello");
    out.write(" World\n");

    EXPECT_EQ(oss.str(), "Hello World\n");
}

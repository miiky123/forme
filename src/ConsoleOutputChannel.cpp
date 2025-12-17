#include "ConsoleOutputChannel.h"

ConsoleOutputChannel::ConsoleOutputChannel(std::ostream& out)
    : out_(out) {
}

void ConsoleOutputChannel::write(const std::string& data) {
    out_ << data;
}

#ifndef CONSOLE_OUTPUT_CHANNEL_H
#define CONSOLE_OUTPUT_CHANNEL_H

#include "IOutputChannel.h"
#include <ostream>
#include <string>

class ConsoleOutputChannel : public IOutputChannel {
public:
    explicit ConsoleOutputChannel(std::ostream& out);

    void write(const std::string& data) override;

private:
    std::ostream& out_;
};

#endif // CONSOLE_OUTPUT_CHANNEL_H

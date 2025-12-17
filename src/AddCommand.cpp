#include "AddCommand.h"
#include <stdexcept>

using std::string;
using std::vector;

AddCommand::AddCommand(IStorage& storage,
                       RLECompressor& compressor,
                       IOutputChannel& out)
    : storage_(storage)
    , compressor_(compressor)
    , out_(out) {}

void AddCommand::execute(const vector<string>& args) {
    // Expected input line: POST <fileName> <text possibly with spaces>
    // App::run splits by spaces, so:
    //   args[0] = fileName
    //   args[1..] = pieces of the text
    if (args.empty()) {
        out_.write("400 Bad Request\n");
        return;
    }

    const string& fileName = args[0];

    // Join all remaining args into a single text with spaces
    string plainText;
    if (args.size() > 1) {
        plainText = args[1];
        for (std::size_t i = 2; i < args.size(); ++i) {
            plainText += ' ';
            plainText += args[i];
        }
    }

    try {
        const string compressed = compressor_.compress(plainText);
        storage_.save(fileName, compressed);
        out_.write("201 Created\n");
    } catch (const std::exception&) {
        // Any problem in compression or save is logical error
        out_.write("400 Bad Request\n");
    }
}

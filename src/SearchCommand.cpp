#include "SearchCommand.h"
#include <stdexcept>

using std::string;
using std::vector;

SearchCommand::SearchCommand(IStorage& storage,
                             RLEDecompressor& decompressor,
                             IOutputChannel& output)
    : storage_(storage)
    , decompressor_(decompressor)
    , output_(output)
{
}

void SearchCommand::execute(const vector<string>& args) {
    // Expected: search <query possibly with spaces>
    if (args.empty()) {
        //  invalid syntax 400
        output_.write("400 Bad Request\n");
        return;
    }

    // Join all args to a single query string
    string query = args[0];
    for (std::size_t i = 1; i < args.size(); ++i) {
        query += ' ';
        query += args[i];
    }

    if (query.empty()) {
        // Treat empty query 
        output_.write("400 Bad Request\n");
        return;
    }

    // Get the current list of files from storage at runtime
    std::vector<std::string> files = storage_.listFiles();

    string body;

    for (const auto& file_name : files) {
        try {
            const string compressed   = storage_.load(file_name);
            const string decompressed = decompressor_.decompress(compressed);

            // match by file name or file content
            bool matches = false;

            // match file name
            if (file_name.find(query) != string::npos) {
                matches = true;
            }

            // match decompressed content
            if (!matches &&
                decompressed.find(query) != string::npos) {
                matches = true;
            }

            if (matches) {
                //file names separated by spaces
                body += file_name;
                body += ' ';
            }
        } catch (...) {
            // Ignore any file that fails to load/decompress
        }
    }

    body += '\n';

    output_.write("200 Ok\n\n");
    output_.write(body);
}

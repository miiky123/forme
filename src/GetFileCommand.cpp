#include "GetFileCommand.h"
#include <stdexcept>
#include <sstream>

using std::string;
using std::vector;

GetFileCommand::GetFileCommand(IStorage& storage, RLEDecompressor& decompressor, IOutputChannel& output)
    : storage_(storage), decompressor_(decompressor), output_(output)
{}

void GetFileCommand::execute(const std::vector<std::string>& args) {
    // Expected: get <fileName>
    if (args.size() != 1) {
        output_.write("400 Bad Request\n");
        return;
    }

try {
    const std::string& file_name = args[0];

    const std::string compressed   = storage_.load(file_name);
    
    const std::string decompressed = decompressor_.decompress(compressed);

    // Only functional output: file contents
    output_.write("200 Ok\n\n");
    output_.write(decompressed);
    output_.write("\n");
    
} catch (const std::runtime_error& e) {

        if (std::string(e.what()).find("file not found") != std::string::npos) {
            output_.write("404 Not Found\n");
        } else {
            output_.write("404 Not Found\n");
        }
    } catch (const std::invalid_argument& e) {
        output_.write("404 Not Found\n");
    }
}

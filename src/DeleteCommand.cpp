#include "DeleteCommand.h"
#include <stdexcept>
#include <iostream>

using namespace std;

DeleteCommand::DeleteCommand(IStorage& storage, IOutputChannel& output)
: storage_(storage), output_(output) {}

void DeleteCommand::execute(const vector<string>& args) {
    
// Check for correct number of arguments
if (args.size() != 1) {
    throw invalid_argument("400 Bad Request");
}

const string& fileName = args[0];

try {
// Execute the removal
// Throws std::runtime_error if file is not found
    storage_.remove(fileName);

// Success: Print the required response for a successful DELETE
    output_.write("204 No Content\n"); 
} catch (const runtime_error& e) {

if (string(e.what()).find("file not found") != string::npos) {
    output_.write("404 Not Found\n");

} else {
    throw; 
}
}
}
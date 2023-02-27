#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

// Function to parse the Nginx configuration file
void parseNginxConfFile(std::string fileName, std::map<std::string, std::vector<std::string> >& confData) {
    std::ifstream file(fileName);
    std::string line;
    std::string currentKey;
    std::vector<std::string> currentValues;

    while (std::getline(file, line)) {
        // Remove comments and trim the line
        std::size_t pos = line.find('#');
        if (pos != std::string::npos) {
            line.erase(pos);
        }
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Check if this is a new key
        if (line.back() == '{') {
            currentKey = line.substr(0, line.size() - 1);
            currentValues.clear();
        } else if (line == "}") {
            confData[currentKey] = currentValues;
        } else {
            currentValues.push_back(line);
        }
    }

    file.close();
}

int main() {
    // Example usage
    std::map<std::string, std::vector<std::string> > confData;
    parseNginxConfFile("n.conf", confData);

    // Print the parsed data
    for (const auto& pair : confData) {
        std::cout << "Key: " << pair.first << std::endl;
        std::cout << "Values:" << std::endl;
        for (const auto& value : pair.second) {
            std::cout << "  " << value << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
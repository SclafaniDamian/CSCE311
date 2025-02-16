// Made by Damian Sclafani

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <pthread.h>

// Define the maximum number of elements
const int MAX_ELEMENTS = 220;

// Define a structure for (key, value) pairs
struct KeyValuePair {
    int key;
    std::string value;
};

// Define a Hash Table using open addressing
class HashMap {
private:
    std::vector<std::pair<int, std::string>> table[MAX_ELEMENTS];

public:
    // Insert a key-value pair into the map
    void insert(int key, const std::string& value) {
        int index = key % MAX_ELEMENTS;
        table[index].emplace_back(key, value);
    }

    // Delete a key from the map
    void remove(int key) {
        int index = key % MAX_ELEMENTS;
        auto& entries = table[index];
        entries.erase(std::remove_if(entries.begin(), entries.end(),
            [key](const auto& entry) { return entry.first == key; }), entries.end());
    }

    // Lookup a key in the map
    std::string lookup(int key) {
        int index = key % MAX_ELEMENTS;
        for (const auto& entry : table[index]) {
            if (entry.first == key) {
                return entry.second;
            }
        }
        return "No " + std::to_string(key);
    }
};

// Global variables
HashMap map;
pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to process a line from the input file
void processLine(const std::string& line) {
    char operation;
    int key;
    std::string value;
    std::istringstream stream(line);
    stream >> operation >> key;

    if (operation == 'I') {
        stream.ignore(); // Ignore the space
        std::getline(stream, value);
        pthread_mutex_lock(&map_mutex);
        map.insert(key, value);
        pthread_mutex_unlock(&map_mutex);
    } else if (operation == 'D') {
        pthread_mutex_lock(&map_mutex);
        map.remove(key);
        pthread_mutex_unlock(&map_mutex);
    } else if (operation == 'L') {
        pthread_mutex_lock(&map_mutex);
        std::cout << map.lookup(key) << std::endl;
        pthread_mutex_unlock(&map_mutex);
    }
}

// Function to read and process lines from the input file
void* processFile(void* arg) {
    const char* filename = reinterpret_cast<const char*>(arg);
    std::ifstream inputFile(filename);

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            processLine(line);
        }
        inputFile.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file relative path> <output file relative path>" << std::endl;
        return 1;
    }

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];

    // Create a thread to process the input file
    pthread_t fileThread;
    pthread_create(&fileThread, nullptr, processFile, const_cast<char*>(inputFile));

    // Wait for the file thread to finish
    pthread_join(fileThread, nullptr);

    // You can add the necessary code here to output the results to the specified output file

    return 0;
}

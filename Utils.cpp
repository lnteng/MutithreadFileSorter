#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <sstream>
#include <fstream>
#include <dirent.h>
#include <cstring>

using namespace std;


void generateData(const string& fileName, size_t dataSize) {
    ofstream outputFile(fileName, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "Failed to open file for writing." << endl;
        return;
    }

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int64_t> distribution;

    for (size_t i = 0; i < dataSize; ++i) {
        int64_t value = distribution(generator);
        outputFile.write(reinterpret_cast<char*>(&value), sizeof(int64_t));
    }

    outputFile.close();
}

void listFiles(const char* path, const char* extension) {
    DIR* dir = opendir(path);
    vector<string> filenames;

    if (dir != nullptr) {
        dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            // Check if the file has the specified extension
            if (entry->d_type == DT_REG) { // Regular file
                const char* fileExtension = strrchr(entry->d_name, '.');
                if (fileExtension != nullptr && strcmp(fileExtension, extension) == 0) {
                    filenames.push_back(entry->d_name);
                }
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Could not open directory: " << path << std::endl;
    }
}

int main() {
    int n = 100;
    for (int i = 1; i <= n; i++) {
        ostringstream oss;
        oss << "data/data" << i << ".bin";
        string fileName = oss.str();
        
        random_device rd;
        mt19937 generator(rd());
        uniform_int_distribution<size_t> distribution(100000, 5000000);
        size_t dataSize = distribution(generator); // 随机生成dataSize的大小

        generateData(fileName, dataSize);

    }
    return 0;
}


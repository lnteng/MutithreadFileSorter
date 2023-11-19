#include <iostream>
#include <fstream>
#include <random>

void generateData(const std::string& fileName, size_t dataSize) {
    std::ofstream outputFile(fileName, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    std::default_random_engine generator;
    std::uniform_int_distribution<int64_t> distribution;

    for (size_t i = 0; i < dataSize; ++i) {
        int64_t value = distribution(generator);
        outputFile.write(reinterpret_cast<char*>(&value), sizeof(int64_t));
    }

    outputFile.close();
}

int main() {
    std::string fileName = "data/data_file.bin";
    size_t dataSize = 100000; // Adjust the size as needed

    generateData(fileName, dataSize);

    return 0;
}

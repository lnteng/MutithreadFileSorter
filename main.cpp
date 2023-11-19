#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "ThreadPool.cpp" // Include the ThreadPool class

void sortFile(const std::string& inputFileName, const std::string& outputFileName) {
    // Read data from the input file
    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file for reading." << std::endl;
        return;
    }

    std::vector<int64_t> data;
    int64_t value;
    while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(int64_t))) {
        data.push_back(value);
    }

    inputFile.close();

    // Perform sorting
    std::sort(data.begin(), data.end());

    // Write sorted data to the output file
    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file for writing." << std::endl;
        return;
    }

    for (const auto& val : data) {
        outputFile.write(reinterpret_cast<const char*>(&val), sizeof(int64_t));
    }

    outputFile.close();
}

int main() {
    ThreadPool threadPool(4); // Number of threads in the pool

    std::vector<std::string> inputFiles = {"data/data_file1.bin", "data/data_file2.bin", "data/data_file3.bin"};
    std::string outputFileName = "data/sorted_output.bin";

    for (const auto& inputFileName : inputFiles) {
        threadPool.addTask([inputFileName, outputFileName]() {
            sortFile(inputFileName, outputFileName);
        });
    }

    // // Wait for all tasks to complete
    // threadPool.waitForCompletion();

    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "ThreadPool.h"
#include "Sorter.h"
using namespace std;

int main() {
    ThreadPool threadPool(4); // Number of threads in the pool

    vector<string> inputFiles = {"data/data_file1.bin", "data/data_file2.bin", "data/data_file3.bin"};
    string outputFileName = "data/sorted_output.bin";

    for (const auto& inputFileName : inputFiles) {
        threadPool.addTask([inputFileName, outputFileName]() {
            Sor(inputFileName, outputFileName);
        });
    }

    // Wait for all tasks to complete
    threadPool.waitForCompletion();
    string data1 = "data/data1.bin", data3 = "data/data3.bin";
    Sorter::externalSort(data1, data3, 1000);
    printf("%d\n", Sorter::isSortedFile(data3));
    return 0;
}

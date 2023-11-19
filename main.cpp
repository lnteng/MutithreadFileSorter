#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "ThreadPool.h"
#include "Sorter.h"
#include "Utils.cpp"
using namespace std;

int main() {
    ThreadPool threadPool(4); // Number of threads in the pool
    vector<string> sortedFileNames;

    vector<string> inputFiles = listFiles("data/");
    for (int i = 0; i < inputFiles.size(); i++) {
        string filename = inputFiles[i];
        string inputFileName = "data/" + filename;
        string outputFileName = "data/result/" + filename;
        sortedFileNames.push_back(outputFileName);
        threadPool.addTask([inputFileName, outputFileName, i]() {
            Sorter::externalSort(inputFileName, outputFileName, 1000 * i); // 避免临时文件名冲突
            printf("%s %d\n", outputFileName.c_str(), Sorter::isSortedFile(outputFileName));
        });
    }

    // // Wait for all tasks to complete
    // threadPool.waitForAllTasks();
    system("pause");

    // 启动合并线程
    while (sortedFileNames.size() > 1) {
        std::vector<std::string> mergedFilenames;

        for (size_t i = 0; i < sortedFileNames.size(); i += 2) {
            std::string inputFile1 = (i < sortedFileNames.size()) ? sortedFileNames[i] : "";
            std::string inputFile2 = (i + 1 < sortedFileNames.size()) ? sortedFileNames[i + 1] : "";
            std::string merged =  inputFile1 + "_" + inputFile2;
            size_t a = hash<string>{}(merged);
            string outputFile = "merged" + to_string(a);

            threadPool.addTask(
                [inputFile1, inputFile2, outputFile]() {
                    Sorter::mergeSortedFiles(inputFile1, inputFile2, outputFile); 
            });

            mergedFilenames.push_back(outputFile);
        }
        sortedFileNames = std::move(mergedFilenames);
    }

    return 0;
}

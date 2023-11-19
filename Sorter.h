#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>
#include <iterator>
#include <functional>
#include <filesystem>

using namespace std;

class Sorter {
public:
    static const size_t CHUNK_SIZE = 4 * 1024 * 1024; // 仅使用 4M 内存空间

    static void externalSort(const string& inputFileName, const string& outputFileName, int index);
    static void mergeSortedFiles(const std::string& inputFile1, const std::string& inputFile2, const std::string& outputFile);
    static bool isSortedFile(const string& inputFileName);
};
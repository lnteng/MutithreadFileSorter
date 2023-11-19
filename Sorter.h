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
    static const size_t CHUNK_SIZE = 4096;

    static void externalSort(const string& inputFileName, const string& outputFileName, int index);
    static bool isSortedFile(const string& inputFileName);
};
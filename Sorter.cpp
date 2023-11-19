#include "Sorter.h"
using namespace std;

void Sorter::externalSort(const string& inputFileName, const string& outputFileName, int index) {
    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "无法打开输入文件进行读取。" << endl;
        return;
    }

    size_t chunkNumber = 0;
    vector<int64_t> buffer;

    while (true) {
        buffer.resize(CHUNK_SIZE / sizeof(int64_t));
        size_t bytesRead = inputFile.readsome(reinterpret_cast<char*>(buffer.data()), CHUNK_SIZE);
        if (bytesRead == 0) {
            break;  // 到达文件末尾
        }

        buffer.resize(bytesRead / sizeof(int64_t));
        sort(buffer.begin(), buffer.end());

        ostringstream oss;
        oss << "data/tmp/chunk" << index << chunkNumber++ << ".tmp";
        string chunkFileName = oss.str();

        ofstream chunkFile(chunkFileName, ios::binary);
        chunkFile.write(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(int64_t));
    }

    inputFile.close();

    // 合并已排序的块
    vector<ifstream> chunkFiles(chunkNumber);
    for (size_t i = 0; i < chunkNumber; ++i) {
        ostringstream oss;
        oss << "data/tmp/chunk" << index << i << ".tmp";
        chunkFiles[i].open(oss.str(), ios::binary);

        if (!chunkFiles[i].is_open()) {
            cerr << "无法打开用于读取的临时块文件。" << endl;
            return;
        }
    }

    ofstream outputFile(outputFileName, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "无法打开输出文件进行写入。" << endl;
        return;
    }

    // 使用优先队列合并已排序的块
    priority_queue<pair<int64_t, size_t>, vector<pair<int64_t, size_t>>, greater<>> minHeap;

    for (size_t i = 0; i < chunkNumber; ++i) {
        int64_t value;
        if (chunkFiles[i].read(reinterpret_cast<char*>(&value), sizeof(int64_t))) {
            minHeap.emplace(value, i);
        }
    }

    while (!minHeap.empty()) {
        auto element = minHeap.top();
        int64_t value = element.first;
        int chunkIndex = element.second;
        minHeap.pop();

        outputFile.write(reinterpret_cast<char*>(&value), sizeof(int64_t));

        int64_t nextValue;
        if (chunkFiles[chunkIndex].read(reinterpret_cast<char*>(&nextValue), sizeof(int64_t))) {
            minHeap.emplace(nextValue, chunkIndex);
        }
    }

    // 关闭和删除临时块文件
    for (size_t i = 0; i < chunkNumber; ++i) {
        chunkFiles[i].close();
        filesystem::remove(("data/tmp/chunk" + to_string(index) + to_string(i) + ".tmp"));
    }
}

void Sorter::mergeSortedFiles(const std::string& inputFile1, const std::string& inputFile2, const std::string& outputFile) {
    std::ifstream file1(inputFile1, std::ios::binary);
    std::ifstream file2(inputFile2, std::ios::binary);
    std::ofstream output(outputFile, std::ios::binary);

    int value1, value2;

    // 将两个排好序的数据归并排序
    while (file1 >> value1 && file2 >> value2) {
        if (value1 <= value2) {
            output.write(reinterpret_cast<char*>(&value1), sizeof(value1));
        } else {
            output.write(reinterpret_cast<char*>(&value2), sizeof(value2));
        }
    }

    // 多出来的拷贝一下
    while (file1 >> value1) {
        output.write(reinterpret_cast<char*>(&value1), sizeof(value1));
    }

    while (file2 >> value2) {
        output.write(reinterpret_cast<char*>(&value2), sizeof(value2));
    }

    file1.close();
    file2.close();
    output.close();
}

bool Sorter::isSortedFile(const string& inputFileName) {
    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Failed to open file for reading." << endl;
        return false;
    }

    int64_t prevValue = INT64_MIN;
    int64_t currentValue;

    while (inputFile.read(reinterpret_cast<char*>(&currentValue), sizeof(int64_t))) {
        if (currentValue < prevValue) {
            cerr << "File is not sorted!" << endl;
            inputFile.close();
            return false;
        }
        prevValue = currentValue;
    }

    inputFile.close();
    return true;
}
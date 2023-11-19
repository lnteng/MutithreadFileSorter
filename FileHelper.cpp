#include <vector>
#include <fstream>
#include <stdexcept>  // 添加这行头文件

#include "FileHelper.h"

using namespace std;

// 打开单个文件
vector<int64_t> FileHelper::readDataFromFile(const string& fileName) {
    vector<int64_t> data;

    ifstream inputFile(fileName, ios::binary);
    if (!inputFile.is_open()) {
        throw runtime_error("Failed to open file for reading");
    }

    // 读取 int64 类型的数据
    int64_t value;
    while (inputFile.read(reinterpret_cast<char*>(&value), sizeof(int64_t))) {
        data.push_back(value);
    }

    inputFile.close();

    return data;
}

// 排好序之后的数据写入文件
void FileHelper::writeDataToFile(const vector<int64_t>& data, const string& fileName) {
    ofstream outputFile(fileName, ios::binary);

    if (!outputFile.is_open()) {
        throw runtime_error("Failed to open file for writing");
    }
    // 写入 int64 类型的数据
    for (const auto& value : data) {
        outputFile.write(reinterpret_cast<const char*>(&value), sizeof(int64_t));
    }

    outputFile.close();
}

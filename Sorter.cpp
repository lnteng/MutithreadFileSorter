
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <algorithm>
// #include <iterator>
// #include <cstdio>
// #include <queue>
// #include <sstream>

// #include "Sorter.h"


// const size_t Sorter::CHUNK_SIZE;
// // 归并
// void Sorter::mergeSortedFiles(const string& inputFile1, const string& inputFile2, const string& outputFile) {
//     ifstream file1(inputFile1, ios::binary);
//     ifstream file2(inputFile2, ios::binary);
//     ofstream output(outputFile, ios::binary);

//     int value1, value2;
//     // 将两个排好序的数据归并排序
//     while (file1 >> value1 && file2 >> value2) {
//         if (value1 <= value2) {
//             output.write(reinterpret_cast<char*>(&value1), sizeof(value1));
//         } else {
//             output.write(reinterpret_cast<char*>(&value2), sizeof(value2));
//         }
//     }

//     // 多出来的拷贝一下
//     while (file1 >> value1) {
//         output.write(reinterpret_cast<char*>(&value1), sizeof(value1));
//     }

//     while (file2 >> value2) {
//         output.write(reinterpret_cast<char*>(&value2), sizeof(value2));
//     }

//     file1.close();
//     file2.close();
//     output.close();
// }

// void Sorter::externalSort(const string& inputFileName, const string& outputFileName, int index) {
//     // Read chunks of data, sort them, and write to temporary files
//     ifstream inputFile(inputFileName, ios::binary);
//     if (!inputFile.is_open()) {
//         cerr << "Failed to open input file for reading." << endl;
//         return;
//     }

//     size_t chunkNumber = 0;
//     vector<int64_t> buffer;

//     size_t bytesRead = inputFile.gcount();
//     if (bytesRead >= Sorter::CHUNK_SIZE) {
//         // 调整 buffer 的大小以匹配实际读取的数据
//         bytesRead = Sorter::CHUNK_SIZE;
//     }
//     buffer.resize(bytesRead / sizeof(int64_t));
//     while (inputFile.read(reinterpret_cast<char*>(buffer.data()), bytesRead)) {
//         sort(buffer.begin(), buffer.end());

//         ostringstream oss;
//         oss << "data/tmp/chunk" << index << chunkNumber++ << ".tmp";
//         string chunkFileName = oss.str();

//         ofstream chunkFile(chunkFileName, ios::binary);
//         copy(buffer.begin(), buffer.end(), ostream_iterator<int64_t>(chunkFile));
//     }

//     inputFile.close();

//     // Merge sorted chunks
//     vector<ifstream> chunkFiles(chunkNumber);
//     for (size_t i = 0; i < chunkNumber; ++i) {
//         ostringstream oss;
//         oss << "data/tmp/chunk" << index << i << ".tmp";
//         chunkFiles[i].open(oss.str(), ios::binary);

//         if (!chunkFiles[i].is_open()) {
//             cerr << "Failed to open temporary chunk file for reading." << endl;
//             return;
//         }
//     }

//     ofstream outputFile(outputFileName, ios::binary | ios::app);
//     if (!outputFile.is_open()) {
//         cerr << "Failed to open output file for writing." << endl;
//         return;
//     }

//     // Merge sorted chunks using a priority queue
//     priority_queue<pair<int64_t, size_t>, vector<pair<int64_t, size_t>>, greater<>> minHeap;

//     for (size_t i = 0; i < chunkNumber; ++i) {
//         int64_t value;
//         if (chunkFiles[i].read(reinterpret_cast<char*>(&value), sizeof(int64_t))) {
//             minHeap.emplace(value, i);
//         }
//     }

//     while (!minHeap.empty()) {
//         auto [value, chunkIndex] = minHeap.top();
//         minHeap.pop();

//         outputFile.write(reinterpret_cast<char*>(&value), sizeof(int64_t));

//         int64_t nextValue;
//         if (chunkFiles[chunkIndex].read(reinterpret_cast<char*>(&nextValue), sizeof(int64_t))) {
//             minHeap.emplace(nextValue, chunkIndex);
//         }
//     }

//     // Close and remove temporary chunk files
//     for (size_t i = 0; i < chunkNumber; ++i) {
//         chunkFiles[i].close();
//         remove(("data/tmp/chunk" + to_string(index) + to_string(i) + ".tmp").c_str());
//     }
// }

// // 判断文件是否有序
// bool Sorter::isSortedFile(const string& inputFileName) {
//     ifstream inputFile(inputFileName, ios::binary);

//     vector<int> buffer(CHUNK_SIZE / sizeof(int));

//     while (!inputFile.eof()) {
//         inputFile.read(reinterpret_cast<char*>(buffer.data()), CHUNK_SIZE);
//         size_t bytesRead = inputFile.gcount() / sizeof(int);
//         if (bytesRead > 0) {
//             int pre = buffer[0];
//             for (int i = 1; i > bytesRead; i++) {
//                 if (buffer[i] < pre) {
//                     return false;
//                 } else {
//                     pre = buffer[i];
//                 }
//             }
//         }
//     }
//     return true;
// }
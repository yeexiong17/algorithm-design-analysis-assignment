#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

struct Data {
    int number;
    string str;
};

void merge(vector<Data>& arr, vector<Data>& temp, int l, int mid, int r) {
    // Left subarray
    int i = l;
    // Right subarray
    int j = mid + 1;
    int k = l;

    while (i <= mid && j <= r) {
        if (arr[i].number <= arr[j].number) {
            temp[k++] = std::move(arr[i++]);
        } else {
            temp[k++] = std::move(arr[j++]);
        }
    }

    while (i <= mid)
        temp[k++] = std::move(arr[i++]);
    while (j <= r)
        temp[k++] = std::move(arr[j++]);

    for (int x = l; x <= r; ++x) {
        arr[x] = std::move(temp[x]);
    }
}

void mergeSort(vector<Data>& arr, vector<Data>& temp, int l, int r) {
    if (l < r) {
        int mid = l + (r - l) / 2;
        mergeSort(arr, temp, l, mid);
        mergeSort(arr, temp, mid + 1, r);
        merge(arr, temp, l, mid, r);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n"; // can alter input/output when to compile the file
        return 1;
    }
    const string inputFile = argv[1];
    const string outputFile = argv[2];
    vector<Data> data;
    
    // Read input
    auto readStart = high_resolution_clock::now();
    ifstream inFile(inputFile);
    if (!inFile) {
        cerr << "Error opening input file: " << inputFile << endl;
        return 1;
    }
    
    string line;
    while (getline(inFile, line)) {
        size_t pos = line.find(',');
        if (pos != string::npos) {
            try {
                int num = stoi(line.substr(0, pos));
                string str = line.substr(pos + 1);
                data.push_back({num, std::move(str)});
            } catch (...) {
                cerr << "Error parsing line: " << line << endl;
            }
        }
    }
    inFile.close();
    auto readEnd = high_resolution_clock::now();
    
    // Sort data
    auto sortStart = high_resolution_clock::now();
    vector<Data> temp(data.size());
    mergeSort(data, temp, 0, data.size() - 1);
    auto sortEnd = high_resolution_clock::now();
    
    // Write output
    auto writeStart = high_resolution_clock::now();
    ofstream outFile(outputFile);
    if (!outFile) {
        cerr << "Error creating output file: " << outputFile << endl;
        return 1;
    }
    
    for (const auto& item : data) {
        outFile << item.number << "," << item.str << "\n";
    }
    outFile.close();
    auto writeEnd = high_resolution_clock::now();
    
    // Calculate durations
    auto readDuration = duration_cast<microseconds>(readEnd - readStart).count() / 1e6;
    auto sortDuration = duration_cast<microseconds>(sortEnd - sortStart).count() / 1e6;
    auto writeDuration = duration_cast<microseconds>(writeEnd - writeStart).count() / 1e6;
    auto totalDuration = duration_cast<microseconds>(writeEnd - readStart).count() / 1e6;
    
    // Print timings
    cout << "File: " << inputFile << " (" << data.size() << " records)\n";
    cout << fixed << setprecision(6);  // Set precision for floating point output
    cout << "Read time: " << readDuration << " s\n";
    cout << "Sort time: " << sortDuration << " s\n";
    cout << "Write time: " << writeDuration << " s\n";
    cout << "Total time: " << totalDuration << " s\n";
    
    return 0;
}

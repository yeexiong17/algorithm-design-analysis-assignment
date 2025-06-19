#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cctype>
#include <iomanip>

using namespace std;
using namespace std::chrono;

struct Data {
    int number;
    string str;
};

void merge(vector<Data>& arr, int l, int mid, int r) {
    int n1 = mid - l + 1;
    int n2 = r - mid;
    vector<Data> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].number <= R[j].number) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Data>& arr, int l, int r) {
    if (l < r) {
        int mid = l + (r - l) / 2;
        mergeSort(arr, l, mid);
        mergeSort(arr, mid + 1, r);
        merge(arr, l, mid, r);
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
                data.push_back({num, str});
            } catch (...) {
                cerr << "Error parsing line: " << line << endl;
            }
        }
    }
    inFile.close();
    auto readEnd = high_resolution_clock::now();
    
    // Sort data
    auto sortStart = high_resolution_clock::now();
    mergeSort(data, 0, data.size() - 1);
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

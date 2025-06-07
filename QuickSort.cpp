#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Read CSV
vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> row;
        while (getline(ss, token, ',')) {
            row.push_back(token);
        }
        data.push_back(row);
    }
    return data;
}

// Write sorted data
void writeCSV(const vector<vector<string>>& data, const string& filename) {
    ofstream file(filename);
    for (const auto& row : data) {
        file << row[0] << "," << row[1] << "\n";
    }
}

// Function to log a subarray
void logSubarray(ofstream& out, const vector<vector<string>>& data, int low, int high) {
    out << "[";
    for (int i = low; i <= high; ++i) {
        out << data[i][0] << "/" << data[i][1];
        if (i != high) out << ", ";
    }
    out << "]\n";
}

// Partition
int partition(vector<vector<string>>& data, int low, int high, ofstream& out) {
    long long pivot = stoll(data[high][0]);
    int i = low - 1;

    // Log the array before this partition
    if (low == 0 && high == static_cast<int>(data.size()) - 1) {
        cout << "Initial array:\n";
        logSubarray(out, data, low, high);
    }

    for (int j = low; j < high; ++j) {
        if (stoll(data[j][0]) <= pivot) {
            ++i;
            swap(data[i], data[j]);
        }
    }
    swap(data[i + 1], data[high]);

    // Log after this partition
    out << "pi=" << (i + 1) << " ";
    logSubarray(out, data, low, high);

    return i + 1;
}

// Quick Sort with logging
void quickSortWithLog(vector<vector<string>>& data, int low, int high, ofstream& out) {
    if (low < high) {
        int pi = partition(data, low, high, out);

        // Recursively sort elements before and after partition
        quickSortWithLog(data, low, pi - 1, out);
        quickSortWithLog(data, pi + 1, high, out);
    }
}

int main() {
    string input = "dataset_1000.csv";
    string output = "quick_sort_1000.csv";
    string stepOutput = "quick_sort_step_0_999.txt"; // adjust start/end rows as needed

    vector<vector<string>> data = readCSV(input);

    ofstream out(stepOutput);
    if (!out.is_open()) {
        cerr << "Failed to open output file!" << endl;
        return 1;
    }

    auto start = high_resolution_clock::now();
    quickSortWithLog(data, 0, data.size() - 1, out);
    auto stop = high_resolution_clock::now();

    writeCSV(data, output);

    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Quick Sort completed in " << fixed << setprecision(6)
         << duration.count() / 1e6 << " seconds" << endl;

    out.close();
    return 0;
}

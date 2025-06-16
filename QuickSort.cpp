#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cstdlib>   // For rand()
#include <ctime>     // For time()

using namespace std;
using namespace std::chrono;

// Read CSV
vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;  // skip empty lines
        stringstream ss(line);
        string token;
        vector<string> row;
        while (getline(ss, token, ',')) {
            row.push_back(token);
        }
        if (row.size() == 2) {  // Ensure valid format [key,value]
            data.push_back(row);
        }
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

// Log a subarray
void logSubarray(ofstream& out, const vector<vector<string>>& data, int low, int high) {
    out << "[";
    for (int i = low; i <= high; ++i) {
        out << data[i][0] << "/" << data[i][1];
        if (i != high) out << ", ";
    }
    out << "]\n";
}

// Partition using Lomuto scheme with random pivot
int partition(vector<vector<string>>& data, int low, int high, ofstream& out) {
    // Randomized pivot selection
    int randomIndex = low + rand() % (high - low + 1);
    swap(data[randomIndex], data[high]);

    long long pivot = stoll(data[high][0]);
    int i = low - 1;

    // Log the array before this partition
    if (low == 0 && high == static_cast<int>(data.size()) - 1) {
        cout << "Initial array:\n";
        logSubarray(out, data, low, high);
    }

    for (int j = low; j < high; ++j) {
        long long val = stoll(data[j][0]);
        if (val <= pivot) {
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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file.csv> <output_file.csv>\n";
        return 1;
    }

    string input = argv[1];
    string output = argv[2];

    // Generate log filename from input (e.g., dataset_1000.csv -> quick_sort_step_0_999.txt)
    size_t dotPos = input.find_last_of('.');
    string baseName = input.substr(0, dotPos);
    int size = readCSV(input).size();
    string stepOutput = "quick_sort_step_0_" + to_string(size - 1) + ".txt";

    vector<vector<string>> data = readCSV(input);

    ofstream out(stepOutput);
    if (!out.is_open()) {
        cerr << "Failed to open log file!" << endl;
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr))); // Seed for random pivot

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

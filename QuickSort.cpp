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

// Partition
int partition(vector<vector<string>>& data, int low, int high) {
    long long pivot = stoll(data[high][0]);
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (stoll(data[j][0]) <= pivot) {
            ++i;
            swap(data[i], data[j]);
        }
    }
    swap(data[i + 1], data[high]);
    return i + 1;
}

// Quick Sort
void quickSort(vector<vector<string>>& data, int low, int high) {
    if (low < high) {
        int pi = partition(data, low, high);
        quickSort(data, low, pi - 1);
        quickSort(data, pi + 1, high);
    }
}

int main() {
    string input = "dataset_sample_1000.csv";
    string output = "quick_sort_1000.csv";

    vector<vector<string>> data = readCSV(input);

    auto start = high_resolution_clock::now();
    quickSort(data, 0, data.size() - 1);
    auto stop = high_resolution_clock::now();

    writeCSV(data, output);

    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Quick Sort completed in " << fixed << setprecision(6)
         << duration.count() / 1e6 << " seconds" << endl;

    return 0;
}

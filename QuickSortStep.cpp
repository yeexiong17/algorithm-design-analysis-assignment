#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Trim whitespace from string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

vector<vector<string>> readCSV(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> row;
        while (getline(ss, token, ',')) {
            row.push_back(trim(token));
        }
        data.push_back(row);
    }
    return data;
}

// Print the entire array
void printArray(const vector<vector<string>>& data, ofstream& out) {
    out << "[";
    for (size_t i = 0; i < data.size(); ++i) {
        out << data[i][0] << "/" << data[i][1];
        if (i != data.size() - 1) out << ", ";
    }
    out << "]";
}

int partitionWithTrace(vector<vector<string>>& data, int low, int high, ofstream& out) {
    long long pivot = stoll(data[high][0]);
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (stoll(data[j][0]) <= pivot) {
            ++i;
            swap(data[i], data[j]);
        }
    }
    swap(data[i + 1], data[high]);

    // Print the pivot index and the entire array state
    out << "pi=" << (i + 1) << " ";
    printArray(data, out);
    out << endl;

    return i + 1;
}

void quickSortWithTrace(vector<vector<string>>& data, int low, int high, ofstream& out) {
    if (low < high) {
        int pi = partitionWithTrace(data, low, high, out);
        quickSortWithTrace(data, low, pi - 1, out);
        quickSortWithTrace(data, pi + 1, high, out);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <filename> <start_row> <end_row>" << endl;
        return 1;
    }

    string filename = argv[1];
    int start = stoi(argv[2])-1;
    int end = stoi(argv[3])-1;

    vector<vector<string>> data = readCSV(filename);
    
    vector<vector<string>> subList(data.begin() + start, data.begin() + end + 1);

    string outputFilename = "QuickSortStep" + to_string(start+1) + "_" + to_string(end+1) + ".txt";
    ofstream outFile(outputFilename);

    // Print initial array
    printArray(subList, outFile);
    outFile << endl;

    quickSortWithTrace(subList, 0, subList.size() - 1, outFile);

    outFile.close();
    cout << "Step-by-step trace written to " << outputFilename << endl;

    return 0;
}

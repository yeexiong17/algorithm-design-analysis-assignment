#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

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

int partitionWithTrace(vector<vector<string>>& data, int low, int high, ofstream& out, int& stepCount) {
    long long pivot = stoll(data[high][0]);
    int i = low - 1;

    out << "[";
    for (int k = low; k <= high; ++k) {
        out << data[k][0] << "/" << data[k][1];
        if (k != high) out << ", ";
    }
    out << "] pi=" << stepCount++;

    for (int j = low; j < high; ++j) {
        if (stoll(data[j][0]) <= pivot) {
            ++i;
            swap(data[i], data[j]);
        }
    }
    swap(data[i + 1], data[high]);

    out << "[" << i + 1 << "]" << endl;
    return i + 1;
}

void quickSortWithTrace(vector<vector<string>>& data, int low, int high, ofstream& out, int& stepCount) {
    if (low < high) {
        int pi = partitionWithTrace(data, low, high, out, stepCount);
        quickSortWithTrace(data, low, pi - 1, out, stepCount);
        quickSortWithTrace(data, pi + 1, high, out, stepCount);
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

    int stepCount = 0;
    quickSortWithTrace(subList, 0, subList.size() - 1, outFile, stepCount);

    outFile.close();
    cout << "Step-by-step trace written to " << outputFilename << endl;

    return 0;
}

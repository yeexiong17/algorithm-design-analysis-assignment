#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Trim whitespace from string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

struct Data {
    int number;
    string str;
};

string arrayToString(const  vector<Data>& arr) {
    ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << arr[i].number << "/" << arr[i].str;
    }
    oss << "]";
    return oss.str();
}

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

void mergeSort(vector<Data>& arr, int l, int r, vector<string>& steps) {
    if (l < r) {
        int mid = l + (r - l) / 2;
        mergeSort(arr, l, mid, steps);
        mergeSort(arr, mid + 1, r, steps);
        merge(arr, l, mid, r);
        steps.push_back(arrayToString(arr));
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <file_path> <start_row> <end_row>\n";
        return 1;
    }

    string filePath = argv[1];
    int startRow = stoi(argv[2]);
    int endRow = stoi(argv[3]);

    vector<Data> arr;
    ifstream inFile(filePath);
    if (!inFile) {
        cerr << "Error opening file: " << filePath << "\n";
        return 1;
    }

    string line;
    int currentRow = 0;
    while (getline(inFile, line)) {
        currentRow++;
        if (currentRow < startRow) continue;
        if (currentRow > endRow) break;

        size_t pos = line.find(',');
        if (pos != string::npos) {
            try {
                int num = stoi(line.substr(0, pos));
                string str = trim(line.substr(pos + 1));
                arr.push_back({num, str});
            } catch (...) {
                cerr << "Invalid format on line " << currentRow << "\n";
            }
        }
    }
    inFile.close();

    vector<string> steps;
    if (!arr.empty()) {
        steps.push_back(arrayToString(arr)); // Initial state
        mergeSort(arr, 0, arr.size() - 1, steps);
    }

    string stepFile = "merge_sort_step_" + to_string(startRow) + "_" + to_string(endRow) + ".txt";
    ofstream stepOut(stepFile);
    if (stepOut) {
        // Output all steps in one line, separated by " -> "
        for (size_t i = 0; i < steps.size(); ++i) {
            if (i > 0) {
                stepOut << "\n";
            }
            stepOut << steps[i];
        }
        stepOut << "\n";
    } else {
        cerr << "Error creating step file\n";
    }

    string sortedFile = "sorted_segment_" + to_string(startRow) + "_" + to_string(endRow) + ".csv";
    ofstream sortedOut(sortedFile);
    if (sortedOut) {
        for (const auto& data : arr) {
            sortedOut << data.number << "," << data.str << "\n";
        }
    } else {
        cerr << "Error creating sorted segment file\n";
    }

    return 0;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Entry {
    int number;
    string text;
};

// Custom merge sort
void merge(vector<Entry>& data, int left, int mid, int right) {
    vector<Entry> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        if (data[i].number <= data[j].number)
            temp[k++] = data[i++];
        else
            temp[k++] = data[j++];
    }
    
    while (i <= mid) temp[k++] = data[i++];
    while (j <= right) temp[k++] = data[j++];
    
    for (int x = 0; x < k; ++x)
        data[left + x] = temp[x];
}

void mergeSort(vector<Entry>& data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(data, left, mid);
        mergeSort(data, mid + 1, right);
        merge(data, left, mid, right);
    }
}

int binarySearchWithSteps(const vector<Entry>& data, int target, const string& logFile) {
    ofstream log(logFile);
    if (!log) {
        cerr << "Unable to create log file.\n";
        return -1;
    }

    int left = 0, right = data.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        log << (mid+1) << ": " << data[mid].number << "/" << data[mid].text << "\n";

        if (data[mid].number == target) {
            log << "Target " << target << " found at index " << mid << "\n";
            return mid;
        }
        else if (data[mid].number < target)
            left = mid + 1;
        else
            right = mid - 1;
    }
    log << "-1\n\n";
    log << "Target " << target << " not found\n";
    return -1;
}

vector<Entry> readDataset(const string& filename) {
    vector<Entry> dataset;
    ifstream file(filename);
    if (!file) {
        cerr << "Unable to open dataset file.\n";
        return dataset;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string numberStr, text;
        getline(ss, numberStr, ',');
        getline(ss, text);
        
        Entry e;
        e.number = stoi(numberStr);
        e.text = text;
        dataset.push_back(e);
    }
    return dataset;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <dataset.csv> <target_integer>\n";
        return 1;
    }

    try {
        string datasetFile = argv[1];
        int target = stoi(argv[2]);
        string logFile = "binary_search_step_" + to_string(target) + ".txt";

        vector<Entry> data = readDataset(datasetFile);
        if (data.empty()) {
            cerr << "Dataset is empty or invalid.\n";
            return 1;
        }

        mergeSort(data, 0, data.size() - 1);
        int foundIndex = binarySearchWithSteps(data, target, logFile);
        
        cout << "Target " << target << (foundIndex != -1 ? 
            " found at index " + to_string(foundIndex) : 
            " not found") << ". Log saved to " << logFile << "\n";
            
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

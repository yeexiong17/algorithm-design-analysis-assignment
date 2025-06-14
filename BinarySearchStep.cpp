#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int binarySearchWithSteps(const vector<vector<string>>& data, int low, int high,
                         const string& target, const string& logFile) {
    ofstream log(logFile);
    if (!log) {
        cerr << "Unable to create log file.\n";
        return -1;
    }

    while (low <= high) {
        int mid = (high + low) / 2;
        log << (mid+1) << ": " << data[mid][0] << "/" << data[mid][1] << "\n";
        
        cout << "Comparing target: '" << target << "' with data: '" << data[mid][0] << "'" << endl;

        if (data[mid][0] == target) {
            log << "Target " << target << " found at index " << mid << "\n";
            return mid;
        }

        if (data[mid][0].compare(target) > 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    log << "-1\n\n";
    log << "Target " << target << " not found\n";
    return -1;
}

vector<vector<string>> readDataset(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    if (!file) {
        cerr << "Unable to open dataset file.\n";
        return data;
    }

    string line, word;
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        while (getline(ss, word, ',')) {
            word.erase(0, word.find_first_not_of(" \t\r\n"));
            word.erase(word.find_last_not_of(" \t\r\n") + 1);
            row.push_back(word);
        }
        data.push_back(row);
    }
    return data;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <dataset.csv> <target>\n";
        return 1;
    }

    try {
        string datasetFile = argv[1];
        string target = argv[2];
        target.erase(0, target.find_first_not_of(" \t\r\n"));
        target.erase(target.find_last_not_of(" \t\r\n") + 1);
        
        string logFile = "binary_search_step_" + target + ".txt";

        vector<vector<string>> data = readDataset(datasetFile);
        if (data.empty()) {
            cerr << "Dataset is empty or invalid.\n";
            return 1;
        }

        cout << "Dataset size: " << data.size() << endl;
        cout << "First element: " << data[0][0] << endl;
        cout << "Last element: " << data[data.size()-1][0] << endl;
        cout << "Searching for target: " << target << endl;

        int foundIndex = binarySearchWithSteps(data, 0, data.size() - 1, target, logFile);
        
        cout << "Target " << target << (foundIndex != -1 ? 
            " found at index " + to_string(foundIndex) : 
            " not found") << ". Log saved to " << logFile << "\n";
            
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

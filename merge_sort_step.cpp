#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

struct Data {
    int number;
    std::string str;
};

std::string arrayToString(const std::vector<Data>& arr) {
    std::ostringstream oss;
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

void merge(std::vector<Data>& arr, int l, int mid, int r) {
    int n1 = mid - l + 1;
    int n2 = r - mid;
    std::vector<Data> L(n1), R(n2);

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

void mergeSort(std::vector<Data>& arr, int l, int r, std::vector<std::string>& steps) {
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
        std::cerr << "Usage: " << argv[0] << " <file_path> <start_row> <end_row>\n";
        return 1;
    }

    std::string filePath = argv[1];
    int startRow = std::stoi(argv[2]);
    int endRow = std::stoi(argv[3]);

    std::vector<Data> arr;
    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cerr << "Error opening file: " << filePath << "\n";
        return 1;
    }

    std::string line;
    int currentRow = 0;
    while (std::getline(inFile, line)) {
        currentRow++;
        if (currentRow < startRow) continue;
        if (currentRow > endRow) break;

        size_t pos = line.find(',');
        if (pos != std::string::npos) {
            try {
                int num = std::stoi(line.substr(0, pos));
                std::string str = line.substr(pos + 1);
                arr.push_back({num, str});
            } catch (...) {
                std::cerr << "Invalid format on line " << currentRow << "\n";
            }
        }
    }
    inFile.close();

    std::vector<std::string> steps;
    if (!arr.empty()) {
        steps.push_back(arrayToString(arr)); // Initial state
        mergeSort(arr, 0, arr.size() - 1, steps);
    }

    std::string stepFile = "merge_sort_step_" + std::to_string(startRow) + "_" + std::to_string(endRow) + ".txt";
    std::ofstream stepOut(stepFile);
    if (stepOut) {
        for (const auto& step : steps) {
            stepOut << step << "\n";
        }
    } else {
        std::cerr << "Error creating step file\n";
    }

    std::string sortedFile = "sorted_segment_" + std::to_string(startRow) + "_" + std::to_string(endRow) + ".csv";
    std::ofstream sortedOut(sortedFile);
    if (sortedOut) {
        for (const auto& data : arr) {
            sortedOut << data.number << "," << data.str << "\n";
        }
    } else {
        std::cerr << "Error creating sorted segment file\n";
    }

    return 0;
}
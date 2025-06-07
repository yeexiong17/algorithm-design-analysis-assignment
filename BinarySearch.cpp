#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

int binarySearch(const vector<vector<string>> &data, int low, int high,
                 const string &target) {
  while (low <= high) {
    int mid = (high + low) / 2;

    if (data[mid][0] == target) {
      return mid;
    }

    if (target < data[mid][0]) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  return -1;
}

// Measure search time for n iterations and repeat measurements multiple times
double measureSearchTime(const vector<vector<string>> &data,
                         const string &target, int n, int repetitions = 5) {
    double totalTime = 0.0;
    
    for (int rep = 0; rep < repetitions; rep++) {
        auto start = high_resolution_clock::now();
        
        for (int i = 0; i < n; i++) {
            binarySearch(data, 0, data.size() - 1, target);
        }
        
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        totalTime += duration.count() / 1000000.0;
    }
    
    return totalTime / repetitions;
}

int main() {
  ifstream MyFile("merge_sort_1000000.csv");
  string line, word;
  vector<vector<string>> data;

  while (getline(MyFile, line)) {
    vector<string> row;
    stringstream ss(line);
    while (getline(ss, word, ',')) {
      row.push_back(word);
    }
    data.push_back(row);
  }
  MyFile.close();

  int n = data.size();
  const int repetitions = 5;  // Number of times to repeat each measurement

  string bestCase = data[n / 2][0];
  double bestTime = measureSearchTime(data, bestCase, n, repetitions);

  // Use an element at roughly 1/4 of the array for average case
  string avgCase = data[n / 4][0];
  double avgTime = measureSearchTime(data, avgCase, n, repetitions);

  string worstCase = data[n - 1][0];
  double worstTime = measureSearchTime(data, worstCase, n, repetitions);

  ofstream outFile("binary_search_1000000.txt");
  outFile << "Binary Search Performance Analysis\n\n";

  outFile << "Number of searches performed for each case: " << n << "\n\n";
  outFile << fixed << setprecision(6);
  outFile << "Best case (middle element) time: " << bestTime << " seconds\n";
  outFile << "Average case (element at 1/4 of array) time: "
          << avgTime << " seconds\n";
  outFile << "Worst case (last element) time: " << worstTime << " seconds\n";
  outFile.close();

  cout << "Results have been written to binary_search_1000000.txt" << endl;

  return 0;
}
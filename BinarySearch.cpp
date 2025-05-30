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

// Measure search time for n iterations
double measureSearchTime(const vector<vector<string>> &data,
                         const string &target, int n) {
  auto start = high_resolution_clock::now();

  for (int i = 0; i < n; i++) {
    binarySearch(data, 0, data.size() - 1, target);
  }

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  return duration.count() / 1000000.0;
}

int main() {
  ifstream MyFile("dataset_sample_1000.csv");
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

  string bestCase = data[n / 2][0];
  double bestTime = measureSearchTime(data, bestCase, n);

  string avgCase = data[2][0];
  double avgTime = measureSearchTime(data, avgCase, n);

  string worstCase = data[n - 1][0];
  double worstTime = measureSearchTime(data, worstCase, n);

  ofstream outFile("binary_search_n.txt");
  outFile << "Binary Search Performance Analysis\n\n";

  outFile << "Number of searches performed for each case: " << n << "\n\n";
  outFile << fixed << setprecision(6);
  outFile << "Best case (middle element) time: " << bestTime << " seconds\n";
  outFile << "Average case (other element than first, last and middle) time: "
          << avgTime << " seconds\n";
  outFile << "Worst case (last element) time: " << worstTime << " seconds\n";
  outFile.close();

  cout << "Results have been written to binary_search_n.txt" << endl;

  return 0;
}
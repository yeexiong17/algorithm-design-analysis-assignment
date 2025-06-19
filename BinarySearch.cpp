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
    int mid = low + (high - low) / 2;

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
        double timeInSeconds = duration.count() / 1000000.0;
        totalTime += timeInSeconds;
    }
    
    double avgTime = totalTime / repetitions;
    return avgTime;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <input_csv_file>" << endl;
    return 1;
  }
  
  string inputFileName = argv[1];
  ifstream MyFile(inputFileName);
  
  if (!MyFile.is_open()) {
    cout << "Error: Could not open file " << inputFileName << endl;
    return 1;
  }
  
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
  const int repetitions = 1;

  string bestCase = data[n / 2][0];
  double bestTime = measureSearchTime(data, bestCase, n, repetitions);

  // Calculate average case using multiple positions
  vector<int> positions = {n/8, n/6, n/4, n/3, (2*n)/5, (3*n)/5, (2*n)/3, (3*n)/4};
  double totalAvgTime = 0.0;
  
  for (int pos : positions) {
    string avgCase = data[pos][0];
    totalAvgTime += measureSearchTime(data, avgCase, n, repetitions);
  }
  double avgTime = totalAvgTime / positions.size();

  string worstCase = data[n - 1][0];
  double worstTime = measureSearchTime(data, worstCase, n, repetitions);

  string outputFileName = "binary_search_" + to_string(n) + ".txt";
  ofstream outFile(outputFileName);
  outFile << "Binary Search Performance Analysis\n\n";

  outFile << "Number of searches performed for each case: " << n << "\n";
  outFile << "Number of measurement repetitions: " << repetitions << "\n\n";
  outFile << fixed << setprecision(9);
  outFile << "Best case (middle element) average time: " << bestTime << " seconds\n";
  outFile << "Average case (average of elements at 1/8, 1/6, 1/4, 1/3, 2/5, 3/5, 2/3, and 3/4 positions) average time: "
          << avgTime << " seconds\n";
  outFile << "Worst case (last element) average time: " << worstTime << " seconds\n";
  outFile.close();

  cout << "Results have been written to " << outputFileName << endl;

  return 0;
}
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int binarySearch(vector<vector<string>> data, int low, int high,
                 string target) {

  if (high >= low) {
    int mid = low + (high - low) / 2;

    if (data[mid][0] == target) {
      cout << mid << endl;
      return mid;
    }

    if (target < data[mid][0]) {
      return binarySearch(data, low, mid - 1, target);
    }

    return binarySearch(data, mid + 1, high, target);
  }

  return -1;
}

int main() {

  ifstream MyFile("dataset_sample_1000.csv");
  string line, word;

  vector<vector<string>> data;

  int i = 0;
  while (getline(MyFile, line)) {
    vector<string> row;

    row.clear();
    stringstream ss(line);

    while (getline(ss, word, ',')) {
      row.push_back(word);
    }
    data.push_back(row);
  }

  int result = binarySearch(data, 0, data.size() - 1, "35675435");

  cout << result << endl;

  return 0;
}
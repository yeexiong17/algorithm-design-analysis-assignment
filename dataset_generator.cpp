#include <iostream>
#include <fstream>
#include <unordered_set>
#include <random>
#include <string>

using  namespace std;
string generateRandomString(int minLeng, int maxLeng, minstd_rand &rng) {
    string chars = "abcdefghijklmnopqrstuvwxyz";
    uniform_int_distribution<int> lengDist(minLeng, maxLeng);
    uniform_int_distribution<int> charDist(0, static_cast<int>(chars.size() - 1));

    int len = lengDist(rng);
    string result;
    
    for (int i = 0; i < len; ++i) {
        result += chars[charDist(rng)];
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: ./dataset_generator <size> <output_filename.csv>\n";
        return 1;
    }

    
    int n;
    try {
        n = stoi(argv[1]);
        if (n <= 0) {
            throw invalid_argument("Invalid size");
        }
    } catch (...) {
        cerr << "Error: Dataset size must be a positive integer.\n";
        return 1;
    }
    
    string outputFile = argv[2];
    unordered_set<int> uniqueNumbers;

    const int maxValue = 1000000000;

    random_device rd;
    minstd_rand rng(rd());
    uniform_int_distribution<int> dist(0, maxValue);

    cout << "Generating " << n << " unique random integers...\n";
    while (uniqueNumbers.size() < n) {
        uniqueNumbers.insert(dist(rng));
    }

    ofstream file(outputFile);
    if (!file) {
        cerr << "Unable to create file: " << outputFile << "\n";
        return 1;
    }

    cout << "Saving data to: " << outputFile << "\n";
    for (const int& number : uniqueNumbers) {
        file << number << "," << generateRandomString(5, 10, rng) << "\n";
    }

    cout << "Done.\n";
    return 0;
}

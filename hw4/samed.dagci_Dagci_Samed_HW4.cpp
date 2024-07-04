// Huseyin Samed Dagci 30996

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <sstream>
#include "sorting.cpp"
#include "searching.cpp"

using namespace std;

struct PhonebookEntry {
    string name;
    string lastname;
    string telephone;
    string city;

    bool operator<(const PhonebookEntry& other) const {
        return name < other.name || (name == other.name && lastname < other.lastname);
    }

    bool operator==(const string& c) const {
        string s2 = name + " " + lastname;
        return s2.find(c) == 0;
    }

    bool operator>(const PhonebookEntry& other) const {
        return name > other.name || (name == other.name && lastname > other.lastname);
    }

    bool operator<=(const PhonebookEntry& other) const {
        return !(*this > other);
    }

    bool operator>=(const PhonebookEntry& other) const {
        return !(*this < other);
    }
};

vector<PhonebookEntry> readPhonebook(const string& filename);
vector<string> compareSpeedsSorting(vector<PhonebookEntry>& phonebook);
void compareSpeedSearching(vector<PhonebookEntry>& phonebook, string& query);
void ToUpper(string& s);
string UpperString(const string& s);


int main() {

    // handle the file input
    string filename;
    cout << "Please enter the contact file name: " << endl;
    cin >> filename;
    vector<PhonebookEntry> phonebook = readPhonebook(filename);        

    // handle the query input
    string query;
    cout << "Please enter the word to be queried: " << endl;
    cin.ignore();
    getline(cin, query);

    // print the timing values for sorting algorithms and save the speed ups in a vector
    vector<string> speedUps = compareSpeedsSorting(phonebook);
    
    // searching part starts here
    cout << "\nSearching for " << query << "\n";
    cout << "======================================\n";
    compareSpeedSearching(phonebook, query);
    
    // speed up for sorting
    cout << "SpeedUps between Sorting Algorithms\n";
    cout << "======================================\n";
    for (const string& speedUp : speedUps) {
        cout << speedUp + "\n";
    }

    return 0;
}

// a simple function to read files, turn each line into an entry, and push them into a vector
vector<PhonebookEntry> readPhonebook(const string& filename) {
    vector<PhonebookEntry> phonebook;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        exit(1);  // Return an empty vector to indicate failure
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        PhonebookEntry entry;
        iss >> entry.name >> entry.lastname >> entry.telephone >> entry.city;
        phonebook.push_back(entry);
    }
    file.close();
    return phonebook;
}

// this function compares all the sorting algorithms, print timing values and saves speedups
vector<string> compareSpeedsSorting(vector<PhonebookEntry>& phonebook) {

    
    // number of repetitions to test each algorithm
    int k = 5;

    // timing variables
    long long quickSortTotalTime = 0;
    long long insertionSortTotalTime = 0;
    long long mergeSortTotalTime = 0;
    long long heapSortTotalTime = 0;

    // measure quick sort time
    for (int i = 0; i < k; i++) {
        vector<PhonebookEntry> phonebookQuickSort = phonebook;
        auto start = chrono::high_resolution_clock::now();
        quicksort<PhonebookEntry>(phonebookQuickSort, 0, phonebookQuickSort.size() - 1);
        auto end = chrono::high_resolution_clock::now();
        quickSortTotalTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // measure insertion sort time
    for (int i = 0; i < k; i++) {
        vector<PhonebookEntry>phonebookInsertionSort = phonebook;
        auto start = chrono::high_resolution_clock::now();
        insertionSort<PhonebookEntry>(phonebookInsertionSort, 0, phonebookInsertionSort.size() - 1);
        auto end = chrono::high_resolution_clock::now();
        insertionSortTotalTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // measure merge sort time
    for (int i = 0; i < k; i++) {
        vector<PhonebookEntry>phonebookMergeSort = phonebook;
        auto start = chrono::high_resolution_clock::now();
        mergeSort<PhonebookEntry>(phonebookMergeSort, 0, phonebookMergeSort.size() - 1);
        auto end = chrono::high_resolution_clock::now();
        mergeSortTotalTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // measure heap sort time
    for (int i = 0; i < k; i++) {
        vector<PhonebookEntry>phonebookHeapSort = phonebook;
        auto start = chrono::high_resolution_clock::now();
        heapsort<PhonebookEntry>(phonebook);
        auto end = chrono::high_resolution_clock::now();
        heapSortTotalTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // calculate the average time values in k iteration
    long long quickSortTime = quickSortTotalTime / k;
    long long insertionSortTime = insertionSortTotalTime / k;
    long long mergeSortTime = mergeSortTotalTime / k;
    long long heapSortTime = heapSortTotalTime / k;


    // print the sorting speeds of each algorithm
    cout << "\nSorting the vector copies\n";
    cout << "======================================\n";
    cout << "Quick Sort Time: " << quickSortTime << " Nanoseconds\n";
    cout << "Insertion Sort Time: " << insertionSortTime << " Nanoseconds\n";
    cout << "Merge Sort Time: " << mergeSortTime << " Nanoseconds\n";
    cout << "Heap Sort Time: " << heapSortTime << " Nanoseconds\n";

    // save the speed up values for later
    vector<string> results;
    results.push_back("Insertion Sort / Quick Sort SpeedUp: " + to_string(static_cast<double>(insertionSortTime) / quickSortTime));
    results.push_back("Merge Sort / Quick Sort SpeedUp: " + to_string(static_cast<double>(mergeSortTime) / quickSortTime));
    results.push_back("Heap Sort / Quick Sort SpeedUp: " + to_string(static_cast<double>(heapSortTime) / quickSortTime));

    return results;
}

// this function compares search algorithms prints the results and the speedup values
void compareSpeedSearching(vector<PhonebookEntry>& phonebook, string& query) {

    int k = 1;
    
    vector<int> binarySearchResults;
    vector<int> sequentialSearchResults;

    long long binarySearchTotalTime = 0;
    long long sequentialSearchTotalTime = 0;
    // measure binary search time
    for (int i = 0; i < k; i++) {
        auto start = chrono::high_resolution_clock::now();
        binarySearchResults = binarySearchAll<PhonebookEntry>(phonebook, query, 0, phonebook.size() - 1);
        auto end = chrono::high_resolution_clock::now();
        binarySearchTotalTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // measure sequential search time
    for (int i = 0; i < k; i++) {
        auto start = chrono::high_resolution_clock::now();
        sequentialSearchResults = sequentialSearch<PhonebookEntry>(phonebook, query);
        auto end = chrono::high_resolution_clock::now();
        sequentialSearchTotalTime += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    long long binarySearchTime = binarySearchTotalTime / k;
    long long sequentialSearchTime = sequentialSearchTotalTime / k;

    // search results for binary search
    if (!binarySearchResults.empty()) {
        insertionSort<int>(binarySearchResults, 0, binarySearchResults.size() - 1);
        for (int idx : binarySearchResults) {
            cout << UpperString(phonebook[idx].name) << " " << UpperString(phonebook[idx].lastname) << " "
                << phonebook[idx].telephone << " " << phonebook[idx].city << "\n";
        }
    }
    else {
        cout << query << " does NOT exist in the dataset\n";
    }
    cout << "\nBinary Search Time: " << binarySearchTime << " Nanoseconds\n\n";

    // search results sequential search
    if (!sequentialSearchResults.empty()) {
        for (int idx : sequentialSearchResults) {
            cout << UpperString(phonebook[idx].name) << " " << UpperString(phonebook[idx].lastname) << " "
                << phonebook[idx].telephone << " " << phonebook[idx].city << "\n";
        }
    }
    else {
        cout << query << " does NOT exist in the dataset\n";
    }
    cout << "\nSequential Search Time: " << sequentialSearchTime << " Nanoseconds\n\n";

    // speed ups
    cout << "SpeedUp between Search Algorithms\n";
    cout << "======================================\n";
    cout << "(Sequential Search/ Binary Search) SpeedUp = " << static_cast<double>(sequentialSearchTime) / binarySearchTime << "\n\n";


}


void ToUpper(string& s)
// postcondition: s all lower case     
{
    int len = s.length();
    for (int k = 0; k < len; k++)
    {
        s[k] = toupper(s[k]);
    }
}

string UpperString(const string& s)
{
    string word = s;
    ToUpper(word);
    return word;
}
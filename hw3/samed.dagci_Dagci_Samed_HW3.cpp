// Huseyin Samed Dagci 30996

#include <iostream>
#include <string>
#include <vector>	
#include "AVL.cpp"
#include <fstream>
#include <sstream>
#include <map>
#include "hashT.cpp"
#include <cctype> 
#include <chrono> 
#include <iomanip> 
#include <sstream> 
#include <unordered_set> 




using namespace std;

// structs
struct DocumentItem {
    string documentName;
    int count;
};
struct WordItem {
    string word;
    vector<DocumentItem> documents;
};

// prototypes of the functions used in the program

template <class Key, class Value>
void preprocessFiles(vector<string> files, AVLSearchTree<Key, Value>& tree, HashTable<string, WordItem*>& hashTable);

template <class Key, class Value>
void searchEngine(vector<string> files, AVLSearchTree<Key, Value>& tree, HashTable<string, WordItem*>& hashTable);
 


// main function serves as an interface for the search engine
int main() {

    vector<string> fileNames;
    int num_of_files;           // get the number of files to process

    cout << "Enter number of input files: ";
    cin >> num_of_files;
    cin.ignore();
    

    for (int i = 1; i <= num_of_files; i++) {
        string file_name;
        cout << "Enter " << i << ". file name: ";
        getline(cin, file_name);
        fileNames.push_back(file_name);
    }

    AVLSearchTree<string, WordItem*> tree;
    HashTable<string, WordItem*> hashTable(11, 0.75);

    preprocessFiles(fileNames, tree, hashTable);
    
    hashTable.printStats();

    searchEngine(fileNames, tree, hashTable);


    return 0;
}


// function to preprocess files and update AVL Search Tree accordingly
template <class Key, class Value>
void preprocessFiles(vector<string> files,  AVLSearchTree<Key, Value>& tree, HashTable<string, WordItem*>& hashTable) {

    for (const auto& file_name : files) {

        ifstream file(file_name);                   // open the file
        if (!file.is_open()) {
            cout << "Error: Unable to open file " << file_name << endl;
            continue; // move to the next iteration if file cannot be opened
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string word;
            while (iss >> word) {                   // parse the words

                string processed_word;
                // Process the word to remove non-alphabetical characters and convert to lowercase
                for (char& c : word) {
                    if (isalpha(c)) {
                        processed_word += tolower(c); // convert to lowercase
                    }
                    else {
                        if (!processed_word.empty()) { // insert processed word if not empty
                            // insert word into AVL Search Tree
                            WordItem* wordItem = tree.search(processed_word); // check if word is already in tree
                            if (wordItem == nullptr) { // if word does not exist in the tree
                                WordItem* newWordItem = new WordItem{ processed_word, {{file_name, 1}} }; // insert new node
                                tree.insert(processed_word, newWordItem);
                                hashTable.insert(processed_word, newWordItem); // insert into hash table
                            }
                            else {
                                // word found in tree, update existing node
                                bool found = false;
                                for (auto& doc : wordItem->documents) {
                                    if (doc.documentName == file_name) {
                                        doc.count++;
                                        found = true;
                                        break;
                                    }
                                }
                                if (!found) { // modify new document's count
                                    wordItem->documents.push_back({ file_name, 1 });
                                }
                            }
                            processed_word.clear(); // clear processed word for next word
                        }
                    }
                }
                if (!processed_word.empty()) { // insert last processed word if not empty
                    // insert word into AVL Search Tree
                    WordItem* wordItem = tree.search(processed_word); // check if word is already in tree
                    if (wordItem == nullptr) { // if word does not exist in the tree
                        WordItem* newWordItem = new WordItem{ processed_word, {{file_name, 1}} }; // insert new node
                        tree.insert(processed_word, newWordItem);
                        hashTable.insert(processed_word, newWordItem); // insert into hash table
                    }
                    else {
                        // word found in tree, update existing node
                        bool found = false;
                        for (auto& doc : wordItem->documents) {
                            if (doc.documentName == file_name) {
                                doc.count++;
                                found = true;
                                break;
                            }
                        }
                        if (!found) { // modify new document's count
                            wordItem->documents.push_back({ file_name, 1 });
                        }
                    }
                }
            }
        }

        file.close();

    }
    cout << endl;
}





template <class Key, class Value>
void searchEngine(vector<string> files, AVLSearchTree<Key, Value>& tree, HashTable<string, WordItem*>& hashTable) {
    // get query from the user
    cout << "Enter queried words in one line: ";
    string query;
    getline(cin, query);

    // parse the query into individual words
    vector<string> words;
    string word;
    for (char& c : query) {
        if (isalpha(c)) {
            word += tolower(c); // convert to lowercase
        }
        else {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    unordered_set<string> commonDocuments;      // tracks documents containing all queried words for tree
    // perform query with the search tree
    int k = 50;     // how many repetition
    auto start = chrono::high_resolution_clock::now();
    for (int t = 0; t < k; t++) {
        // Process each word in the query
        for (const auto& queriedWord : words) {
            WordItem* wordItem = tree.search(queriedWord);
            if (wordItem != nullptr) {
                unordered_set<string> currentDocuments;
                for (const auto& doc : wordItem->documents) {
                    currentDocuments.insert(doc.documentName);
                }
                if (commonDocuments.empty()) {
                    commonDocuments = currentDocuments;
                }
                else {
                    // Find the intersection of commonDocuments and currentDocuments
                    unordered_set<string> intersection;
                    for (const auto& doc : commonDocuments) {
                        if (currentDocuments.find(doc) != currentDocuments.end()) {
                            intersection.insert(doc);
                        }
                    }
                    commonDocuments = intersection;
                }
            }
            else {
                // if any word is not found, clear the commonDocuments set
                commonDocuments.clear();
                break;
            }
        }

    }
    // query ended stop the clock
    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::high_resolution_clock::now() -
            start);
    // print the document names and word counts for each queried word
    if (!commonDocuments.empty()) {
        for (const auto& doc : commonDocuments) {
            cout << "in Document " << doc;
            for (const auto& queriedWord : words) {
                WordItem* wordItem = tree.search(queriedWord);
                for (const auto& item : wordItem->documents) {
                    if (item.documentName == doc) {
                        cout << ", " << queriedWord << " found " << item.count << " times";
                    }
                }
            }
            cout << endl;
        }
    }
    else {
        cout << "No document contains the given query" << endl;
    }

    unordered_set<string> commonDocumentsHT; // tracks documents containing all queried words
    // perform query with hash table
    start = chrono::high_resolution_clock::now();
    for (int z = 0; z < k; z++) {

        // process each word in the query
        for (const auto& queriedWord : words) {
            WordItem* wordItem = hashTable.get(queriedWord);
            if (wordItem != nullptr) {
                unordered_set<string> currentDocuments;
                for (const auto& doc : wordItem->documents) {
                    currentDocuments.insert(doc.documentName);
                }
                if (commonDocumentsHT.empty()) {
                    commonDocumentsHT = currentDocuments;
                }
                else {
                    // find the intersection of commonDocuments and currentDocuments
                    unordered_set<string> intersection;
                    for (const auto& doc : commonDocumentsHT) {
                        if (currentDocuments.find(doc) != currentDocuments.end()) {
                            intersection.insert(doc);
                        }
                    }
                    commonDocumentsHT = intersection;
                }
            }
            else {
                // if any word is not found, clear the commonDocumentsHT set
                commonDocumentsHT.clear();
                break;
            }
        }

    }
    // query ended stop the clock
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::high_resolution_clock::now() - start);
    // print the document names and word counts for each queried word
    if (!commonDocumentsHT.empty()) {
        for (const auto& doc : commonDocumentsHT) {
            cout << "in Document " << doc;
            for (const auto& queriedWord : words) {
                WordItem* wordItem = hashTable.get(queriedWord);
                for (const auto& item : wordItem->documents) {
                    if (item.documentName == doc) {
                        cout << ", " << queriedWord << " found " << item.count << " times";
                    }
                }
            }
            cout << endl;
        }
    }
    else {
        cout << "No document contains the given query" << endl;
    }


    // timing part 
    cout << "\nTime: " << BSTTime.count() / k << endl;
    cout << "Time: " << HTTime.count() / k << endl;

    // Calculate speedup ratio
    double speedupRatio = static_cast<double>(BSTTime.count()) / HTTime.count();
    cout << "Speed Up: " << fixed << setprecision(5) << speedupRatio << endl;
}

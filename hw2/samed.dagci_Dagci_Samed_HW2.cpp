// Huseyin Samed Dagci 30996

#include <iostream>
#include <string>
#include <vector>	
#include "AVL.cpp"
#include <fstream>
#include <sstream>
#include <map>


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
string toLowerCase(const string& word);

template <class Key, class Value>
void preprocessFiles(AVLSearchTree<Key, Value>& tree);

template <class Key, class Value>
void searchTree(AVLSearchTree<Key, Value>& tree);


// main function serves as an interface for the search engine
int main() {

    AVLSearchTree<string, WordItem*> tree;

    preprocessFiles(tree);
    searchTree(tree);

    return 0;
}


// function to preprocess files and update AVL Search Tree accordingly
template <class Key, class Value>
void preprocessFiles(AVLSearchTree<Key, Value>& tree) {
    
    int num_of_files;           // get the number of files to process

    cout << "Enter number of input files: ";
    cin >> num_of_files;
    cin.ignore();

    for (int i = 1; i <= num_of_files; i++) {               
        string file_name;
        cout << "Enter " << i << ". file name: ";
        getline(cin, file_name);                   

        ifstream file(file_name);                   // open the file
        if (!file.is_open()) {
            cout << "Error: Unable to open file " << file_name << endl;
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string word;
            while (iss >> word) {                   // parse the words

                word = toLowerCase(word);                               // convert word to lowercase
                // Remove non-alphabetic characters
                //word.erase(remove_if(word.begin(), word.end(), [](unsigned char c) { return !isalpha(c); }), word.end());

                // insert word into AVL Search Tree
                if (!word.empty()) {            // if word is not erased
                    WordItem* wordItem = tree.search(word);                 // to check if word is already exist in the tree
                    if (wordItem == nullptr) {              // if word not exist in the tree
                        WordItem* newWordItem = new WordItem{ word, {{file_name, 1}} };             // insert new node
                        tree.insert(word, newWordItem);
                    }
                    else {
                        // word found in tree update existing node
                        bool found = false;
                        for (auto& doc : wordItem->documents) {
                            if (doc.documentName == file_name) {
                                doc.count++;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {           // modify new document's count
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


// function to query the tree
// function to query the tree
template <class Key, class Value>
void searchTree(AVLSearchTree<Key, Value>& tree) {

    string line;
    while (true) {
        cout << "Enter queried words in one line: ";                // get the input for the query
        getline(cin, line);
        if (line.substr(0, 6) == "REMOVE") {                        // if consist REMOVE remove the word from the tree
            string wordToRemove = toLowerCase(line.substr(7));
            tree.remove(wordToRemove);
            cout << wordToRemove << " has been REMOVED" << endl << endl;
        }
        else if (line == "ENDOFINPUT") {            // ENDOFINPUT prompted, terminate
            break;
        }
        else {
            istringstream iss(line);
            string word;
            vector<string> words;
            while (iss >> word) {                   // parse the queried words
                words.push_back(toLowerCase(word));
            }

            // map to store word occurrences for each document
            map<string, map<string, int>> wordOccurrences;  // to store document name, the word and its count

            bool found = false;
            for (const auto& word : words) {
                WordItem* wordItem = tree.search(word);             // search for the word
                if (wordItem != nullptr) {
                    found = true;
                    for (const auto& doc : wordItem->documents) {
                        wordOccurrences[doc.documentName][word] += doc.count;               // update the count
                    }
                }
            }

            if (!found) {
                cout << "No document contains the given query" << endl << endl;                    // not found in the tree
            }
            else {
                // for each document print each queried word's count 
                for (const auto& document : wordOccurrences) {
                    cout << "in Document " << document.first << ", ";
                    bool isFirstWord = true;
                    for (const auto& wordCount : words) {
                        if (document.second.find(wordCount) != document.second.end()) {
                            if (!isFirstWord) {
                                cout << ", "; // Print comma between words if it's not the first word
                            }
                            cout << wordCount << " found " << document.second.at(wordCount) << " times";
                            isFirstWord = false;
                        }
                    }
                    cout << "." << endl; // End the line with a dot
                }
                cout << endl;
            }
        }
    }
}




string toLowerCase(const string& word) {
    string result;
    for (char c : word) {
        if (isalpha(static_cast<unsigned char>(c))) {
            result += static_cast<char>(tolower(static_cast<unsigned char>(c)));
        }
    }
    return result;
}
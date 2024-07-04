#include <iostream>
#include <vector>
#include <list>
#include <functional>


// HashTable class
template<typename KeyType, typename ValueType>
class HashTable {
private:
    // Entry struct to hold key-value pairs
    struct Entry {
        KeyType key;
        ValueType value;
    };

    std::vector<std::list<Entry>> buckets; // Vector of linked lists
    size_t itemCount;
    size_t capacity;
    double loadFactorThreshold;

    // Hash function to calculate the index of a key
    size_t hashFunction(const KeyType& key) const {
        // Custom hash function implementation for WordItem structs
        size_t hash = 0;
        for (char c : key) {
            hash = (hash * 31) + c; // Use a prime number for multiplication
        }
        return hash % capacity;
    }


    // Rehash the hash table when load factor exceeds the threshold
    void rehash() {
        size_t prevCapacity = capacity;
        double prevLoad = loadRatio();

        std::vector<std::list<Entry>> prevBuckets = std::move(buckets);

        capacity *= 2;
        buckets.clear();
        buckets.resize(capacity);
        itemCount = 0;

        // Reinsert all entries into the new buckets
        for (const auto& bucket : prevBuckets) {
            for (const auto& entry : bucket) {
                insert(entry.key, entry.value);
            }
        }

        // Print internal information of HashTable after rehashing
        std::cout << "rehashed..." << std::endl;
        std::cout << "previous table size: " << prevCapacity << ", new table size: " << capacity << ", ";
        std::cout << "current unique word count: " << uniqueWordCount() << ", ";
        std::cout << "previous load factor: " << prevLoad << ", ";
        std::cout << "current load factor: " << loadRatio() << std::endl;
    }

public:
    // Constructor
    HashTable(size_t initialCapacity, double loadFactor) : capacity(initialCapacity), loadFactorThreshold(loadFactor), itemCount(0) {
        buckets.resize(capacity);
    }

    // Insert a key-value pair into the hash table
    void insert(const KeyType& key, const ValueType& value) {
        size_t index = hashFunction(key);
        buckets[index].push_back({ key, value });
        itemCount++;

        double loadFactor = static_cast<double>(itemCount) / capacity;
        if (loadFactor > loadFactorThreshold) {
            rehash();
        }
    }

    // Calculate the load ratio of the hash table
    double loadRatio() const {
        return static_cast<double>(itemCount) / capacity;
    }

    // Retrieve the value associated with a key
    ValueType get(const KeyType& key) const {
        size_t index = hashFunction(key);
        for (const auto& entry : buckets[index]) {
            if (entry.key == key) {
                return entry.value;
            }
        }
        // Key not found, return a default value
        return ValueType();
    }

    // Remove a key-value pair from the hash table
    void remove(const KeyType& key) {
        size_t index = hashFunction(key);
        auto& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                itemCount--;
                break;
            }
        }
    }

    int uniqueWordCount() const {
        int count = 0;
        for (const auto& bucket : buckets) {
            count += bucket.size();
        }
        return count;
    }

    void printStats() {
        std::cout << "After preprocessing, the unique word count is " << uniqueWordCount() << ". Current load ratio is " << loadRatio() << "." << std::endl;
    }


};


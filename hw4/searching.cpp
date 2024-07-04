#include <iostream>
#include <vector>

using namespace std;


template <typename T>
vector<int> sequentialSearch(const vector<T>& arr, const string& query) {
    vector<int> results;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == query) {
            results.push_back(i);
        }
    }
    return results;
}

template <typename T>
vector<int> binarySearchAll(const vector<T>& arr, const string& query, int low, int high) {
    vector<int> results;
    int mid = -1;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (arr[mid] == query) {
            // Found one match, now find all matches around it
            break;
        }
        if ((arr[mid].name + " " + arr[mid].lastname) > query) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    if (mid != -1 && arr[mid] == query) {
        // Find all matches around mid
        int left = mid;
        while (left >= low && arr[left] == query) {
            results.push_back(left);
            left--;
        }
        int right = mid + 1;
        while (right <= high && arr[right] == query) {
            results.push_back(right);
            right++;
        }
    }
    return results;
}
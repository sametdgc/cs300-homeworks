#include <vector>
using namespace std;

inline int leftChild(int i) {
    return 2 * i + 1;
}

template <class Comparable>
void heapsort(vector<Comparable>& a) {
    for (int i = a.size() / 2; i >= 0; i--)
        percDown(a, i, a.size());
    for (int j = a.size() - 1; j > 0; j--) {
        swap(a[0], a[j]);
        percDown(a, 0, j);
    }
}

template <class Comparable>
void percDown(vector<Comparable>& a, int i, int n) {
    int child;
    Comparable tmp;

    for (tmp = a[i]; leftChild(i) < n; i = child) {
        child = leftChild(i);
        if (child != n - 1 && a[child] < a[child + 1])
            child++;
        if (a[child] > tmp)
            a[i] = a[child];
        else
            break;
    }
    a[i] = tmp;
}

template <class Comparable>
void insertionSort(vector<Comparable>& a, int left, int right) {
    for (int p = left + 1; p <= right; p++) {
        Comparable tmp = a[p];
        int j;

        for (j = p; j > left && tmp < a[j - 1]; j--)
            a[j] = a[j - 1];
        a[j] = tmp;
    }
}

template <class Comparable>
inline void swapME(Comparable& obj1, Comparable& obj2) {
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}

template <class Comparable>
const Comparable& median3(vector<Comparable>& a, int left, int right) {
    int center = (left + right) / 2;
    if (a[center] < a[left])
        swap(a[left], a[center]);
    if (a[right] < a[left])
        swap(a[left], a[right]);
    if (a[right] < a[center])
        swap(a[center], a[right]);

    swap(a[center], a[right - 1]);
    return a[right - 1];
}

template <class Comparable>
void quicksort(vector<Comparable>& a, int left, int right) {
    if (left + 10 <= right) {
        Comparable pivot = median3(a, left, right);
        int i = left, j = right - 1;
        for (;;) {
            while (a[++i] < pivot) {}
            while (pivot < a[--j]) {}
            if (i < j)
                swapME(a[i], a[j]);
            else
                break;
        }
        swapME(a[i], a[right - 1]);
        quicksort(a, left, i - 1);
        quicksort(a, i + 1, right);
    }
    else {
        insertionSort(a, left, right);
    }
}

template <class Comparable>
void merge(vector<Comparable>& a, int left, int center, int right) {
    int start = center + 1;
    if (a[center] <= a[start])
        return;

    while (left <= center && start <= right) {
        if (a[left] <= a[start]) {
            left++;
        }
        else {
            Comparable info = a[start];
            int idx = start;
            while (idx != left) {
                a[idx] = a[idx - 1];
                idx--;
            }
            a[left] = info;
            left++;
            center++;
            start++;
        }
    }
}

template <class Comparable>
void mergeSort(vector<Comparable>& a, int left, int right) {
    if (left < right) {
        int center = (left + right) / 2;
        mergeSort(a, left, center);
        mergeSort(a, center + 1, right);
        merge(a, left, center, right);
    }
};
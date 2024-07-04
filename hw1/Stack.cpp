#include <iostream>

template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& newData, Node* newNext) : data(newData), next(newNext) {}
    };

    Node* topNode;

public:
    Stack() : topNode(nullptr) {}
    ~Stack();
    void push(const T& newData);
    void pop();
    T& top();
    bool isEmpty() const;
};

template<typename T>
Stack<T>::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

template<typename T>
void Stack<T>::push(const T& newData) {
    topNode = new Node(newData, topNode);
}

template<typename T>
void Stack<T>::pop() {
    if (!isEmpty()) {
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }
}

template<typename T>
T& Stack<T>::top() {
    if (!isEmpty()) {
        return topNode->data;
    }
    
}

template<typename T>
bool Stack<T>::isEmpty() const {
    return topNode == nullptr;
}


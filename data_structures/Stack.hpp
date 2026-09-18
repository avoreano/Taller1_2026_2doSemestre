#ifndef STACK_HPP
#define STACK_HPP
#include "Node.hpp"

template <typename T>
class Stack {
private:
    Node<T>* topNode;
    int count;

public:
    Stack() {
        this->topNode = nullptr;
        this->count = 0;
    }

    ~Stack() {
        clear();
    }

    void clear() {
        Node<T>* current = topNode;
        while (current != nullptr) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
        topNode = nullptr;
        count = 0;
    }

    void push(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    bool pop(T& outValue) {
        if (topNode == nullptr) {
            return false;
        }
        Node<T>* temp = topNode;
        outValue = temp->data;
        topNode = topNode->next;
        delete temp;
        count--;
        return true;
    }

    bool peek(T& outValue) const {
        if (topNode == nullptr) {
            return false;
        }
        outValue = topNode->data;
        return true;
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }

    int getSize() const {
        return count;
    }

    Node<T>* getTop() const {
        return topNode;
    }
};

#endif

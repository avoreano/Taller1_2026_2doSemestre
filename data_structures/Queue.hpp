#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "Node.hpp"

template <typename T>
class Queue {
private:
    Node<T>* frontNode;
    Node<T>* backNode;
    int count;

public:
    Queue() {
        this->frontNode = nullptr;
        this->backNode = nullptr;
        this->count = 0;
    }

    ~Queue() {
        clear();
    }

    void clear() {
        Node<T>* current = frontNode;
        while (current != nullptr) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
        frontNode = nullptr;
        backNode = nullptr;
        count = 0;
    }

    void enqueue(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (backNode == nullptr) {
            frontNode = newNode;
            backNode = newNode;
        } else {
            backNode->next = newNode;
            backNode = newNode;
        }
        count++;
    }

    bool dequeue(T& outValue) {
        if (frontNode == nullptr) {
            return false;
        }
        Node<T>* temp = frontNode;
        outValue = temp->data;
        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            backNode = nullptr;
        }
        delete temp;
        count--;
        return true;
    }

    bool isEmpty() const {
        return frontNode == nullptr;
    }

    int getSize() const {
        return count;
    }

    Node<T>* getFront() const {
        return frontNode;
    }

    template <typename Check>
    bool exists(Check checker) const {
        Node<T>* current = frontNode;
        while (current != nullptr) {
            if (checker(current->data)) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

#endif

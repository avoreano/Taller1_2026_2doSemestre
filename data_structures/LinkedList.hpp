#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP
#include "Node.hpp"

template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int count;

public:
    LinkedList() {
        this->head = nullptr;
        this->tail = nullptr;
        this->count = 0;
    }

    ~LinkedList() {
        clear();
    }

    void clear() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
    }
    
    void pushBack(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    void pushFront(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) {
            tail = newNode;
        }
        count++;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    int getSize() const {
        return count;
    }


    Node<T>* getHead() const {
        return head;
    }

    template <typename Check>
    bool exists(Check checker) const {
        Node<T>* current = head;
        while (current != nullptr) {
            if (checker(current->data)) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    template <typename Func>
    void forEach(Func function) const {
        Node<T>* current = head;
        while (current != nullptr) {
            function(current->data);
            current = current->next;
        }
    }

};

#endif

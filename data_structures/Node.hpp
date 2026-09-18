template <typename T>
struct Node {
    T data;
    Node<T>* next;

    Node(const T& value) {
        this->data = value;
        this->next = nullptr;
    }
};
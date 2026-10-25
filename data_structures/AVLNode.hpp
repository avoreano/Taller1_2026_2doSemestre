#ifndef AVL_NODE_HPP
#define AVL_NODE_HPP

template <typename T>
struct AVLNode {
    int key;
    T data;
    AVLNode<T>* left;
    AVLNode<T>* right;
    int height;

    AVLNode(int key, const T& value) {
        this->key = key;
        this->data = value;
        this->left = nullptr;
        this->right = nullptr;
        this->height = 1;
    }
};

#endif
#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "AVLNode.hpp"

template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;
    int count;

    int heightOf(AVLNode<T>* node) const {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int maxOf(int a, int b) const {
        if(a>b) {return a;}
        else {return b;}
    }

    void updateHeight(AVLNode<T>* node) {
        node->height = 1 + maxOf(heightOf(node->left), heightOf(node->right));
    }

    AVLNode<T>* insertNode(AVLNode<T>* node, int key, const T& value, bool& inserted) {
        if (node == nullptr) {
            inserted = true;
            return new AVLNode<T>(key, value);
        }
        // se levanta una flag para saber si hay duplicados

        if (key < node->key) {
            node->left = insertNode(node->left, key, value, inserted);
        } else if (key > node->key) {
            node->right = insertNode(node->right, key, value, inserted);
        } else {
            inserted = false; // aca no se inserta el duplicado
            return node;
        }

        updateHeight(node);
        return node;
    }

    void destroy(AVLNode<T>* node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    template <typename Func>
    void inOrder(AVLNode<T>* node, Func function) const {
        if (node == nullptr) {
            return;
        }
        inOrder(node->left, function);
        function(node->key, node->data);
        inOrder(node->right, function);
    }

    template <typename Func>
    void preOrder(AVLNode<T>* node, Func function) const {
        if (node == nullptr) {
            return;
        }
        function(node->key, node->data);
        preOrder(node->left, function);
        preOrder(node->right, function);
    }

    template <typename Func>
    void postOrder(AVLNode<T>* node, Func function) const {
        if (node == nullptr) {
            return;
        }
        postOrder(node->left, function);
        postOrder(node->right, function);
        function(node->key, node->data);
    }

public:
    AVLTree() {
        this->root = nullptr;
        this->count = 0;
    }

    ~AVLTree() {
        clear();
    }

    void clear() {
        destroy(root);
        root = nullptr;
        count = 0;
    }

    // retorna false si el id ya existe
    bool insert(int key, const T& value) {
        bool inserted = false;
        root = insertNode(root, key, value, inserted);
        if (inserted) {
            count++;
        }
        return inserted;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    int getSize() const {
        return count;
    }

    int getHeight() const {
        return heightOf(root);
    }

    AVLNode<T>* getRoot() const {
        return root;
    }

    template <typename Func>
    void inOrder(Func function) const {
        inOrder(root, function);
    }

    template <typename Func>
    void preOrder(Func function) const {
        preOrder(root, function);
    }

    template <typename Func>
    void postOrder(Func function) const {
        postOrder(root, function);
    }
};

#endif
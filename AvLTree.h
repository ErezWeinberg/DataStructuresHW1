#ifndef AVLTREE_H
#define AVLTREE_H

#include <functional>
#include <algorithm>

template <typename T, typename Compare = std::less<T>>
class AVLTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height;
        
        Node(const T& data) : data(data), left(nullptr), right(nullptr), height(1) {}
    };
    
    Node* root;
    Compare comp;
    int size;
    
    // Helper methods
    void clear(Node* node);
    int getHeight(Node* node);
    int getBalance(Node* node);
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* insertHelper(Node* node, const T& data, bool& inserted);
    Node* removeHelper(Node* node, const T& data, bool& removed);
    Node* findMin(Node* node);
    T* findHelper(Node* node, const T& data) const;
    T* findClosestHelper(Node* node, const T& data, T* closest) const;

public:
    class Iterator {
    private:
        Node* current;
        // Add stack or other mechanism for tree traversal if needed
    public:
        Iterator(Node* node = nullptr) : current(node) {}

        T& operator*() { return current->data; }
        Iterator& operator++() {
            // Implement in-order traversal
            return *this;
        }
        bool operator!=(const Iterator& other) { return current != other.current; }
        bool operator==(const Iterator& other) { return current == other.current; }
    };

    AVLTree() : root(nullptr), size(0) {}
    ~AVLTree();

    bool insert(const T& data);
    bool remove(const T& data);
    T* find(const T& data) const;
    T* findClosest(const T& data) const;
    Iterator begin();
    Iterator end();
    bool contains(const T& data) const;
    bool isEmpty() const;
    int getSize() const;
};

// Template implementation (must be in header file)

template <typename T, typename Compare>
AVLTree<T, Compare>::~AVLTree() {
    clear(root);
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::clear(Node* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T, typename Compare>
int AVLTree<T, Compare>::getHeight(Node* node) {
    return node ? node->height : 0;
}

template <typename T, typename Compare>
int AVLTree<T, Compare>::getBalance(Node* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

template <typename T, typename Compare>
bool AVLTree<T, Compare>::insert(const T& data) {
    bool inserted = false;
    root = insertHelper(root, data, inserted);
    if (inserted) size++;
    return inserted;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::insertHelper(Node* node, const T& data, bool& inserted) {
    // Standard BST insertion
    if (!node) {
        inserted = true;
        return new Node(data);
    }

    if (comp(data, node->data)) {
        node->left = insertHelper(node->left, data, inserted);
    } else if (comp(node->data, data)) {
        node->right = insertHelper(node->right, data, inserted);
    } else {
        // Duplicate key
        inserted = false;
        return node;
    }

    // Update height
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // Get balance factor
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && comp(data, node->left->data)) {
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && comp(node->right->data, data)) {
        return rotateLeft(node);
    }

    // Left Right Case
    if (balance > 1 && comp(node->left->data, data)) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && comp(data, node->right->data)) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

template <typename T, typename Compare>
bool AVLTree<T, Compare>::remove(const T& data) {
    bool removed = false;
    root = removeHelper(root, data, removed);
    if (removed) size--;
    return removed;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::removeHelper(Node* node, const T& data, bool& removed) {
    if (!node) {
        removed = false;
        return node;
    }

    if (comp(data, node->data)) {
        node->left = removeHelper(node->left, data, removed);
    } else if (comp(node->data, data)) {
        node->right = removeHelper(node->right, data, removed);
    } else {
        removed = true;

        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            Node* temp = findMin(node->right);
            node->data = temp->data;
            node->right = removeHelper(node->right, temp->data, removed);
        }
    }

    if (!node) return node;

    // Update height
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // Get balance factor
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }

    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }

    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Node* AVLTree<T, Compare>::findMin(Node* node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

template <typename T, typename Compare>
T* AVLTree<T, Compare>::find(const T& data) const {
    return findHelper(root, data);
}

template <typename T, typename Compare>
T* AVLTree<T, Compare>::findHelper(Node* node, const T& data) const {
    if (!node) return nullptr;

    if (comp(data, node->data)) {
        return findHelper(node->left, data);
    } else if (comp(node->data, data)) {
        return findHelper(node->right, data);
    } else {
        return &(node->data);
    }
}

template <typename T, typename Compare>
T* AVLTree<T, Compare>::findClosest(const T& data) const {
    T* closest = nullptr;
    return findClosestHelper(root, data, closest);
}

template <typename T, typename Compare>
T* AVLTree<T, Compare>::findClosestHelper(Node* node, const T& data, T* closest) const {
    if (!node) return closest;

    if (!closest) closest = &(node->data);

    // Update closest if current node is closer
    // This is a simplified version - you may need to customize based on your comparison logic
    closest = &(node->data);

    if (comp(data, node->data)) {
        return findClosestHelper(node->left, data, closest);
    } else if (comp(node->data, data)) {
        return findClosestHelper(node->right, data, closest);
    } else {
        return &(node->data);
    }
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Iterator AVLTree<T, Compare>::begin() {
    Node* leftmost = root;
    while (leftmost && leftmost->left) {
        leftmost = leftmost->left;
    }
    return Iterator(leftmost);
}

template <typename T, typename Compare>
typename AVLTree<T, Compare>::Iterator AVLTree<T, Compare>::end() {
    return Iterator(nullptr);
}

template <typename T, typename Compare>
bool AVLTree<T, Compare>::contains(const T& data) const {
    return find(data) != nullptr;
}

template <typename T, typename Compare>
bool AVLTree<T, Compare>::isEmpty() const {
    return root == nullptr;
}

template <typename T, typename Compare>
int AVLTree<T, Compare>::getSize() const {
    return size;
}

#endif // AVLTREE_H
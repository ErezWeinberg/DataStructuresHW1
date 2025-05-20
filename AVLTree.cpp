// AvLTree.h
#ifndef AVLTREE_H
#define AVLTREE_H // Prevents multiple inclusions of this header file

#include <algorithm>
#include <functional>
#include <stack>

template<class T, class Compare = std::less<T>>
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
    int size;
    Compare comp;
    
    // All private methods remain the same

public:
    // Iterator class definition
    class Iterator {
    private:
        Node* current;
        std::stack<Node*> stk;

        void pushLeft(Node* node) {
            while (node) {
                stk.push(node);
                node = node->left;
            }
        }

    public:
        Iterator(Node* root) {
            pushLeft(root);
            if (!stk.empty()) {
                current = stk.top();
            } else {
                current = nullptr;
            }
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        T& operator*() const {
            return current->data;
        }

        Iterator& operator++() {
            if (!stk.empty()) {
                Node* node = stk.top();
                stk.pop();
                pushLeft(node->right);
                current = stk.empty() ? nullptr : stk.top();
            }
            return *this;
        }
    };

    Iterator begin() {
        return Iterator(root);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    AVLTree() : root(nullptr), size(0) {}

    ~AVLTree() {
        clear();
    }

    // Add all the other public methods that were in your original code
    // (insert, remove, contains, find, findClosest, clear, getSize, isEmpty, inOrder, copy constructor, operator=)

    // הוספת איבר
    bool insert(const T& data) {
        int oldSize = size;
        root = insertRec(root, data);
        return size > oldSize;
    }

    // הסרת איבר
    bool remove(const T& data) {
        int oldSize = size;
        root = removeRec(root, data);
        return size < oldSize;
    }

    // חיפוש איבר
    bool contains(const T& data) const {
        return findRec(root, data) != nullptr;
    }

    // החזרת הנתונים של איבר
    T* find(const T& data) const {
        Node* node = findRec(root, data);
        return node ? &(node->data) : nullptr;
    }

    // מציאת האיבר הקטן ביותר שגדול או שווה ל-data
    T* findClosest(const T& data) const {
        Node* node = findClosestRec(root, data, nullptr);
        return node ? &(node->data) : nullptr;
    }

    // ניקוי העץ
    void clear() {
        clearRec(root);
        root = nullptr;
        size = 0;
    }

    // החזרת גודל העץ
    int getSize() const {
        return size;
    }

    // בדיקה אם העץ ריק
    bool isEmpty() const {
        return size == 0;
    }

    // מעבר על כל איברי העץ בסדר עולה
    template<class Func>
    void inOrder(Func func) const {
        inOrderRec(root, func);
    }

    // העתקת העץ
    AVLTree(const AVLTree& other) : root(nullptr), size(0) {
        root = copyRec(other.root);
        size = other.size;
    }

    // השמה
    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            clear();
            root = copyRec(other.root);
            size = other.size;
        }
        return *this;
    }
    ~AVLTree() {
        clear();
    }
};  

#endif // AVLT
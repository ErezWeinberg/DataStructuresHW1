// AvLTree.h
#ifndef AVLTREE_H
#define AVLTREE_H // Prevents multiple inclusions of this header file

#include <algorithm>
#include <functional>

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
    
    // החזרת גובה של צומת (0 אם nullptr)
    int height(Node* node) const {
        return node ? node->height : 0;
    }
    
    // עדכון גובה של צומת
    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + std::max(height(node->left), height(node->right));
        }
    }
    
    // החזרת פקטור האיזון של צומת
    int balanceFactor(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }
    
    // רוטציה ימינה
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        updateHeight(y);
        updateHeight(x);
        
        return x;
    }
    
    // רוטציה שמאלה
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        updateHeight(x);
        updateHeight(y);
        
        return y;
    }
    
    // הוספה רקורסיבית
    Node* insertRec(Node* node, const T& data) {
        if (!node) {
            size++;
            return new Node(data);
        }
        
        if (comp(data, node->data)) {
            node->left = insertRec(node->left, data);
        } else if (comp(node->data, data)) {
            node->right = insertRec(node->right, data);
        } else {
            // המפתח כבר קיים
            return node;
        }
        
        updateHeight(node);
        
        int balance = balanceFactor(node);
        
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
    
    // מציאת הצומת המינימלי
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }
    
    // הסרה רקורסיבית
    Node* removeRec(Node* node, const T& data) {
        if (!node) {
            return nullptr;
        }
        
        if (comp(data, node->data)) {
            node->left = removeRec(node->left, data);
        } else if (comp(node->data, data)) {
            node->right = removeRec(node->right, data);
        } else {
            // מצאנו את הצומת למחיקה
            
            // צומת עם ילד אחד או ללא ילדים
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                
                // ללא ילדים
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    // ילד אחד
                    *node = *temp;
                }
                
                delete temp;
                size--;
            } else {
                // צומת עם שני ילדים
                Node* temp = minValueNode(node->right);
                
                node->data = temp->data;
                node->right = removeRec(node->right, temp->data);
            }
        }
        
        if (!node) {
            return nullptr;
        }
        
        updateHeight(node);
        
        int balance = balanceFactor(node);
        
        // Left Left Case
        if (balance > 1 && balanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }
        
        // Left Right Case
        if (balance > 1 && balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Right Right Case
        if (balance < -1 && balanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }
        
        // Right Left Case
        if (balance < -1 && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    // חיפוש רקורסיבי
    Node* findRec(Node* node, const T& data) const {
        if (!node) {
            return nullptr;
        }
        
        if (comp(data, node->data)) {
            return findRec(node->left, data);
        } else if (comp(node->data, data)) {
            return findRec(node->right, data);
        } else {
            return node;
        }
    }
    
    // חיפוש הקטן ביותר שגדול או שווה ל-data
    Node* findClosestRec(Node* node, const T& data, Node* closest) const {
        if (!node) {
            return closest;
        }
        
        if (comp(data, node->data)) {
            // הנוכחי גדול מ-data, יכול להיות מועמד
            if (!closest || comp(node->data, closest->data)) {
                closest = node;
            }
            return findClosestRec(node->left, data, closest);
        } else if (comp(node->data, data)) {
            // הנוכחי קטן מ-data, נמשיך בחיפוש
            return findClosestRec(node->right, data, closest);
        } else {
            // מצאנו התאמה מדויקת
            return node;
        }
    }
    
    // שחרור הזיכרון
    void clearRec(Node* node) {
        if (node) {
            clearRec(node->left);
            clearRec(node->right);
            delete node;
        }
    }
    
    // העתקה עמוקה של עץ
    Node* copyRec(Node* node) {
        if (!node) {
            return nullptr;
        }
        
        Node* newNode = new Node(node->data);
        newNode->height = node->height;
        newNode->left = copyRec(node->left);
        newNode->right = copyRec(node->right);
        
        return newNode;
    }
    
    // מעבר inorder על העץ עם פונקציית callback
    template<class Func>
    void inOrderRec(Node* node, Func func) const {
        if (node) {
            inOrderRec(node->left, func);
            func(node->data);
            inOrderRec(node->right, func);
        }
    }
    
public:
    AVLTree() : root(nullptr), size(0) {}
    
    ~AVLTree() {
        clear();
    }
    
    // פונקציות ציבוריות
    
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
};

#endif // AVLTREE_H
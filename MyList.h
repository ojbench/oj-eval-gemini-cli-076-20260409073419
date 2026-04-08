#ifndef MYLIST_HPP
#define MYLIST_HPP

#include <stdexcept>
#include <utility>

template<typename ValueType>
class MyList
{
private:
    struct Node {
        ValueType data;
        unsigned int priority;
        int size;
        Node* left;
        Node* right;
        Node(const ValueType& val) : data(val), size(1), left(nullptr), right(nullptr) {
            static unsigned int seed = 123456789;
            seed = seed * 1664525 + 1013904223;
            priority = seed;
        }
    };

    Node* root;

    int getSize(Node* node) const {
        return node ? node->size : 0;
    }

    void update(Node* node) {
        if (node) {
            node->size = 1 + getSize(node->left) + getSize(node->right);
        }
    }

    void split(Node* node, int k, Node*& left, Node*& right) {
        if (!node) {
            left = right = nullptr;
            return;
        }
        int leftSize = getSize(node->left);
        if (k <= leftSize) {
            split(node->left, k, left, node->left);
            right = node;
        } else {
            split(node->right, k - leftSize - 1, node->right, right);
            left = node;
        }
        update(node);
    }

    Node* merge(Node* left, Node* right) {
        if (!left || !right) return left ? left : right;
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            update(left);
            return left;
        } else {
            right->left = merge(left, right->left);
            update(right);
            return right;
        }
    }

    Node* copyTree(Node* node) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->data);
        newNode->priority = node->priority;
        newNode->size = node->size;
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    Node* getKth(Node* node, int k) const {
        if (!node) return nullptr;
        int leftSize = getSize(node->left);
        if (k == leftSize) return node;
        if (k < leftSize) return getKth(node->left, k);
        return getKth(node->right, k - leftSize - 1);
    }

public:
    MyList() : root(nullptr) {}

    MyList(MyList &&obj) noexcept : root(obj.root) {
        obj.root = nullptr;
    }

    MyList(const MyList &obj) : root(copyTree(obj.root)) {}

    ~MyList() {
        deleteTree(root);
    }

    MyList& operator=(const MyList& obj) {
        if (this != &obj) {
            deleteTree(root);
            root = copyTree(obj.root);
        }
        return *this;
    }

    MyList& operator=(MyList&& obj) noexcept {
        if (this != &obj) {
            deleteTree(root);
            root = obj.root;
            obj.root = nullptr;
        }
        return *this;
    }

    void push_back(const ValueType &value) {
        root = merge(root, new Node(value));
    }

    void pop_back() {
        if (!root) return;
        Node *left, *right;
        split(root, getSize(root) - 1, left, right);
        delete right;
        root = left;
    }

    void push_front(const ValueType &value) {
        root = merge(new Node(value), root);
    }

    void pop_front() {
        if (!root) return;
        Node *left, *right;
        split(root, 1, left, right);
        delete left;
        root = right;
    }

    ValueType &front() const {
        if (!root) throw std::out_of_range("List is empty");
        return getKth(root, 0)->data;
    }

    ValueType &back() const {
        if (!root) throw std::out_of_range("List is empty");
        return getKth(root, getSize(root) - 1)->data;
    }

    void insert(int index, const ValueType &value) {
        if (index < 0 || index > getSize(root)) throw std::out_of_range("Index out of range");
        Node *left, *right;
        split(root, index, left, right);
        root = merge(merge(left, new Node(value)), right);
    }

    void erase(int index) {
        if (index < 0 || index >= getSize(root)) throw std::out_of_range("Index out of range");
        Node *left, *mid, *right;
        split(root, index, left, right);
        split(right, 1, mid, right);
        delete mid;
        root = merge(left, right);
    }

    int size() const {
        return getSize(root);
    }

    bool empty() const {
        return getSize(root) == 0;
    }

    void clear() {
        deleteTree(root);
        root = nullptr;
    }

    void link(const MyList &obj) {
        root = merge(root, copyTree(obj.root));
    }

    MyList cut(int index) {
        if (index < 0 || index > getSize(root)) throw std::out_of_range("Index out of range");
        Node *left, *right;
        split(root, index, left, right);
        root = left;
        MyList result;
        result.root = right;
        return result;
    }
};

#endif

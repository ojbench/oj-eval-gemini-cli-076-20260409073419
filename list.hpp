#ifndef MYLIST_HPP
#define MYLIST_HPP

#include <stdexcept>

template<typename ValueType>
class MyList
{
private:
    struct Node {
        ValueType data;
        Node* prev;
        Node* next;
        Node(const ValueType& val) : data(val), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int sz;

public:
    MyList() : head(nullptr), tail(nullptr), sz(0) {}

    MyList(MyList &&obj) noexcept : head(obj.head), tail(obj.tail), sz(obj.sz) {
        obj.head = nullptr;
        obj.tail = nullptr;
        obj.sz = 0;
    }

    MyList(const MyList &obj) : head(nullptr), tail(nullptr), sz(0) {
        Node* curr = obj.head;
        while (curr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    ~MyList() {
        clear();
    }

    MyList& operator=(const MyList& obj) {
        if (this != &obj) {
            clear();
            Node* curr = obj.head;
            while (curr) {
                push_back(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }

    MyList& operator=(MyList&& obj) noexcept {
        if (this != &obj) {
            clear();
            head = obj.head;
            tail = obj.tail;
            sz = obj.sz;
            obj.head = nullptr;
            obj.tail = nullptr;
            obj.sz = 0;
        }
        return *this;
    }

    void push_back(const ValueType &value) {
        Node* newNode = new Node(value);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        sz++;
    }

    void pop_back() {
        if (!tail) return;
        Node* temp = tail;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete temp;
        sz--;
    }

    void push_front(const ValueType &value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        sz++;
    }

    void pop_front() {
        if (!head) return;
        Node* temp = head;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
        sz--;
    }

    ValueType &front() const {
        if (!head) throw std::out_of_range("List is empty");
        return head->data;
    }

    ValueType &back() const {
        if (!tail) throw std::out_of_range("List is empty");
        return tail->data;
    }

    void insert(int index, const ValueType &value) {
        if (index < 0 || index > sz) throw std::out_of_range("Index out of range");
        if (index == 0) {
            push_front(value);
        } else if (index == sz) {
            push_back(value);
        } else {
            Node* curr = head;
            for (int i = 0; i < index; i++) {
                curr = curr->next;
            }
            Node* newNode = new Node(value);
            newNode->prev = curr->prev;
            newNode->next = curr;
            curr->prev->next = newNode;
            curr->prev = newNode;
            sz++;
        }
    }

    void erase(int index) {
        if (index < 0 || index >= sz) throw std::out_of_range("Index out of range");
        if (index == 0) {
            pop_front();
        } else if (index == sz - 1) {
            pop_back();
        } else {
            Node* curr = head;
            for (int i = 0; i < index; i++) {
                curr = curr->next;
            }
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            delete curr;
            sz--;
        }
    }

    int size() const {
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        sz = 0;
    }

    void link(const MyList &obj) {
        Node* curr = obj.head;
        while (curr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    MyList cut(int index) {
        if (index < 0 || index > sz) throw std::out_of_range("Index out of range");
        MyList result;
        if (index == sz) {
            return result;
        }
        if (index == 0) {
            result.head = head;
            result.tail = tail;
            result.sz = sz;
            head = tail = nullptr;
            sz = 0;
            return result;
        }
        
        Node* curr = head;
        for (int i = 0; i < index; i++) {
            curr = curr->next;
        }
        
        result.head = curr;
        result.tail = tail;
        result.sz = sz - index;
        
        tail = curr->prev;
        tail->next = nullptr;
        curr->prev = nullptr;
        
        sz = index;
        
        return result;
    }
};

#endif

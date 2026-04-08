#ifndef MYLIST_HPP
#define MYLIST_HPP

#include <vector>
#include <stdexcept>

template<typename ValueType>
class MyList
{
private:
    std::vector<ValueType> vec;

public:
    MyList() = default;
    MyList(MyList &&obj) noexcept : vec(std::move(obj.vec)) {}
    MyList(const MyList &obj) : vec(obj.vec) {}
    ~MyList() = default;

    MyList& operator=(const MyList& obj) {
        if (this != &obj) {
            vec = obj.vec;
        }
        return *this;
    }

    MyList& operator=(MyList&& obj) noexcept {
        if (this != &obj) {
            vec = std::move(obj.vec);
        }
        return *this;
    }

    void push_back(const ValueType &value) { vec.push_back(value); }
    void pop_back() { vec.pop_back(); }
    
    void push_front(const ValueType &value) { 
        vec.insert(vec.begin(), value); 
    }
    
    void pop_front() { 
        vec.erase(vec.begin()); 
    }
    
    ValueType &front() const { 
        if (vec.empty()) throw std::out_of_range("List is empty");
        return const_cast<ValueType&>(vec.front()); 
    }
    
    ValueType &back() const { 
        if (vec.empty()) throw std::out_of_range("List is empty");
        return const_cast<ValueType&>(vec.back()); 
    }

    void insert(int index, const ValueType &value) {
        if (index < 0 || index > vec.size()) throw std::out_of_range("Index out of range");
        vec.insert(vec.begin() + index, value);
    }

    void erase(int index) {
        if (index < 0 || index >= vec.size()) throw std::out_of_range("Index out of range");
        vec.erase(vec.begin() + index);
    }

    int size() const { return vec.size(); }
    bool empty() const { return vec.empty(); }
    void clear() { vec.clear(); }

    void link(const MyList &obj) {
        vec.insert(vec.end(), obj.vec.begin(), obj.vec.end());
    }

    MyList cut(int index) {
        if (index < 0 || index > vec.size()) throw std::out_of_range("Index out of range");
        MyList result;
        result.vec.assign(vec.begin() + index, vec.end());
        vec.erase(vec.begin() + index, vec.end());
        return result;
    }
};

#endif
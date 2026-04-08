#ifndef MYLIST_HPP
#define MYLIST_HPP

#include <list>
#include <stdexcept>

template<typename ValueType>
class MyList
{
private:
    std::list<ValueType> lst;

public:
    MyList() = default;
    MyList(MyList &&obj) noexcept : lst(std::move(obj.lst)) {}
    MyList(const MyList &obj) : lst(obj.lst) {}
    ~MyList() = default;

    MyList& operator=(const MyList& obj) {
        if (this != &obj) {
            lst = obj.lst;
        }
        return *this;
    }

    MyList& operator=(MyList&& obj) noexcept {
        if (this != &obj) {
            lst = std::move(obj.lst);
        }
        return *this;
    }

    void push_back(const ValueType &value) { lst.push_back(value); }
    void pop_back() { lst.pop_back(); }
    void push_front(const ValueType &value) { lst.push_front(value); }
    void pop_front() { lst.pop_front(); }
    
    ValueType &front() { return lst.front(); }
    const ValueType &front() const { return lst.front(); }
    
    ValueType &back() { return lst.back(); }
    const ValueType &back() const { return lst.back(); }

    void insert(int index, const ValueType &value) {
        auto it = lst.begin();
        int sz = lst.size();
        if (index > sz / 2) {
            it = lst.end();
            for (int i = sz; i > index; i--) --it;
        } else {
            for (int i = 0; i < index; i++) ++it;
        }
        lst.insert(it, value);
    }

    void erase(int index) {
        auto it = lst.begin();
        int sz = lst.size();
        if (index > sz / 2) {
            it = lst.end();
            for (int i = sz; i > index; i--) --it;
        } else {
            for (int i = 0; i < index; i++) ++it;
        }
        lst.erase(it);
    }

    int size() const { return lst.size(); }
    bool empty() const { return lst.empty(); }
    void clear() { lst.clear(); }

    void link(const MyList &obj) {
        lst.insert(lst.end(), obj.lst.begin(), obj.lst.end());
    }

    MyList cut(int index) {
        MyList result;
        auto it = lst.begin();
        int sz = lst.size();
        if (index > sz / 2) {
            it = lst.end();
            for (int i = sz; i > index; i--) --it;
        } else {
            for (int i = 0; i < index; i++) ++it;
        }
        result.lst.splice(result.lst.end(), lst, it, lst.end());
        return result;
    }
};

#endif
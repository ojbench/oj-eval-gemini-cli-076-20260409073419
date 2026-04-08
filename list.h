#ifndef MYLIST_HPP
#define MYLIST_HPP

#include <list>
#include <stdexcept>
#include <cmath>

template<typename ValueType>
class MyList
{
private:
    std::list<ValueType> lst;
    mutable typename std::list<ValueType>::iterator cache_it;
    mutable int cache_index;

    void reset_cache() const {
        cache_it = const_cast<std::list<ValueType>&>(lst).begin();
        cache_index = 0;
    }

    typename std::list<ValueType>::iterator get_iterator(int index) const {
        if (index == 0) return const_cast<std::list<ValueType>&>(lst).begin();
        if (index == lst.size()) return const_cast<std::list<ValueType>&>(lst).end();

        int dist_begin = index;
        int dist_end = lst.size() - index;
        int dist_cache = std::abs(index - cache_index);

        typename std::list<ValueType>::iterator it;
        if (dist_cache <= dist_begin && dist_cache <= dist_end) {
            it = cache_it;
            if (index > cache_index) {
                for (int i = 0; i < index - cache_index; ++i) ++it;
            } else {
                for (int i = 0; i < cache_index - index; ++i) --it;
            }
        } else if (dist_begin <= dist_end) {
            it = const_cast<std::list<ValueType>&>(lst).begin();
            for (int i = 0; i < dist_begin; ++i) ++it;
        } else {
            it = const_cast<std::list<ValueType>&>(lst).end();
            for (int i = 0; i < dist_end; ++i) --it;
        }

        cache_it = it;
        cache_index = index;
        return it;
    }

public:
    MyList() {
        reset_cache();
    }

    MyList(MyList &&obj) noexcept : lst(std::move(obj.lst)) {
        reset_cache();
        obj.reset_cache();
    }

    MyList(const MyList &obj) : lst(obj.lst) {
        reset_cache();
    }

    ~MyList() = default;

    MyList& operator=(const MyList& obj) {
        if (this != &obj) {
            lst = obj.lst;
            reset_cache();
        }
        return *this;
    }

    MyList& operator=(MyList&& obj) noexcept {
        if (this != &obj) {
            lst = std::move(obj.lst);
            reset_cache();
            obj.reset_cache();
        }
        return *this;
    }

    void push_back(const ValueType &value) { 
        bool is_end = (cache_index == lst.size());
        lst.push_back(value); 
        if (is_end) cache_index = lst.size();
    }

    void pop_back() { 
        if (lst.empty()) return;
        bool need_reset = (cache_index == lst.size() - 1);
        bool is_end = (cache_index == lst.size());
        lst.pop_back(); 
        if (need_reset) reset_cache();
        else if (is_end) cache_index = lst.size();
    }

    void push_front(const ValueType &value) { 
        lst.push_front(value); 
        cache_index++;
    }

    void pop_front() { 
        if (lst.empty()) return;
        if (cache_index == 0) {
            lst.pop_front();
            reset_cache();
        } else {
            lst.pop_front();
            cache_index--;
        }
    }

    ValueType &front() const { 
        if (lst.empty()) throw std::out_of_range("List is empty");
        return const_cast<ValueType&>(lst.front()); 
    }

    ValueType &back() const { 
        if (lst.empty()) throw std::out_of_range("List is empty");
        return const_cast<ValueType&>(lst.back()); 
    }

    void insert(int index, const ValueType &value) {
        if (index < 0 || index > lst.size()) throw std::out_of_range("Index out of range");
        auto it = get_iterator(index);
        lst.insert(it, value);
        if (index <= cache_index) {
            cache_index++;
        }
    }

    void erase(int index) {
        if (index < 0 || index >= lst.size()) throw std::out_of_range("Index out of range");
        auto it = get_iterator(index);
        if (it == cache_it) {
            cache_it = lst.erase(it);
        } else {
            lst.erase(it);
            if (index < cache_index) {
                cache_index--;
            }
        }
    }

    int size() const { return lst.size(); }
    bool empty() const { return lst.empty(); }
    
    void clear() { 
        lst.clear(); 
        reset_cache();
    }

    void link(const MyList &obj) {
        if (this == &obj) {
            MyList temp(obj);
            link(temp);
            return;
        }
        bool is_end = (cache_index == lst.size());
        lst.insert(lst.end(), obj.lst.begin(), obj.lst.end());
        if (is_end) {
            cache_index = lst.size();
        }
    }

    MyList cut(int index) {
        if (index < 0 || index > lst.size()) throw std::out_of_range("Index out of range");
        auto it = get_iterator(index);
        MyList result;
        result.lst.splice(result.lst.end(), lst, it, lst.end());
        if (cache_index >= index) {
            reset_cache();
        }
        return result;
    }
};

#endif

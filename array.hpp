#pragma once

#include <algorithm>
#include <vector>

namespace js4cpp {

template <typename T> class Array
{
public:
    Array();
    explicit Array(size_t length);

    T & operator [](size_t i);

    ssize_t indexOf(const T & item);

    void push(const T & value);
    void unshift(const T & value);
    T pop();
    T shift();

    void sort();
    template <class Function> void sort(Function comparator);

    template <class Function> void forEach(Function callback);
    template <typename R, class Function> Array<R> map(Function callback);

    template <class Function> bool every(Function callback);
    template <class Function> Array<T> filter(Function callback);

    template <class Function> T reduce(Function callback);
    template <class Function> T reduce(Function callback, const T & initialValue, size_t startForm = 0);

    size_t length() const;

    ~Array();
private:
    std::vector<T> data_;
};

template <typename T>
Array<T>::Array() : data_() {}

template <typename T>
Array<T>::Array(size_t length) : data_(length) {}

template <typename T>
T & Array<T>::operator [](size_t i) {
    return data_[i];
}

template <typename T>
ssize_t Array<T>::indexOf(const T & item) {
    typename std::vector<T>::iterator end = data_.end();
    typename std::vector<T>::iterator itemPos = std::find(data_.begin(), end, item);

    return itemPos != end ? (itemPos - data_.begin())  : -1;
}

template <typename T>
void Array<T>::push(const T & value) {
    data_.push_back(value);
}

template <typename T>
void Array<T>::unshift(const T & value) {
    data_.insert(data_.begin(), value);
}

template <typename T>
T Array<T>::pop() {
    T result = data_.back();
    data_.pop_back();
    return result;
}

template <typename T>
T Array<T>::shift() {
    T result = data_.front();
    data_.erase(data_.begin());
    return result;
}

template <typename T>
void Array<T>::sort() {
    std::sort(data_.begin(), data_.end());
}

template <typename T>
template <class Function>
void Array<T>::sort(Function comparator) {
    std::sort(data_.begin(), data_.end(), comparator);
}

template <typename T>
template <class Function>
void Array<T>::forEach(Function callback) {
    std::for_each(data_.begin(), data_.end(), callback);
}

template <typename T>
template <typename R, class Function>
Array<R> Array<T>::map(Function callback) {
    Array<R> result(length());

    std::transform(data_.begin(), data_.end(),
        result.data_.begin(), callback);

    return result;
}

template <typename T>
template <class Function>
Array<T> Array<T>::filter(Function callback) {
    Array<T> result(length());

    std::for_each(data_.begin(), data_.end(), [&result] (const T & item) {
        if (callback(item)) {
            result.push(item);
        }
    });
    return result;
}

template <typename T>
template <class Function>
bool Array<T>::every(Function callback) {
    bool result;

    std::for_each(data_.begin(), data_.end(), [&result] (const T & item) {
        result = result && callback(item);
    });

    return result;
}


template <typename T>
template <class Function>
T Array<T>::reduce(Function callback) {
    return reduce(callback, data_[0], 1);
}

template <typename T>
template <class Function>
T Array<T>::reduce(Function callback, const T & initialValue, size_t startFrom) {
    T result = initialValue;

    std::for_each(data_.begin() + startFrom, data_.end(), [&result] (const T & item) {
        result = callback(result, item);
    });

    return result;
}

template <typename T>
size_t Array<T>::length() const {
    return data_.size();
}

template <typename T>
Array<T>::~Array() {
    data_.clear();
}

} // namespace js4cpp

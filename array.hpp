#pragma once

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <tr1/functional>
#include <numeric>
#include <vector>

namespace js4cpp {

template <typename T> class Array
{
public:
    explicit Array(size_t length = 0);

    T & operator [](size_t i);

    ssize_t indexOf(const T & item);

    void push(const T & value);

    void unshift(const T & value);

    T pop();

    T shift();

    void reverse();

    Array<T> slice(ssize_t begin, ssize_t end = 0) const;

    void sort(std::tr1::function<bool(const T &, const T &)> comparator = std::less<T>());

    void forEach(std::tr1::function<void(T &)> callback);

    template <typename R>
    Array<R> map(std::tr1::function<R(const T &)> callback) const;

    bool every(std::tr1::function<bool(const T &)> callback) const;

    Array<T> filter(std::tr1::function<bool(const T &)> callback) const;

    T reduce(std::tr1::function<T(const T &, const T &)> callback = std::plus<T>());

    T reduce(std::tr1::function<T(const T &, const T &)> callback, const T & initialValue, size_t startFrom = 0);

    size_t length() const;
private:
    std::vector<T> data_;
};

template <typename T>
Array<T>::Array(size_t length) : data_(length) {}

template <typename T>
T & Array<T>::operator [](size_t i) {
// This is to provide an opportunity to disable JS-like Array
// behavior:
//
//  var a = new Array();
//  a.length; // -> 0
//  a[99] = 1;
//  a.length; // -> 100
#ifndef INTOLERANT_TO_OUT_OF_RANGE_INDEXES
    if (i >= data_.size()) {
        data_.resize(i + 1);
    }
#endif
    return data_[i];
}

template <typename T>
ssize_t Array<T>::indexOf(const T & item) {
    size_t index = std::find(data_.begin(), data_.end(), item) - data_.begin();

    return index < data_.size() ? index : -1;
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
void Array<T>::reverse() {
    std::reverse(data_.begin(), data_.end());
}

template <typename T>
Array<T> Array<T>::slice(ssize_t begin, ssize_t end) const {
    ssize_t length = data_.size();

    if (
        begin >= length ||
        begin < -length ||
        end >= length ||
        end < -length
    ) {
        return Array<T>();
    }

    begin = (length + begin) % length;
    end = (length + end) % length;

    size_t newSize = end > begin ? end - begin : 0;
    Array<T> result(newSize);

    if (newSize) {
        std::copy(
            data_.begin() + begin,
            data_.end() + end,
            result.data_.begin());
    }

    return result;
}

template <typename T>
void Array<T>::sort(std::tr1::function<bool(const T &, const T &)> comparator) {
    std::sort(data_.begin(), data_.end(), comparator);
}

template <typename T>
void Array<T>::forEach(std::tr1::function<void(T &)> callback) {
    std::for_each(data_.begin(), data_.end(), callback);
}

template <typename T>
template <typename R>
Array<R> Array<T>::map(std::tr1::function<R(const T &)> callback) const {
    Array<R> result(length());

    std::transform(data_.begin(), data_.end(),
        result.data_.begin(), callback);

    return result;
}

template <typename T>
Array<T> Array<T>::filter(std::tr1::function<bool(const T &)> callback) const {
    Array<T> result(length());
    size_t resultSize = std::remove_copy_if(data_.begin(), data_.end(), result.data_.begin(),
        std::not1(callback)) - result.data_.begin();

    result.data_.resize(resultSize);

    return result;
}

template <typename T>
bool Array<T>::every(std::tr1::function<bool(const T &)> callback) const {
    return std::find_if(data_.begin(), data_.end(), std::not1(callback)) == data_.end();
}


template <typename T>
T Array<T>::reduce(std::tr1::function<T(const T &, const T&)> callback) {
    return reduce(callback, data_[0], 1);
}

template <typename T>
T Array<T>::reduce(std::tr1::function<T(const T &, const T &)> callback, const T & initialValue, size_t startFrom) {
    return std::accumulate(data_.begin() + startFrom, data_.end(), initialValue, callback);
}

template <typename T>
size_t Array<T>::length() const {
    return data_.size();
}

} // namespace js4cpp

#pragma once

#include <stdint.h>
#include <stdlib.h>

namespace js4cpp {

const size_t DEFAULT_ARRAY_SIZE = 32;

template <typename T> class Array
{
public:
    Array();
    Array(size_t length);

    T & operator [](size_t i);

    int64_t indexOf(const T & item);

    void push(const T & value);
    void unshift(const T & value);
    T & pop();
    T & shift();

    template <class Function> bool every(Function callback);
    template <class Function> Array<T> & filter(Function callback);
    template <class Function> void forEach(Function callback);
    template <class Function> T & reduce(Function callback);
    template <class Function> T & reduce(Function callback, const T & initialValue, size_t startForm = 0);
    template <typename R, class Function> Array<R> & map(Function callback);

    size_t inline length();

    ~Array();
private:
    size_t dataLength_;
    size_t bufferLength_;
    T * buffer_;
    T * data_;
};

template <typename T>
Array<T>::Array() {
}

template <typename T>
Array<T>::Array(size_t length) {
    bufferLength_ = dataLength_ = length;
    buffer_ = data_ = (T *) malloc(length * sizeof(T));

    for (size_t i = 0; i < dataLength_; ++i) {
        data_[i] = T();
    }
}

template <typename T>
T & Array<T>::operator [](size_t i) {
    if (i < dataLength_) {
        return data_[i];
    } else {
        // TODO exception or resize array?
    }
}

template <typename T>
int64_t Array<T>::indexOf(const T & item) {
    size_t i = dataLength_;
    while (i--) {
        if (data_[i] == item) {
            return i;
        }
    }
    return -1;
}

template <typename T>
void Array<T>::push(const T & value) {
    if ((data_ + dataLength_) != (buffer_ + bufferLength_)) {
        data_[dataLength_] = value;
        dataLength_++;
    } else {
        // TODO resize array;
    }
}

template <typename T>
void Array<T>::unshift(const T & value) {
    if (data_ != buffer_) {
        data_--;
        data_[0] = value;
    } else {
        // TODO resize array;
    }
}

template <typename T>
T & Array<T>::pop() {
    dataLength_--;
    return data_[dataLength_];
}

template <typename T>
T & Array<T>::shift() {
    dataLength_--;
    data_++;
    return *(data_ - 1);
}

template <typename T>
template <class Function>
bool Array<T>::every(Function callback) {
    bool result = true;
    size_t i = dataLength_;

    while (i--) {
        result = result && callback(*buffer_[i]);
    }

    return result;
}

template <typename T>
template <class Function>
Array<T> & Array<T>::filter(Function callback) {
    Array<T> result(dataLength_);

    for (size_t i = 0; i < dataLength_; ++i) {
        if (callback(data_[i])) {
            result.push(data_[i]);
        }
    }

    return result;
}

template <typename T>
template <class Function>
void Array<T>::forEach(Function callback) {
    for (size_t i = 0; i < dataLength_; ++i) {
        callback(data_[i]);
    }
}

template <typename T>
template <class Function>
T & Array<T>::reduce(Function callback) {
    return reduce(callback, data_[0], 1);
}

template <typename T>
template <class Function>
T & Array<T>::reduce(Function callback, const T & initialValue, size_t startFrom) {
    T result = initialValue;

    for (size_t i = startFrom; i < dataLength_; ++i) {
        result = callback(result, data_[i]);
    }

    return result;
}

template <typename T>
template <typename R, class Function>
Array<R> & Array<T>::map(Function callback) {
    Array<R> result(dataLength_);

    for (size_t i = 0; i < dataLength_; ++i) {
        result.push(callback(data_[i]));
    }

    return result;
}

template <typename T>
size_t Array<T>::length() {
    return dataLength_;
}

template <typename T>
Array<T>::~Array() {
    free(buffer_);
}

} // namespace js4cpp

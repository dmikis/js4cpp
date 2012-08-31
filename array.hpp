/* Copyright (c) 2012 Kirill Dmitrenko <kdmitrenko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file array.hpp
 * JS-style Array.
 */

#pragma once

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <tr1/functional>
#include <iterator>
#include <numeric>
#include <vector>

/**
 * Project's namespace.
 */
namespace js4cpp {

/**
 * JS-style array.
 * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array
 *
 * @tparam T %Array's elements type.
 */
template <typename T> class Array
{
public:
    /**
     * Create new array with given preallocated length.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array#Syntax
     *
     * @param length Length.
     */
    explicit Array(size_t length = 0);

    /**
     * Create new array from iterators range.
     *
     * @tparam InputIterator Iterator.
     * @param begin Range's begin.
     * @param end Range's end.
     */
    template <class InputIterator>
    Array(InputIterator begin, InputIterator end);

    /**
     * Get element under given index.
     *
     * @param i Index.
     * @returns Element.
     */
    T & operator [](size_t i);

    /**
     * Get index of given item.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/indexOf
     *
     * @param item Item index of which we want to find out.
     * @returns Item's index or -1 if item does not exist in the array.
     */
    ssize_t indexOf(const T & item) const;

    /**
     * Push value to the end of array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/push
     *
     * @param value Value to be pushed.
     */
    void push(const T & value);

    /**
     * Insert value just before the first element of the array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/unshift
     *
     * @param value Value to be inserted.
     */
    void unshift(const T & value);

    /**
     * Remove one element from the end of the array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/pop
     *
     * @returns Removed element.
     */
    T pop();

    /**
     * Remove element from the end of the array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/shift
     *
     * @returns Removed element.
     */
    T shift();

    /**
     * Reverse order of elements in the array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/reverse
     */
    void reverse();

    /**
     * Create subarray bounded by begin and end indexes.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/slice
     *
     * @param begin Begin index.
     * @param end End index. If not specified, subarray will be bounded at the end by length of original array.
     * @returns Subarray.
     */
    Array<T> slice(ssize_t begin, ssize_t end = 0) const;

    /**
     * Rearrange elements by given comparator.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/sort
     *
     * @param comparator Comparator.
     */
    void sort(std::tr1::function<bool(const T &, const T &)> comparator = std::less<T>());

    /**
     * Iterate over all elements and call callback for every one.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/forEach
     *
     * @param callback Callback.
     */
    void forEach(std::tr1::function<void(T &)> callback);

    /**
     * Creates a new array with the results of calling a provided function on every element in this array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/map
     *
     * @tparam R New array's element type.
     * @param callback Function generates new array's element from this array's one.
     * @returns New array.
     */
    template <typename R>
    Array<R> map(std::tr1::function<R(const T &)> callback) const;

    /**
     * Tests whether all elements in the array pass the test implemented by the provided function.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/every
     *
     * @param condition Test implementation.
     * @returns `true` if all elements passed the test and `false` otherwise.
     */
    bool every(std::tr1::function<bool(const T &)> condition) const;

    /**
     * Create new array consists of only this array's elements passed the test.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/filter
     *
     * @param test Test implementation.
     * @returns New array.
     */
    Array<T> filter(std::tr1::function<bool(const T &)> test) const;

    /**
     * Apply a function against an accumulator and each value of the array (from left-to-right) as to reduce it to a single value.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/reduce
     *
     * @param callback Function to execute on each value in the array.
     * @returns Accumulated value.
     */
    T reduce(std::tr1::function<T(const T &, const T &)> callback = std::plus<T>()) const;

    /**
     * Apply a function against an accumulator and each value of the array (from left-to-right) as to reduce it to a single value.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/reduce
     *
     * @param callback Function to execute on each value in the array.
     * @param initialValue Accumulator initial value.
     * @param startFrom Index from which iteration will start.
     * @returns Accumulated value.
     */
    T reduce(std::tr1::function<T(const T &, const T &)> callback, const T & initialValue, size_t startFrom = 0) const;

    /**
     * Get array's length.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/length
     *
     * @returns Length.
     */
    size_t length() const;

private:
    std::vector<T> data_;
};

template <typename T>
Array<T>::Array(size_t length) : data_(length) {}

template <typename T>
template <class InputIterator>
Array<T>::Array(InputIterator begin, InputIterator end) : data_(begin, end) {}

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
ssize_t Array<T>::indexOf(const T & item) const {
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

    if (end < begin) {
        return Array<T>();
    }

    return Array<T>(data_.begin() + begin, data_.begin() + end);
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
Array<T> Array<T>::filter(std::tr1::function<bool(const T &)> test) const {
    Array<T> result;

    std::remove_copy_if(data_.begin(), data_.end(),
        std::back_inserter(result.data_),
        std::not1(test));

    return result;
}

template <typename T>
bool Array<T>::every(std::tr1::function<bool(const T &)> condition) const {
    return std::find_if(data_.begin(), data_.end(), std::not1(condition)) == data_.end();
}


template <typename T>
T Array<T>::reduce(std::tr1::function<T(const T &, const T&)> callback) const {
    return reduce(callback, data_[0], 1);
}

template <typename T>
T Array<T>::reduce(std::tr1::function<T(const T &, const T &)> callback, const T & initialValue, size_t startFrom) const {
    return std::accumulate(data_.begin() + startFrom, data_.end(), initialValue, callback);
}

template <typename T>
size_t Array<T>::length() const {
    return data_.size();
}

} // namespace js4cpp

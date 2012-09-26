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
    explicit Array(size_t length = 0) : data_(length) {}

    /**
     * Create new array from iterators range.
     *
     * @tparam InputIterator Iterator.
     * @param begin Range's begin.
     * @param end Range's end.
     */
    template <class InputIterator>
    Array(InputIterator begin, InputIterator end) : data_(begin, end) {}

    /**
     * Get element under given index.
     *
     * @param i Index.
     * @returns Element.
     */
    T & operator [](size_t i) {
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


    /**
     * Get index of given item.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/indexOf
     *
     * @param item Item index of which we want to find out.
     * @returns Item's index or -1 if item does not exist in the array.
     */
    ssize_t indexOf(const T & item) const {
        size_t index = std::find(data_.begin(), data_.end(), item) - data_.begin();

        return index < data_.size() ? index : -1;
    }

    /**
     * Get the last index at which a given element can be found in the array
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/lastIndexOf
     *
     * @param item Item last index of which we want to find out.
     * @returns Item's last index or -1 if item does not exist in the array.
     */
    ssize_t lastIndexOf(const T & item) const {
        size_t lastIndex = std::find(data_.rbegin(), data_.rend(), item) - data_.rbegin();

        return data_.size() - lastIndex - 1;
    }

    /**
     * Push value to the end of array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/push
     *
     * @param value Value to be pushed.
     */
    void push(const T & value) {
        data_.push_back(value);
    }

    /**
     * Insert value just before the first element of the array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/unshift
     *
     * @param value Value to be inserted.
     */
    void unshift(const T & value) {
        data_.insert(data_.begin(), value);
    }

    /**
     * Remove one element from the end of the array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/pop
     *
     * @returns Removed element.
     */
    T pop() {
        T result = data_.back();
        data_.pop_back();
        return result;
    }

    /**
     * Remove element from the end of the array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/shift
     *
     * @returns Removed element.
     */
    T shift() {
        T result = data_.front();
        data_.erase(data_.begin());
        return result;
    }

    /**
     * Reverse order of elements in the array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/reverse
     */
    void reverse() {
        std::reverse(data_.begin(), data_.end());
    }


    /**
     * Create subarray bounded by begin and end indexes.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/slice
     *
     * @param begin Begin index.
     * @param end End index. If not specified, subarray will be bounded at the end by length of original array.
     * @returns Subarray.
     */
    Array<T> slice(ssize_t begin, ssize_t end = 0) const {
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

    /**
     * Rearrange elements by given comparator.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/sort
     *
     * @param comparator Comparator.
     */
    void sort(std::tr1::function<bool(const T &, const T &)> comparator = std::less<T>()) {
        std::sort(data_.begin(), data_.end(), comparator);
    }


    /**
     * Iterate over all elements and call callback for every one.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/forEach
     *
     * @param callback Callback.
     */
    void forEach(std::tr1::function<void(T &)> callback) {
        std::for_each(data_.begin(), data_.end(), callback);
    }

    /**
     * Creates a new array with the results of calling a provided function on every element in this array.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/map
     *
     * @tparam R New array's element type.
     * @param callback Function generates new array's element from this array's one.
     * @returns New array.
     */
    template <typename R>
    Array<R> map(std::tr1::function<R(const T &)> callback) const {
        Array<R> result(length());

        std::transform(data_.begin(), data_.end(),
            result.data_.begin(), callback);

        return result;
    }

    /**
     * Tests whether all elements in the array pass the test implemented by the provided function.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/every
     *
     * @param condition Test implementation.
     * @returns `true` if all elements passed the test and `false` otherwise.
     */
    bool every(std::tr1::function<bool(const T &)> condition) const {
        return std::find_if(data_.begin(), data_.end(), std::not1(condition)) == data_.end();
    }

    /**
     * Tests whether any of elements in the array pass the test implemented by the provided function.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/some
     *
     * @param condition Test implementation.
     * @returns `true` if at least one element passed the test and `false` otherwise.
     */
    bool some(std::tr1::function<bool(const T &)> condition) const {
        return std::find_if(data_.begin(), data_.end(), condition) != data_.end();
    }

    /**
     * Create new array consists of only this array's elements passed the test.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/filter
     *
     * @param test Test implementation.
     * @returns New array.
     */
    Array<T> filter(std::tr1::function<bool(const T &)> test) const {
        Array<T> result;

        std::remove_copy_if(data_.begin(), data_.end(),
            std::back_inserter(result.data_),
            std::not1(test));

        return result;
    }

    /**
     * Apply a function against an accumulator and each value of the array (from left-to-right) as to reduce it to a single value.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/reduce
     *
     * @param callback Function to execute on each value in the array.
     * @returns Accumulated value.
     */
    T reduce(std::tr1::function<T(const T &, const T &)> callback = std::plus<T>()) const {
        return reduce(callback, data_[0], 1);
    }

    /**
     * Apply a function against an accumulator and each value of the array (from left-to-right) as to reduce it to a single value.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/reduce
     *
     * @param callback Function to execute on each value in the array.
     * @param initialValue Accumulator initial value.
     * @param startFrom Index from which iteration will start.
     * @returns Accumulated value.
     */
    T reduce(std::tr1::function<T(const T &, const T &)> callback, const T & initialValue, size_t startFrom = 0) const {
        return std::accumulate(data_.begin() + startFrom, data_.end(), initialValue, callback);
    }

    /**
     * Get array's length.
     * @see https://developer.mozilla.org/en-US/docs/JavaScript/Reference/Global_Objects/Array/length
     *
     * @returns Length.
     */
    size_t length() const {
        return data_.size();
    }

private:
    std::vector<T> data_;
};

} // namespace js4cpp

#pragma once

#include <functional>
#include <string>
#include <tr1/functional>

#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "array.hpp"

class ArrayTest : public CppUnit::TestCase
{
public:
    ArrayTest() : CppUnit::TestCase("Array Test Case") {};

    void setUp() {
        arr = new js4cpp::Array<int>(10);

        int count = 0;
        arr->forEach([&count] (int & item) {
            item = count++;
        });
    }

    void testLength() {
        CPPUNIT_ASSERT( arr->length() == 10 );
    }

    void testFilter() {
        using std::tr1::bind;
        using std::tr1::placeholders::_1;

        CPPUNIT_ASSERT( arr->filter(bind(std::greater<int>(), _1, 5)).length() == 4 );
    }

    void testReduce() {
        CPPUNIT_ASSERT( arr->reduce() == 45 );
    }

    void testEvery() {
        using std::tr1::bind;
        using std::tr1::placeholders::_1;

        CPPUNIT_ASSERT( arr->every(bind(std::greater_equal<int>(), _1, 0)) );

        CPPUNIT_ASSERT( !arr->every(bind(std::not_equal_to<int>(), _1, 5)) );
    }

    void testSome() {
        using std::tr1::bind;
        using std::tr1::placeholders::_1;

        CPPUNIT_ASSERT( arr->some(bind(std::equal_to<int>(), _1, 8)) );

        CPPUNIT_ASSERT( !arr->some(bind(std::equal_to<int>(), _1, 11)) );
    }

    void testSlice() {
        using std::tr1::bind;
        using std::tr1::placeholders::_1;
        using js4cpp::Array;

        Array<int> t1 = arr->slice(5);
        CPPUNIT_ASSERT( t1.length() == 5 );
        CPPUNIT_ASSERT( t1.every(bind(std::greater<int>(), _1, 4)) );

        Array<int> t2 = arr->slice(-3);
        CPPUNIT_ASSERT( t2.length() == 3 );
        CPPUNIT_ASSERT( t2.every(bind(std::greater<int>(), _1, 6)) );

        Array<int> t3 = arr->slice(1, -1);
        CPPUNIT_ASSERT( t3.length() == 8 );
        CPPUNIT_ASSERT( t3.every(bind(std::greater<int>(), _1, 0)) &&
            t3.every(bind(std::less<int>(), _1, 9)) );

        Array<int> t4 = arr->slice(-7, 7);
        CPPUNIT_ASSERT( t4.length() == 4 );
        CPPUNIT_ASSERT( t4.every(bind(std::greater<int>(), _1, 2)) &&
            t4.every(bind(std::less<int>(), _1, 7)) );

        Array<int> t5 = arr->slice(6, 5);
        CPPUNIT_ASSERT( t5.length() == 0 );

        Array<int> t6 = arr->slice(-1, 1);
        CPPUNIT_ASSERT( t6.length() == 0 );
    }

    void testPopPush() {
        CPPUNIT_ASSERT( arr->pop() == 9 );
        CPPUNIT_ASSERT( arr->length() == 9 );

        arr->push(10);

        CPPUNIT_ASSERT( (*arr)[arr->length() - 1] == 10 );
        CPPUNIT_ASSERT( arr->length() == 10 );
    }

    void testShiftUnshift() {
        CPPUNIT_ASSERT( arr->shift() == 0 );
        CPPUNIT_ASSERT( arr->length() == 9 );

        arr->unshift(10);

        CPPUNIT_ASSERT( (*arr)[0] == 10 );
        CPPUNIT_ASSERT( arr->length() == 10 );
    }

    void tearDown() {
        delete arr;
    }

    CPPUNIT_TEST_SUITE( ArrayTest );

        CPPUNIT_TEST( testLength );
        CPPUNIT_TEST( testFilter );
        CPPUNIT_TEST( testReduce );
        CPPUNIT_TEST( testEvery );
        CPPUNIT_TEST( testSome );
        CPPUNIT_TEST( testPopPush );
        CPPUNIT_TEST( testShiftUnshift );

    CPPUNIT_TEST_SUITE_END();

private:
    js4cpp::Array<int> * arr;
};

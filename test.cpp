#include <cppunit/ui/text/TestRunner.h>

#include "array.test.hpp"

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(ArrayTest::suite());
    runner.run();
    return 0;
}

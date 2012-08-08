#include <iostream>

#include "array.hpp"

int main() {
    js4cpp::Array<int> t(16);

    t.forEach([] (int & x) {
        x = 10;
    });

    t[3] = 10;

    t.forEach([] (int x) {
        std::cout << x << '\n';
    });

    std::cout << t.length() << '\n';
    std::cout << t.pop() << '\n';
    std::cout << t.length() << '\n';

    return 0;
}

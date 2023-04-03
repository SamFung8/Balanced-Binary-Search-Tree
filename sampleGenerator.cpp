/*
Here is a sample generator that will only generate the data containing the first three types of operations to "test.in".

It is only for demonstration, and you may design it on your own.
*/

#include <iostream>
#include <algorithm>
#include <ctime>

const int N = 10;

inline int randopt() {
    return rand() % 3 + 1;
}

inline int randval() {
    return rand() % 10;
}

int main() {
    freopen("test.in", "wb", stdout);

    srand(time(NULL));
    std::cout << N << std::endl;

    for (int i = 1; i <= N; i++) {
        std::cout << randopt() << " " << randval() << std::endl;
    }

    return 0;
}
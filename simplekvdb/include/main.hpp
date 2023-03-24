#include <version>
#include <iostream>

class Test2 {
public:
    int x = 3;

    int return42() {
        return 42;
    }

    void printVersion() {
            std::cout << "C++20 support: " << __cplusplus << std::endl;
    }
};
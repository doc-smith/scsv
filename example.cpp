#include "csv.h"

#include <iostream>
#include <string>

using namespace NCSV;


int main() {
    try {
        TCSV csv("test.csv");
        while (csv.HasNext()) {
            int a;
            int b;
            std::string c;
            int d;
            csv.Next().To(a, b, ignore, c, d);
            std::cout << a << ", "
                      << b << ", "
                      << c << ", "
                      << d << std::endl;
        }
    } catch (std::runtime_error& ex) {
        std::cerr << "error: " << ex.what() << std::endl;
    }
}


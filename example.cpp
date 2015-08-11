#include "csv.h"
#include "utilities.h"

#include <iostream>
#include <string>

using namespace NCSV;


struct TMyInt {
    int Value;
};


int main() {
    try {
        TCSV csv("test.csv");
        while (csv.HasNext()) {
            int a;
            int b;
            std::string c;
            TMyInt d;
            csv.Next().To(a, b, ignore, c,
                [&d](const std::string& s) {
                    d.Value = std::stoi(s);
                }
            );
            std::cout << a << ", "
                      << b << ", "
                      << c << ", "
                      << d.Value << std::endl;
        }
    } catch (std::runtime_error& ex) {
        std::cerr << "error: " << ex.what() << std::endl;
    }
}


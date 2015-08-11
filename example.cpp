#include "csv.h"

#include <iostream>
#include <string>

using namespace NCSV;


struct TMyInt {
    int Value;
};


namespace NCSV {
    template <>
    struct TConverter<TMyInt> {
        static TMyInt ConvertValue(const std::string& s) {
            return TMyInt { std::stoi(s) };
        }
    };
}



int main() {
    try {
        TCSV csv("test.csv");
        while (csv.HasNext()) {
            int a;
            int b;
            std::string c;
            TMyInt d;
            csv.Next().To(a, b, ignore, c, d);
            std::cout << a << ", "
                      << b << ", "
                      << c << ", "
                      << d.Value << std::endl;
        }
    } catch (std::runtime_error& ex) {
        std::cerr << "error: " << ex.what() << std::endl;
    }
}


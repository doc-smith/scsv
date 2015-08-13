#include "csv.h"
#include "utilities.h"
#include "optional.h"

#include <iostream>
#include <string>


struct TMyInt {
    int Value;
};


int main() {
    using namespace NCSV;

    try {
        TCSV csv("test.csv", TCSVParams().Delim(',').SkipHeader(true));
        while (csv.HasNext()) {
            Optional<int> a;
            int b;
            std::string c;
            TMyInt d;
            csv.Next().To(a, b, ignore, c,
                [&d](const std::string& s) {
                    d.Value = std::stoi(s);
                }
            );
            if (a) {
                std::cout << a.value() << ", ";
            } else {
                std::cout << "NA, ";
            }
            std::cout << b << ", "
                      << c << ", "
                      << d.Value << std::endl;
        }
    } catch (std::runtime_error& ex) {
        std::cerr << "error: " << ex.what() << std::endl;
    }
}


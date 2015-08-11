#include "utilities.h"


namespace NCSV {


std::vector<std::string> Split(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    if (!str.empty()) {
        size_t it = 0;
        do {
            size_t begin = it;
            it = str.find(delim, begin);
            if (it != std::string::npos) {
                tokens.push_back(str.substr(begin, it - begin));
                ++it;
            } else {
                tokens.push_back(str.substr(begin));
            }
        } while (it < str.size());
    }
    return tokens;
}


} // namespace NCSV


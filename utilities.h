#pragma once

#include <string>
#include <vector>
#include <stdexcept>


#define DECLARE_NOCOPY(T) \
    T(const T&) = delete; \
    T& operator = (const T&) = delete;


namespace NCSV {


class TCSVError : public std::runtime_error {
public:
    TCSVError(const std::string& msg)
        : std::runtime_error(msg)
    {}  
};


std::vector<std::string> Split(const std::string& str, char delim);


} // namespace NCSV


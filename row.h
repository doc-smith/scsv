#pragma once

#include "utilities.h"

#include <string>
#include <vector>


namespace NCSV {


struct TIgnore {};
static const TIgnore ignore;


namespace {

    template <typename T>
    T ConvertValue(const std::string& s);


    template <>
    std::string ConvertValue(const std::string& s) {
        return s;
    }


    template <>
    int ConvertValue(const std::string& s) {
        return std::stoi(s);
    }


    template <>
    double ConvertValue(const std::string& s) {
        return std::stod(s);
    }


    template <>
    float ConvertValue(const std::string& s) {
        return std::stof(s);
    }

}


class TRow {
public:
    TRow(const std::string& data, char delim)
        : Tokens(Split(data, delim))
    {
    }

    template <typename ... Args>
    void To(Args& ... args) {
        TImpl<0, Args...>::To(Tokens, args...);
    }

private:
    template <size_t Index, typename ... Args>
    struct TImpl;

    template <size_t Index, typename T>
    struct TImpl<Index, T> {
        static void To(const std::vector<std::string>& tokens, T& t) {
            if (Index >= tokens.size()) {
                throw TCSVError("to many values to unpack");
            }
            t = ConvertValue<T>(tokens[Index]);
        }
    };

    template <size_t Index, typename T, typename ... Args>
    struct TImpl<Index, T, Args...> {
        static void To(const std::vector<std::string>& tokens, T& t, Args& ... args) {
            if (Index >= tokens.size()) {
                throw TCSVError("to many values to unpack");
            }
            t = ConvertValue<T>(tokens[Index]);
            TImpl<Index + 1, Args...>::To(tokens, args...);
        }
    };

    template <size_t Index, typename ... Args>
    struct TImpl<Index, const TIgnore, Args...> {
        static void To(const std::vector<std::string>& tokens, TIgnore, Args& ... args) {
            if (Index >= tokens.size()) {
                throw TCSVError("to many values to unpack");
            }
            TImpl<Index + 1, Args...>::To(tokens, args...);
        }
    };

    std::vector<std::string> Tokens;
};


} // namespace NCSV

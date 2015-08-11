#pragma once

#include "utilities.h"

#include <string>
#include <vector>


namespace NCSV {


struct TIgnore {};
static const TIgnore ignore;


template <typename T>
struct TConverter;


template <>
struct TConverter<std::string> {
    static std::string ConvertValue(const std::string& s) {
        return s;
    }
};


template <>
struct TConverter<int> {
    static int ConvertValue(const std::string& s) {
        return std::stoi(s);
    }
};


template <>
struct TConverter<double> {
    static double ConvertValue(const std::string& s) {
        return std::stod(s);
    }
};


template <>
struct TConverter<float> {
    static float ConvertValue(const std::string& s) {
        return std::stof(s);
    }
};


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
            t = TConverter<T>::ConvertValue(tokens[Index]);
        }
    };

    template <size_t Index, typename T, typename ... Args>
    struct TImpl<Index, T, Args...> {
        static void To(const std::vector<std::string>& tokens, T& t, Args& ... args) {
            if (Index >= tokens.size()) {
                throw TCSVError("to many values to unpack");
            }
            t = TConverter<T>::ConvertValue(tokens[Index]);
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

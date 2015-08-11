#pragma once

#include "utilities.h"

#include <functional>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>


namespace NCSV {


struct TIgnore {};
static TIgnore ignore;


template<bool IsFunction, typename T>
struct TParser;


template<typename T>
struct TParser<true, T> {
    static void Parse(const std::string& s, T& parser) {
        parser(s);
    }
};


template<typename T>
struct TParser<false, T> {
    static void Parse(const std::string& s, T& value) {
        std::istringstream is(s);
        is >> value;
    }
};


    namespace {

    template <typename T>
    void Parse(const std::string& s, T& value) {
        constexpr bool is_function =
          std::is_convertible<T, std::function<void(const std::string&)>>::value;
        TParser<is_function, T>::Parse(s, value);
    }


    template<>
    void Parse<TIgnore>(const std::string& s, TIgnore&) {}

    } // namespace


class TRow {
public:
    TRow(const std::string& data, char delim)
        : Tokens(Split(data, delim))
    {
    }

    template <typename ... Args>
    void To(Args&& ... args) {
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
            Parse(tokens[Index], t);
        }
    };

    template <size_t Index, typename T, typename ... Args>
    struct TImpl<Index, T, Args...> {
        static void To(const std::vector<std::string>& tokens, T& t, Args& ... args) {
            if (Index >= tokens.size()) {
                throw TCSVError("to many values to unpack");
            }
            Parse(tokens[Index], t);
            TImpl<Index + 1, Args...>::To(tokens, args...);
        }
    };

    std::vector<std::string> Tokens;
};


} // namespace NCSV

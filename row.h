#pragma once

#include "optional.h"
#include "utilities.h"

#include <functional>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>


namespace NCSV {


struct TIgnore {
    TIgnore() {}
};
static const TIgnore ignore;


enum ArgumentType {
    PLAIN,
    FUNCTOR,
    OPTIONAL
};


template<int ArgumentType, typename T>
struct TParser;


template<typename T>
struct TParser<PLAIN, T> {
    static void Parse(const std::string& s, T& value) {
        std::istringstream is(s);
        is >> value;
    }
};


template<typename T>
struct TParser<FUNCTOR, T> {
    static void Parse(const std::string& s, T& parser) {
        parser(s);
    }
};


template<typename T>
struct TParser<OPTIONAL, T> {
    static void Parse(const std::string& s, T& optional) {
        if (!s.empty()) {
            typename T::value_type value;
            std::istringstream is(s);
            is >> value;
            optional = std::move(value);
        } else {
            optional.clear();
        }
    }
};


    namespace {

    template <typename T>
    struct IsOptional {
        enum { value = false };
    };

    template <typename T>
    struct IsOptional<Optional<T>> {
        enum { value = true };
    };

    constexpr int SelectType(bool is_function, bool is_optional) {
        return is_function ? FUNCTOR : (is_optional ? OPTIONAL : PLAIN);
    }

    template <typename T>
    void Parse(const std::string& s, T& value) {
        constexpr bool is_function =
          std::is_convertible<T, std::function<void(const std::string&)>>::value;
        constexpr bool is_optional = IsOptional<T>::value;
        TParser<SelectType(is_function, is_optional), T>::Parse(s, value);
    }


    template<>
    void Parse<const TIgnore>(const std::string& s, const TIgnore&) {
        UNUSED(s);
    }


    template<>
    void Parse<std::string>(const std::string& s, std::string& value) {
        value = s;
    }

    } // namespace


class TRow {
public:
    TRow(const std::string& data, char delim, char quoteChar) {
        auto callback = [&](const char* data, std::size_t size) {
            Tokens.push_back(std::string(data, size));
        };
        TCSVSplitter<decltype(callback)> splitter(callback, delim, quoteChar);
        splitter.Split(data);
    }

    template <typename ... Args>
    void To(Args&& ... args) {
        TImpl<0, Args...>::To(Tokens, args...);
    }

    void To(std::vector<std::string>& row) const {
        row.assign(Tokens.begin(), Tokens.end());
    }

private:
    template <std::size_t Index, typename ... Args>
    struct TImpl;

    template <std::size_t Index, typename T>
    struct TImpl<Index, T> {
        static void To(const std::vector<std::string>& tokens, T& t) {
            if (Index >= tokens.size()) {
                throw TCSVError("to many values to unpack");
            }
            Parse(tokens[Index], t);
        }
    };

    template <std::size_t Index, typename T, typename ... Args>
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

#pragma once

#include "optional.h"
#include "utilities.h"

#include <functional>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>


namespace CSV {


struct Ignore {
    Ignore() {}
};
static const Ignore ignore;


enum ArgumentType {
    PLAIN,
    FUNCTOR,
    OPTIONAL
};


template<int ArgumentType, typename T>
struct Parser;


template<typename T>
struct Parser<PLAIN, T> {
    static void parse(const std::string& s, T& value) {
        std::istringstream is(s);
        is >> value;
    }
};


template<typename T>
struct Parser<FUNCTOR, T> {
    static void parse(const std::string& s, T& parser) {
        parser(s);
    }
};


template<typename T>
struct Parser<OPTIONAL, T> {
    static void parse(const std::string& s, T& optional) {
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
        Parser<SelectType(is_function, is_optional), T>::parse(s, value);
    }


    template<>
    void Parse<const Ignore>(const std::string& s, const Ignore&) {
        UNUSED(s);
    }


    template<>
    void Parse<std::string>(const std::string& s, std::string& value) {
        value = s;
    }

    } // namespace


class Row {
public:
    Row(const std::string& data, char delim, char quoteChar) {
        auto callback = [&](const char* data, std::size_t size) {
            tokens_.push_back(std::string(data, size));
        };
        CSVSplitter<decltype(callback)> splitter(callback, delim, quoteChar);
        splitter.split(data);
    }

    template <typename ... Args>
    void to(Args&& ... args) {
        Impl<0, Args...>::to(tokens_, args...);
    }

    void to(std::vector<std::string>& row) const {
        row.assign(tokens_.begin(), tokens_.end());
    }

private:
    template <std::size_t Index, typename ... Args>
    struct Impl;

    template <std::size_t Index, typename T>
    struct Impl<Index, T> {
        static void to(const std::vector<std::string>& tokens, T& t) {
            if (Index >= tokens.size()) {
                throw CSVError("to many values to unpack");
            }
            Parse(tokens[Index], t);
        }
    };

    template <std::size_t Index, typename T, typename ... Args>
    struct Impl<Index, T, Args...> {
        static void to(const std::vector<std::string>& tokens, T& t, Args& ... args) {
            if (Index >= tokens.size()) {
                throw CSVError("to many values to unpack");
            }
            Parse(tokens[Index], t);
            Impl<Index + 1, Args...>::to(tokens, args...);
        }
    };

    std::vector<std::string> tokens_;
};


} // namespace NCSV

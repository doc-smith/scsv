#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include <cctype>


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


template <typename TCallback>
class TCSVSplitter {
    DECLARE_NOCOPY(TCSVSplitter)

public:
    TCSVSplitter(const TCallback& submitCallback, char delim, char quoteChar)
        : Submit(submitCallback)
        , Delim(delim)
        , QuoteChar(quoteChar)
    {}

    void Split(const std::string& str) const {
        TState state = TState::FIELD_NOT_BEGUN;
        std::size_t start = 0;
        std::size_t pos = start;
        bool quoted = false;
        int spaces = 0;

        while (pos < str.length()) {
            char c = str[pos++];
            switch (state) {
            case TState::FIELD_NOT_BEGUN:
                if (isspace(c)) {
                    start = pos;
                } else if (c == QuoteChar) {
                    state = TState::FIELD_BEGUN;
                    quoted = true;
                } else if (c == Delim) {
                    Submit(str.data() + start, 0);
                    quoted = false;
                    spaces = 0;
                    state = TState::FIELD_NOT_BEGUN;
                    start = pos;
                } else {
                    state = TState::FIELD_BEGUN;
                }
                break;
            case TState::FIELD_BEGUN:
                if (c == Delim) {
                    if (!quoted) {
                        Submit(str.data() + start, pos - start - spaces - 1);
                        quoted = false;
                        spaces = 0;
                        state = TState::FIELD_NOT_BEGUN;
                        start = pos;
                    }
                } else if (c == QuoteChar) {
                    if (quoted) {
                        state = TState::FIELD_MIGHT_HAVE_ENDED;
                    } else {
                        spaces = 0;
                    }
                } else if (isspace(c)) {
                    if (!quoted) {
                        ++spaces;
                    }
                } else {
                    spaces = 0;
                }
                break;
            case TState::FIELD_MIGHT_HAVE_ENDED:
                if (c == Delim) {
                    Submit(str.data() + start + 1, pos - start - spaces - 1);
                    quoted = false;
                    spaces = 0;
                    state = TState::FIELD_NOT_BEGUN;
                    start = pos;
                } else if (c == QuoteChar) {
                    if (spaces) {
                        spaces = 0;
                    } else {
                        state = TState::FIELD_BEGUN;
                    }
                } else if (isspace(c)) {
                    ++spaces;
                } else {
                    state = TState::FIELD_BEGUN;
                    spaces = 0;
                }
                break;
            }
        }

        if (pos != start) {
            switch (state) {
            case TState::FIELD_MIGHT_HAVE_ENDED:
                Submit(str.data() + start + 1, pos - start - spaces - 2);
                break;
            case TState::FIELD_BEGUN:
                Submit(str.data() + start, pos - start);
                break;
            case TState::FIELD_NOT_BEGUN:
                break;
            };
        }
    }

private:
    enum class TState {
        FIELD_NOT_BEGUN,
        FIELD_BEGUN,
        FIELD_MIGHT_HAVE_ENDED
    };

    TCallback Submit;
    char Delim;
    char QuoteChar;
};


} // namespace NCSV


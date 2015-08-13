#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include <cctype>


#define DECLARE_NOCOPY(T) \
    T(const T&) = delete; \
    T& operator = (const T&) = delete;

#define UNUSED(x) (void)(x)


namespace CSV {


class CSVError : public std::runtime_error {
public:
    CSVError(const std::string& msg)
        : std::runtime_error(msg)
    {}
};


template <typename TCallback>
class CSVSplitter {
    DECLARE_NOCOPY(CSVSplitter)

public:
    CSVSplitter(const TCallback& submitCallback, char delim, char quoteChar)
        : submit_(submitCallback)
        , delim_(delim)
        , quoteChar_(quoteChar)
    {}

    void split(const std::string& str) const {
        State state = State::FIELD_NOT_BEGUN;
        std::size_t start = 0;
        std::size_t pos = start;
        bool quoted = false;
        int spaces = 0;

        auto submitField = [&](std::size_t start, std::size_t end) {
            submit_(str.data() + start, end - start);
            quoted = false;
            spaces = 0;
            state  = State::FIELD_NOT_BEGUN;
        };

        while (pos < str.length()) {
            char c = str[pos++];
            switch (state) {
            case State::FIELD_NOT_BEGUN:
                if (isspace(c)) {
                    start = pos;
                } else if (c == quoteChar_) {
                    state = State::FIELD_BEGUN;
                    quoted = true;
                } else if (c == delim_) {
                    submitField(start, start);
                    start = pos;
                } else {
                    state = State::FIELD_BEGUN;
                }
                break;
            case State::FIELD_BEGUN:
                if (c == delim_) {
                    if (!quoted) {
                        submitField(start, pos - spaces - 1);
                        start = pos;
                    }
                } else if (c == quoteChar_) {
                    if (quoted) {
                        state = State::FIELD_MIGHT_HAVE_ENDED;
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
            case State::FIELD_MIGHT_HAVE_ENDED:
                if (c == delim_) {
                    submitField(start + 1, pos - spaces - 2);
                    start = pos;
                } else if (c == quoteChar_) {
                    if (spaces) {
                        spaces = 0;
                    } else {
                        state = State::FIELD_BEGUN;
                    }
                } else if (isspace(c)) {
                    ++spaces;
                } else {
                    state = State::FIELD_BEGUN;
                    spaces = 0;
                }
                break;
            }
        }

        if (pos != start) {
            switch (state) {
            case State::FIELD_MIGHT_HAVE_ENDED:
                submitField(start + 1, pos - spaces - 1);
                break;
            case State::FIELD_BEGUN:
                submitField(start, pos);
                break;
            case State::FIELD_NOT_BEGUN:
                break;
            };
        }
    }

private:
    enum class State {
        FIELD_NOT_BEGUN,
        FIELD_BEGUN,
        FIELD_MIGHT_HAVE_ENDED
    };

    TCallback submit_;
    char delim_;
    char quoteChar_;
};


} // namespace NCSV


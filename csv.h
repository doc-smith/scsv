#pragma once

#include "row.h"
#include "utilities.h"

#include <fstream>
#include <string>


namespace NCSV {


struct TCSVParams {
    TCSVParams& Delim(char delim) {
        _Delim = delim;
        return *this;
    }

    TCSVParams& QuoteChar(char quoteChar) {
        _QuoteChar = quoteChar;
        return *this;
    }

    TCSVParams& SkipHeader(bool skipHeader) {
        _SkipHeader = skipHeader;
        return *this;
    }

    char _Delim = ',';
    char _QuoteChar = '\"';
    bool _SkipHeader = false;
};


class TCSV {
    DECLARE_NOCOPY(TCSV)

public:
    TCSV(const std::string& filename, TCSVParams params = TCSVParams());
    bool HasNext() const;
    TRow Next();

private:
    std::ifstream Input;
    char Delim;
    char QuoteChar;
    bool SkipHeader;

    std::string Line;
    bool Eof = false;
    bool FirstLine = true;
};


} // namespace NCSV

#pragma once

#include "row.h"
#include "utilities.h"

#include <fstream>
#include <string>


namespace CSV {


struct CSVParams {
    CSVParams& Delim(char delim) {
        _Delim = delim;
        return *this;
    }

    CSVParams& QuoteChar(char quoteChar) {
        _QuoteChar = quoteChar;
        return *this;
    }

    CSVParams& SkipHeader(bool skipHeader) {
        _SkipHeader = skipHeader;
        return *this;
    }

    char _Delim = ',';
    char _QuoteChar = '\"';
    bool _SkipHeader = false;
};


class CSVReader {
    DECLARE_NOCOPY(CSVReader)

public:
    CSVReader(const std::string& filename, CSVParams params = CSVParams());
    bool hasNext() const;
    Row next();

private:
    std::ifstream input_;
    char delim_;
    char quoteChar_;
    bool skipHeader_;

    std::string line_;
    bool eof_ = false;
    bool firstLine_ = true;
};


} // namespace NCSV

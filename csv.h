#pragma once

#include "row.h"
#include "utilities.h"

#include <fstream>
#include <string>


namespace NCSV {


class TCSV {
    DECLARE_NOCOPY(TCSV)

public:
    TCSV(const std::string& filename, char delim=',', char quoteChar='\"');
    bool HasNext() const;
    TRow Next();

private:
    std::ifstream Input;
    char Delim;
    char QuoteChar;

    std::string Line;
    bool Eof = false;
    bool FirstLine = true;
};


} // namespace NCSV

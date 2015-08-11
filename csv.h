#pragma once

#include "row.h"
#include "utilities.h"

#include <fstream>
#include <string>


namespace NCSV {


class TCSV {
    DECLARE_NOCOPY(TCSV)

public:
    TCSV(const std::string& filename, char delim=',');
    bool HasNext() const;
    TRow Next();

private:
    std::ifstream Input;
    char Delim;

    std::string Line;
    bool Eof = false;
    bool FirstLine = true;
};


} // namespace NCSV

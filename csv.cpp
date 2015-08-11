#include "csv.h"


namespace NCSV {


TCSV::TCSV(const std::string& filename, char delim)
    : Input(filename)
    , Delim(delim)
{
    if (!Input) {
        throw TCSVError("cannot open csv file");
    }
}


bool TCSV::HasNext() const {
    return !Eof;
}


TRow TCSV::Next() {
    if (!HasNext()) {
        throw TCSVError("end of file");
    }

    if (FirstLine) {
        std::getline(Input, Line);
        FirstLine = false;
    }

    TRow row(std::move(Line), Delim);

    if (!std::getline(Input, Line)) {
        Eof = true;
    }

    return row;
}


} // namespace NCSV


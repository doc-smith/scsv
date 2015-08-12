#include "csv.h"


namespace NCSV {


TCSV::TCSV(const std::string& filename, TCSVParams params)
    : Input(filename)
    , Delim(params._Delim)
    , QuoteChar(params._QuoteChar)
    , SkipHeader(params._SkipHeader)
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

    TRow row(std::move(Line), Delim, QuoteChar);

    if (!std::getline(Input, Line)) {
        Eof = true;
    }

    return row;
}


} // namespace NCSV


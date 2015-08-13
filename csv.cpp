#include "csv.h"


namespace CSV {


CSVReader::CSVReader(const std::string& filename, CSVParams params)
    : input_(filename)
    , delim_(params._Delim)
    , quoteChar_(params._QuoteChar)
    , skipHeader_(params._SkipHeader)
{
    if (!input_) {
        throw CSVError("cannot open csv file");
    }
}


bool CSVReader::hasNext() const {
    return !eof_;
}


Row CSVReader::next() {
    if (!hasNext()) {
        throw CSVError("end of file");
    }

    if (firstLine_) {
        std::getline(input_, line_);
        firstLine_ = false;
    }

    Row row(std::move(line_), delim_, quoteChar_);

    if (!std::getline(input_, line_)) {
        eof_ = true;
    }

    return row;
}


} // namespace NCSV


#pragma once

#include "Reader.h"

namespace inputReader {

    class XMLParserReader : public Reader {

    public:
        XMLParserReader();
        virtual ~XMLParserReader();

        virtual void readFile(const char* filename); // TODO
    };

} // namespace inputReader

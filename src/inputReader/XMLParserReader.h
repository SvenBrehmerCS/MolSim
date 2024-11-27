#pragma once

#include "Reader.h"

namespace inputReader {

    class XMLParserReader : public Reader {

    public:
        XMLParserReader();
        virtual ~XMLParserReader();

        virtual void readFile(ParticleContainer& container, const char* filename); // TODO
    };

} // namespace inputReader

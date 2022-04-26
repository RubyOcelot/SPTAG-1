#include "inc/IVF/utils/SimpleTokenStream.h"

namespace IVF {

    SimpleTokenStream::SimpleTokenStream(std::unique_ptr<std::vector<Token>> inputTokens):inputTokens(std::move(inputTokens)),curPos(0) {

    }

    bool SimpleTokenStream::fetchNextToken() {
        if(curPos>=0&&curPos<inputTokens->size())
            return false;
        token=inputTokens->at(curPos++);
        return true;
    }

    void SimpleTokenStream::close() {
        inputTokens.reset();
    }

    void SimpleTokenStream::end() {

    }

    void SimpleTokenStream::reset() {
        curPos=0;
        token="";
    }
}
#ifndef IVF_TERMTFIDFSCORESCHEME_H
#define IVF_TERMTFIDFSCORESCHEME_H

#include <math.h>

#include "interfaces/ScoreScheme.h"
#include "inc/Core/Common.h"
#include "inc/IVF/utils/ByteToDocId.h"

namespace IVF {
    typedef uint32_t TermStatType;

    class TermTFIDFScoreScheme : public ScoreScheme {
    public:
        ScoreScheme *clone();

        bool postingStatisticsLoader(std::istream *rawStream) override;

        void keywordStatisticsLoader(std::string) override;

        void collectionStatisticsLoader(std::string) override;

        float score() override;

        DocId getDocId() override;
    private:
        float termFreq;
        TermStatType docFreq;
        TermStatType docNum;
        DocId docId = -1;
    };

    bool TermTFIDFScoreScheme::postingStatisticsLoader(std::istream *rawStream) {
        if(*rawStream){

            auto* buffer=new unsigned char[sizeof(SPTAG::SizeType)+sizeof(float)];
            rawStream->read((char*)(buffer), sizeof(SPTAG::SizeType));
            if(rawStream->gcount()!=sizeof(SPTAG::SizeType)){
                return false;
            }

            docId = ByteToDocId(buffer);

            rawStream->read((char*)(buffer), sizeof(TermStatType));
            if(rawStream->gcount()!=sizeof(TermStatType)){
                return false;
            }
            termFreq=*((float*)buffer);

            delete[] buffer;
            return true;
        }
        else{
            return false;
        }
    }

    void TermTFIDFScoreScheme::keywordStatisticsLoader(std::string rawData) {
        docFreq=*((TermStatType*)rawData.c_str());
    }

    void TermTFIDFScoreScheme::collectionStatisticsLoader(std::string rawData) {
        docNum=*((TermStatType*)rawData.c_str());
    }

    float TermTFIDFScoreScheme::score() {
        //inverse document frequency smooth
        return termFreq*(std::log((float)docNum/(float)(docFreq+1))+1);
    }

    DocId TermTFIDFScoreScheme::getDocId() {
        return docId;
    }
}

#endif //IVF_TERMTFIDFSCORESCHEME_H

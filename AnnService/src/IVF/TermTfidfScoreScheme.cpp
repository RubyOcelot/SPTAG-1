#include "inc/IVF/TermTfidfScoreScheme.h"

namespace IVF{

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

    std::unique_ptr<KeywordStatistic> TermTFIDFScoreScheme::getKeywordStatistic() {
        return std::make_unique<DocFreq>();
    }
}
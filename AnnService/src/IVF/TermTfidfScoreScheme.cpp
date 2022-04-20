#include "inc/IVF/TermTfidfScoreScheme.h"
#include "inc/IVF/utils/DataTransform.h"

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

    std::unique_ptr<KeywordStatistic> TermTFIDFScoreScheme::getEmptyKeywordStatistic() {
        return std::make_unique<TermTFIDFScoreScheme::DocFreq>();
    }

    TermTFIDFScoreScheme::TermTFIDFScoreScheme():docId(-1),docFreq(0),docNum(0),termFreq(0) {

    }

    ScoreScheme *TermTFIDFScoreScheme::clone() {
        return new TermTFIDFScoreScheme(termFreq,docFreq,docNum,docId);
    }

    std::unique_ptr<ScoreScheme> TermTFIDFScoreScheme::smart_clone() {
        return std::make_unique<TermTFIDFScoreScheme>(termFreq,docFreq,docNum,docId);
    }

    TermTFIDFScoreScheme::TermTFIDFScoreScheme(float termFreq, TermTFIDFScoreScheme::TermStatType docFreq,
                                               TermTFIDFScoreScheme::TermStatType docNum, DocId docId):termFreq(termFreq),docFreq(docFreq),docNum(docNum),docId(docId) {

    }

    std::string TermTFIDFScoreScheme::serializePostingStat(DocId docId, float termFreq) {

        return DataToString((uint32_t)docId)+DataToString( termFreq);
    }

    TermTFIDFScoreScheme::DocFreq::DocFreq():docFreq(0) {
    }

    TermTFIDFScoreScheme::DocFreq::DocFreq(TermStatType docFreq):docFreq(docFreq) {

    }

    void TermTFIDFScoreScheme::DocFreq::reset() {
        docFreq=0;
    }

    std::string TermTFIDFScoreScheme::DocFreq::getContent() {
        return TermTFIDFScoreScheme::TermStatToString(docFreq);
    }

    std::unique_ptr<KeywordStatistic> TermTFIDFScoreScheme::DocFreq::clone() {
        return std::make_unique<DocFreq>(docFreq);
    }

    std::unique_ptr<KeywordStatistic> TermTFIDFScoreScheme::DocFreq::getNew() {
        return std::make_unique<DocFreq>();
    }

    void TermTFIDFScoreScheme::DocFreq::set(const std::string &data) {
        docFreq=*StringToTermStat(data);
    }

    void TermTFIDFScoreScheme::DocFreq::modForAdd(const std::string &data) {
        docFreq++;
    }

    void TermTFIDFScoreScheme::DocFreq::modForDel(const std::string &data) {
        docFreq--;
    }

    TermTFIDFScoreScheme::DocNum::DocNum():docNum(0) {

    }

    TermTFIDFScoreScheme::DocNum::DocNum(TermTFIDFScoreScheme::TermStatType docNum):docNum(docNum) {

    }

    void TermTFIDFScoreScheme::DocNum::reset() {
        docNum=0;
    }

    void TermTFIDFScoreScheme::DocNum::set(const std::string & data) {
        docNum=*StringToTermStat(data);

    }

    void TermTFIDFScoreScheme::DocNum::modForAdd(const std::string &data) {
        docNum++;
    }

    void TermTFIDFScoreScheme::DocNum::modForDel(const std::string &data) {
        docNum--;
    }

    std::string TermTFIDFScoreScheme::DocNum::getContent() {
        return TermTFIDFScoreScheme::TermStatToString(docNum);
    }

    std::unique_ptr<CollectionStatistic> TermTFIDFScoreScheme::DocNum::clone() {
        return std::make_unique<DocNum>(docNum);
    }
}
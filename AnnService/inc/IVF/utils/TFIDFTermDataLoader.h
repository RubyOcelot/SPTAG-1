#ifndef SPTAGLIB_TFIDFTERMDATALOADER_H
#define SPTAGLIB_TFIDFTERMDATALOADER_H

#include <memory>
#include "inc/IVF/interfaces/TermDataLoader.h"
#include "inc/IVF/utils/TokenStream.h"

namespace IVF {
    class DocData{
    public:
        DocId docId;
        std::vector<std::string> tokens;
    };

    class TermInDoc{
    public:
        explicit TermInDoc(std::string str="",std::string posting_data="");
        std::string str;
        std::string posting_data;
    };

    class TFIDFTermDataLoader:public TermDataLoader {
    public:
        std::unique_ptr<TermSetDataHolder> getTermSetData(const std::string &path, const SourceDataType &dataType) override;
    private:
        void collectData(std::vector<std::shared_ptr<TermInDoc>> &termInDocData,
                         std::unique_ptr<TokenStream> tokenStream, DocId docId);
        std::unique_ptr<TermSetDataHolder> concat(std::vector<std::shared_ptr<TermInDoc>>& termInDocData);
    };
}

#endif //SPTAGLIB_TFIDFTERMDATALOADER_H

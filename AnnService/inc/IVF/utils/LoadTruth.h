#ifndef SPTAGLIB_LOADTRUTH_H
#define SPTAGLIB_LOADTRUTH_H

#include <vector>
#include <set>
#include "inc/Core/Common.h"
#include "inc/Core/Common/CommonUtils.h"
#include "inc/Core/Common/TruthSet.h"
#include "inc/Core/SPANN/Options.h"

using namespace SPTAG;

namespace IVF{
    class TruthWrap{
    public:
        std::vector<std::set<SPTAG::SizeType>> truth;
        int truthK;
        int loadData(SPANN::Options* p_opts,int numQueries){

            std::string truthFile = p_opts->m_truthPath;
            if (!truthFile.empty())
            {
                LOG(Helper::LogLevel::LL_Info, "Start loading TruthFile...\n");

                int K = p_opts->m_resultNum;
                truthK = (p_opts->m_truthResultNum <= 0) ? K : p_opts->m_truthResultNum;

                auto ptr = f_createIO();
                if (ptr == nullptr || !ptr->Initialize(truthFile.c_str(), std::ios::in | std::ios::binary)) {
                    LOG(Helper::LogLevel::LL_Error, "Failed open truth file: %s\n", truthFile.c_str());
                    exit(1);
                }
                int originalK = truthK;
                COMMON::TruthSet::LoadTruth(ptr, truth, numQueries, originalK, truthK, p_opts->m_truthType);
                char tmp[4];
                if (ptr->ReadBinary(4, tmp) == 4) {
                    LOG(Helper::LogLevel::LL_Error, "TruthWrap number is larger than query number(%d)!\n", numQueries);
                }

            }
            return 0;
        }
        int print_truth_by_id(SizeType p_vectorID){

            auto t=truth.at(p_vectorID);
            for(auto it:t){
                std::cout<<it<<" ";
            }
            std::cout<<std::endl;
            return 0;
        }
    };
}

#endif //SPTAGLIB_LOADTRUTH_H

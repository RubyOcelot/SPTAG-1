#include "inc/Core/Common.h"
#include "inc/Core/VectorIndex.h"

#include "inc/SSDServing/BuildHead/BootBuildHead.h"
#include "inc/SSDServing/IndexBuildManager/CommonDefines.h"


namespace SPTAG {
	namespace SSDServing {
		namespace BuildHead {
			ErrorCode Bootstrap(Options& options, const SPTAG::Helper::IniReader::ParameterValueMap& params) {
                // These three params are mandatory.
                auto indexBuilder = SPTAG::VectorIndex::CreateInstance(COMMON_OPTS.m_indexAlgoType, COMMON_OPTS.m_valueType);
                indexBuilder->SetParameter("DistCalcMethod", SPTAG::Helper::Convert::ConvertToString(COMMON_OPTS.m_distCalcMethod));

                for (const auto& iter : params)
                {
                    indexBuilder->SetParameter(iter.first.c_str(), iter.second.c_str());
                }

                SPTAG::ErrorCode code;

                {
                    std::shared_ptr<Helper::ReaderOptions> vectorOptions(new Helper::ReaderOptions(COMMON_OPTS.m_valueType, COMMON_OPTS.m_dim, VectorFileType::DEFAULT));
                    auto vectorReader = Helper::VectorSetReader::CreateInstance(vectorOptions);
                    if (ErrorCode::Success != vectorReader->LoadFile(COMMON_OPTS.m_headVectorFile))
                    {
                        LOG(Helper::LogLevel::LL_Error, "Failed to read vector file.\n");
                        exit(1);
                    }
                    std::shared_ptr<SPTAG::VectorSet> p_vectorSet = vectorReader->GetVectorSet();
                    std::shared_ptr<SPTAG::MetadataSet> p_metaSet = vectorReader->GetMetadataSet();
                    code = indexBuilder->BuildIndex(p_vectorSet, p_metaSet);
                    indexBuilder->SaveIndex(COMMON_OPTS.m_headIndexFolder);
                }

                if (SPTAG::ErrorCode::Success != code)
                {
                    fprintf(stderr, "Failed to build index.\n");
                    return ErrorCode::Fail;
                }
				return ErrorCode::Success;
			}
		}
	}
}

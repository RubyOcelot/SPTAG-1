#pragma once
#include <limits>

#include "inc/Core/Common.h"
#include "inc/Helper/StringConvert.h"

using namespace std;

namespace SPTAG {
	namespace SSDServing {
		namespace VectorSearch {
			class Options {
			public:
				// Both Building and Searching
				bool m_buildSsdIndex;
				string m_vectorIDTranslate;
				string m_headIndexFolder;
				string m_queryFile;
				int m_internalResultNum;
				int m_iNumberOfThreads;
				string m_headConfig;
				
				// query file meta
				VectorFileType m_queryFileType;
				SizeType m_iQueryNumber;
				DimensionType m_iQueryDimension;

				// Warmup file meta
				VectorFileType m_warmupFileType;
				SizeType m_iWarmupNumber;
				DimensionType m_iWarmupDimension;

				// truth file meta
				TruthFileType m_truthFileType;
				SizeType m_iTruthNumber;

				// Building
				string m_ssdIndex;
				int m_replicaCount;
				int m_postingPageLimit;
				bool m_outputEmptyReplicaID;

				// Searching
				string m_searchResult;
				string m_extraFullGraphFile;
				string m_truthFile;
				string m_warmupFile;
				string m_logFile;
				string m_extraMaxCheck;
				int m_qpsLimit;
				int m_resultNum;
				int m_queryCountLimit;

				Options() {
#define DefineSSDParameter(VarName, VarType, DefaultValue, RepresentStr) \
                VarName = DefaultValue; \

#include "inc/SSDServing/VectorSearch/ParameterDefinitionList.h"
#undef DefineSSDParameter
				}

				~Options() {}

				ErrorCode SetParameter(const char* p_param, const char* p_value)
				{
					if (nullptr == p_param || nullptr == p_value) return ErrorCode::Fail;

#define DefineSSDParameter(VarName, VarType, DefaultValue, RepresentStr) \
    else if (SPTAG::Helper::StrUtils::StrEqualIgnoreCase(p_param, RepresentStr)) \
    { \
        fprintf(stderr, "Setting %s with value %s\n", RepresentStr, p_value); \
        VarType tmp; \
        if (SPTAG::Helper::Convert::ConvertStringTo<VarType>(p_value, tmp)) \
        { \
            VarName = tmp; \
        } \
    } \

#include "inc/SSDServing/VectorSearch/ParameterDefinitionList.h"
#undef DefineSSDParameter

					return ErrorCode::Success;
				}
			};
		}
	}
}
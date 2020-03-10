#pragma once
#include "inc/SSDServing/Common/stdafx.h"
#include <limits>

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

				// Building
				string m_ssdIndex;
				int m_replicaCount;
				int m_postingPageLimit;
				bool m_outputEmptyReplicaID;

				// Searching
				string m_searchResult;
				string m_extraFullGraphFile;
				string m_extraGraphFile;
				string m_extraGraphVectorSetFile;
				string m_truthFile;
				string m_warmupFile;
				string m_logFile;
				string m_extraMaxCheck;
				int m_qpsLimit;
				int m_resultNum;
				int m_queryCountLimit;
				string m_parallelLoadPercentage;

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
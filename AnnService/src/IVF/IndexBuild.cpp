

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "inc/SSDServing/main.h"
#include "inc/IVF/SSDServingWrapper.h"
using namespace SPTAG;

//TODO vectortype float, type


int main(int argc, char* argv[]) {
    if (argc < 2)
    {
        LOG(Helper::LogLevel::LL_Error,
            "ssdserving configFilePath\n");
        exit(-1);
    }

    std::map<std::string, std::map<std::string, std::string>> my_map;
    auto ret = IVF::SetupIndex(false, &my_map, argv[1]);
    return 0;
}


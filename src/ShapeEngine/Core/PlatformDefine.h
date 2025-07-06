//
// Created by yuyzc on 2025/7/1.
//

#pragma once

#ifdef _WIN32
    #ifdef SE_BUILD_DLL
        #define SE_API __declspec(dllexport)
    #else
        #define SE_API __declspec(dllimport)
    #endif
#else
    #define SE_API
#endif
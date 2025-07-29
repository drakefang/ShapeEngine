//
// Created by yuyzc on 2025/7/29.
//

#pragma once

#ifdef _WIN32
    #ifdef SP_BUILD_DLL
        #define SHAPE_PLATFORM_API __declspec(dllexport)
    #else
        #define SHAPE_PLATFORM_API __declspec(dllimport)
    #endif
#else
    #define SHAPE_PLATFORM_API // ...
#endif
//
// Created by yuyzc on 2025/7/1.
//

#pragma once

#ifdef _WIN32
    #ifdef SE_BUILD_DLL
        #define SHAPE_ENGINE_API __declspec(dllexport)
    #else
        #define SHAPE_ENGINE_API __declspec(dllimport)
    #endif
#else
    #define SHAPE_ENGINE_API
#endif
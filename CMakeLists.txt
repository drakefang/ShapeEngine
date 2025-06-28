cmake_minimum_required(VERSION 3.31)
project(ShapeGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(ProjName "ShapeGame")

set(Sources
    src/main.cpp
    src/Core/Object.h
    src/Core/Object.cpp
    src/Core/Archive.cpp
    src/Core/Archive.h
    src/Core/Logger.cpp
    src/Core/Logger.h
    src/Core/BinaryArchive.cpp
    src/Core/BinaryArchive.h
)

add_executable(${ProjName} ${Sources})

add_compile_definitions(RAYGUI_IMPLEMENTATION)
target_link_libraries(${ProjName} PRIVATE
    raylib
    EnTT::EnTT
    glm::glm
    spdlog::spdlog
)

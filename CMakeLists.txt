cmake_minimum_required(VERSION 3.31)
set(ProjName "ShapeGame")
project(${ProjName})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/utf-8")
endif()

#find_package(bgfx CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
#find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


add_subdirectory(src/ShapeEngine)
add_subdirectory(src/ShapeHost)
add_subdirectory(src/ShapePlugins)

#[[
set(CoreSrc
    src/Core/Object.h
    src/Core/Object.cpp
    src/Core/Archive.cpp
    src/Core/Archive.h
    src/Core/Logger.cpp
    src/Core/Logger.h
    src/Core/BinaryArchive.cpp
    src/Core/BinaryArchive.h
)

set(GameplayFrameworkSrc
    src/GameplayFramework/Application.cpp
    src/GameplayFramework/Application.h
    src/GameplayFramework/GameInstance.cpp
    src/GameplayFramework/GameInstance.h
    src/GameplayFramework/World.cpp
    src/GameplayFramework/World.h
    src/GameplayFramework/Actor.cpp
    src/GameplayFramework/Actor.h
)

set(ComponentSrc
    src/GameplayFramework/Components/ActorComponent.cpp
    src/GameplayFramework/Components/ActorComponent.h
)

set(Sources
    src/main.cpp
)

add_executable(${ProjName}
    ${Sources}
    ${CoreSrc}
    ${GameplayFrameworkSrc}
    ${ComonentSrc}
)

target_include_directories(${ProjName} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_compile_definitions(
    RAYGUI_IMPLEMENTATION
    GLM_FORCE_QUAT_CTOR_INIT_XYZW
    GLM_FORCE_QUAT_DATA_XYZW
    GLM_ENABLE_EXPERIMENTAL
)
target_link_libraries(${ProjName} PRIVATE
    EnTT::EnTT
    glm::glm
    spdlog::spdlog
    bgfx::bx
    bgfx::bgfx
    bgfx::bimg
    bgfx::bimg_decode
)
]]
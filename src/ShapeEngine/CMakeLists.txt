
set(EngineName "ShapeEngine")

add_library(${EngineName} SHARED)

set(CoreSrc
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Archive.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/BinaryArchive.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/BinaryArchive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Logger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Object.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Object.cpp
)

set(AppSrc
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Application.h
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Application.cpp
)

target_sources(${EngineName} PRIVATE
    ${CoreSrc}
    ${AppSrc}
)

target_include_directories(${EngineName} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${EngineName} PRIVATE SE_BUILD_DLL)
target_link_libraries(${EngineName} PRIVATE
    EnTT::EnTT
    glm::glm
    spdlog::spdlog
    bgfx::bx
    bgfx::bgfx
    bgfx::bimg
    bgfx::bimg_decode
)

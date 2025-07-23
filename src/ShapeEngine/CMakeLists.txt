
set(EngineName "ShapeEngine")

add_library(${EngineName} SHARED)

set(CoreSrc
    Core/Archive.h
    Core/BinaryArchive.h
    Core/BinaryArchive.cpp
    Core/Logger.h
    Core/Logger.cpp
    Core/Object.h
    Core/Object.cpp
)

set(AppSrc
    App/Application.h
    App/Application.cpp
)

set(ModuleSrc
    Module/IModuleInterface.h
    Module/ModuleManager.cpp
    Module/ModuleManager.h
)

target_sources(${EngineName} PRIVATE
    ${CoreSrc}
    ${AppSrc}
    ${ModuleSrc}
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
    tomlplusplus::tomlplusplus
)

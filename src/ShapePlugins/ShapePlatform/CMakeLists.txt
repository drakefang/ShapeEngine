
set(PluginName "ShapePlatform")

add_library(${PluginName} SHARED)

set(PluginSrc
    PlatformPlugin.h
    PlatformPlugin.cpp
    PlatformModule.h
    PlatformModule.cpp
)

target_sources(${PluginName} PRIVATE
    ${PluginSrc}
)

target_compile_definitions(${PluginName} PRIVATE
    SP_BUILD_DLL
)

target_link_libraries(${PluginName} PRIVATE
    SDL3::SDL3
    spdlog::spdlog
    ShapeEngine
)

setup_plugin_deployment(${PluginName})

set(PluginName "ShapePlatformPlugin")

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

target_link_libraries(${PluginName} PRIVATE
    SDL3::SDL3
    spdlog::spdlog
    ShapeEngine
)

add_custom_command(
    TARGET ${PluginName}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:${PluginName}>"
    "$<TARGET_FILE_DIR:ShapeHost>/Plugins/$<TARGET_FILE_NAME:${PluginName}>"
    COMMENT "Copying ${PluginName}.dll to Plugins directory..."
)
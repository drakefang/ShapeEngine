
set(EngineName "ShapeEngine")

add_library(${EngineName} SHARED)

set(CoreSrc
    Core/Archive.h
    Core/Archive.cpp
    Core/BinaryArchive.h
    Core/BinaryArchive.cpp
    Core/Logger.h
    Core/Logger.cpp
    Core/Object.h
    Core/Object.cpp
    Core/GameTime.h
    Core/GameClock.h
    Core/GameClock.cpp
    Core/TimerManager.h
    Core/TimerManager.cpp
    Core/PlatformDefine.h
    Core/Coroutine/Task.h
)

set(AppSrc
    App/Application.h
    App/Application.cpp
    App/ConfigManager.h
    App/ConfigManager.cpp
    App/DynamicLibrary.h
    App/DynamicLibrary.cpp
    App/IPlugin.h
    App/IPrimaryGameModule.h
    App/EngineMain.cpp
)

set(ModuleSrc
    Module/IModuleInterface.h
    Module/ModuleManager.cpp
    Module/ModuleManager.h
)

set(ServiceSrc
    Service/ServiceLocator.h
    Service/ServiceLocator.cpp
)

target_sources(${EngineName} PRIVATE
    ${CoreSrc}
    ${AppSrc}
    ${ModuleSrc}
    ${ServiceSrc}
)

target_include_directories(${EngineName} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${EngineName} PRIVATE
    SE_BUILD_DLL
)

target_link_libraries(${EngineName} PRIVATE
    EnTT::EnTT
    #glm::glm
    spdlog::spdlog
    #bgfx::bx
    #bgfx::bgfx
    #bgfx::bimg
    #bgfx::bimg_decode
    tomlplusplus::tomlplusplus
)

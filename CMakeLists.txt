cmake_minimum_required(VERSION 3.31)
set(ProjName "ShapeGame")
project(${ProjName})

set(VCPKG_APPLOCAL_DEPS OFF CACHE BOOL "Disable vcpkg auto-dependency deployment")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options("/utf-8")
endif()

find_package(bgfx CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(PLUGIN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Plugins)
set(PLUGIN_DEPLOY_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Plugins)

function(setup_plugin_deployment PLUGIN_TARGET)
    set_target_properties(${PLUGIN_TARGET} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIRECTORY}"
        PDB_OUTPUT_DIRECTORY "${PLUGIN_OUTPU_DIRECTORY}"
    )

    add_custom_command(
        TARGET ${PLUGIN_TARGET}
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:${PLUGIN_TARGET}>"
        "${PLUGIN_DEPLOY_DIRECTORY}/$<TARGET_FILE_NAME:${PLUGIN_TARGET}>"

        COMMENT "Deploying ${PLUGIN_TARGET} to ${PLUGIN_DEPLOY_DIRECTORY}"
    )
    set(SEPLUGIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_TARGET}.seplugin")
    if(EXISTS ${SEPLUGIN_FILE})
        add_custom_command(
                TARGET ${PLUGIN_TARGET}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SEPLUGIN_FILE}"
                "${PLUGIN_DEPLOY_DIRECTORY}/${PLUGIN_TARGET}.seplugin"
                COMMENT "Deploying ${PLUGIN_TARGET}.seplugin to ${PLUGIN_DEPLOY_DIRECTORY}"
        )
    endif()

    if (MSVC)
        add_custom_command(
            TARGET ${PLUGIN_TARGET}
            POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PLUGIN_OUTPUT_DIRECTORY}/$<TARGET_PDB_FILE_NAME:${PLUGIN_TARGET}>"
            "${PLUGIN_DEPLOY_DIRECTORY}/$<TARGET_PDB_FILE_NAME:${PLUGIN_TARGET}>"

            COMMENT "Deploying ${PLUGIN_TARGET} PDB to ${PLUGIN_DEPLOY_DIRECTORY}"
        )
    endif()
endfunction()

add_subdirectory(src/ShapeEngine)
add_subdirectory(src/ShapeHost)
add_subdirectory(src/ShapePlugins)
#add_subdirectory(src/ShapeGameDemo)

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
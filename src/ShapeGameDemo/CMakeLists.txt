
set(PROJECT_NAME ShapeGameDemo)

find_program(SHADERC_EXECUTABLE shaderc HINTS ${CMAKE_CURRENT_SOURCE_DIR})

function(add_shader_target target_name shader_file platform profile type)
    set(input_path "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer/shaders/${shader_file}.sc")
    set(output_path "${CMAKE_BINARY_DIR}/shaders/${shader_file}.bin.h")
    set(shader_name "${shader_file}")

    add_custom_command(
            OUTPUT ${output_path}
            COMMAND ${BGFX_SHADERC_TOOLS} -f ${input_path} -o ${output_path} --type ${type} --platform ${platform} -p ${profile} --define BGFX_SHADER_EMBEDDED_HEADER_NAME=${shader_name}
            DEPENDS ${input_path}
            COMMENT "Compiling shader ${shader_file}.sc for ${platform}"
    )

    add_custom_target(${target_name} DEPENDS ${output_path})
endfunction()

if(WIN32)
    set(SHADER_PLATFORM "windows")
    set(VS_PROFILE "vs_4_0")
    set(FS_PROFILE "ps_4_0")
elseif(APPLE)
    set(SHADER_PLATFORM "osx")
    set(VS_PROFILE "metal")
    set(FS_PROFILE "metal")
else()
    set(SHADER_PLATFORM "linux")
    set(VS_PROFILE "spirv")
    set(FS_PROFILE "spirv")
endif()

add_shader_target(vs_basic_color vs_basic_color ${SHADER_PLATFORM} ${VS_PROFILE} "v")
add_shader_target(fs_basic_color fs_basic_color ${SHADER_PLATFORM} ${FS_PROFILE} "f")

add_executable(${PROJECT_NAME}
    main.cpp
    Core/Application.cpp
    Core/Log.cpp
    Renderer/Renderer2D.cpp
)

add_dependencies(${PROJECT_NAME} Shaders)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/Zc:__cplusplus")
    target_compile_options(${PROJECT_NAME} PRIVATE "/Zc:preprocessor")
endif()

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    bgfx::bgfx
    EnTT::EnTT
    spdlog::spdlog
    tomlplusplus::tomlplusplus
)
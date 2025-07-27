
set(HostName "ShapeHost")

add_executable(${HostName}
    main.cpp
)

target_link_libraries(
    ${HostName}
    PRIVATE
    SDL3::SDL3
)

add_custom_command(
    TARGET ${HostName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:ShapeEngine>
    $<TARGET_FILE_DIR:${HostName}>
    COMMENT "Copying ShapeEngine.dll to host directory"
)
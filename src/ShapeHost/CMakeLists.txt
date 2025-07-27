
set(HostName "ShapeHost")

add_executable(${HostName}
    main.cpp
)

target_link_libraries(
    ${HostName}
    PRIVATE
    SDL3::SDL3
)

add_rules("mode.debug", "mode.release")

local project_name = "ShapeGame"

set_project(project_name)
set_languages("c++20")

local requires = {
    "raylib 5.5",
    "entt v3.14.0",
    "glm 1.0.1"
}
add_requires(requires)

target(project_name)
    set_kind("binary")
    add_packages("raylib")
    add_packages("entt")
    add_packages("glm")

    add_includedirs("src")

    add_headerfiles("src/**/*.h")
    add_files("src/main.cpp")
    add_files("src/**/*.cpp")
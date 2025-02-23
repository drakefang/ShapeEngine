add_rules("mode.debug", "mode.release")

local project_name = "ShapeGame"

set_project(project_name)
set_languages("c++17")

local requires = {
    "raylib 5.5"
}
add_requires(requires)

target(project_name)
    set_kind("binary")
    add_packages("raylib")
    add_files("src/*.cpp")
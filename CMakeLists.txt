cmake_minimum_required (VERSION 3.15)

set(PROJECT_NAME ParticleSystem)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "-std=c++20 -fsanitize=address")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works (in creating the compile_commands.json file)
project(${PROJECT_NAME})

include(cmake/modules.cmake)
find_package(GameEngine REQUIRED)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ParticleSystem.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    glm::glm
    glad::glad
    fmt::fmt
    spdlog::spdlog

    # Linking the libraries for Cocoa, OpenGL, IOKit, and glfw
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework IOKit"
    glfw
    ${OPENGL_gl_LIBRARY}
    GLEW::GLEW
    imgui::imgui
    ImGuizmo::ImGuizmo
	GameEngine::GameEngine
)

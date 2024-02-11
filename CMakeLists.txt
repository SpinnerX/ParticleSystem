cmake_minimum_required (VERSION 3.15)

set(PROJECT_NAME Sandbox)
set(CMAKE_CXX_STANDARD 20)
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

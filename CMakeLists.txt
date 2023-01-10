cmake_minimum_required(VERSION 3.24)

project(GalaxySimulationUI)

set(CMAKE_CXX_STANDARD 11)

set(IMGUI_DIR ./imgui)
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set(GLFW_PATH ./glfw)
set(SOURCES
        main.cpp
        windows/window_handler.hpp)

include_directories(
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

find_package(OpenGL REQUIRED)
add_subdirectory(${GLFW_PATH})

add_executable(GalaxySimulationUI main.cpp ${SOURCES} ${IMGUI_SOURCES})
target_link_libraries(GalaxySimulationUI glfw ${OPENGL_gl_LIBRARY})

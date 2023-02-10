cmake_minimum_required(VERSION 3.24)

project(GalaxySimulationUI)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_DIR ./imgui)
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        imconfig.h
)
set(IMPLOT_DIR ./implot)
set(IMPLOT_SOURCES
        ${IMPLOT_DIR}/implot.cpp
        ${IMPLOT_DIR}/implot_demo.cpp
        ${IMPLOT_DIR}/implot_items.cpp
)

set(GLFW_PATH ./glfw)
set(WINDOW_SOURCES
        windows/window_handler.h
        windows/run_sim.h
        windows/header.h
        windows/window_logs.h
        windows/window_graphics.h
)

set(SOURCES
        main.cpp
        components/LiveLogViewer.cpp components/LiveLogViewer.h
        components/imgui_custom.h
        includes/Vector3.h
        includes/CommandLine.h
        includes/Globals.h includes/Globals.cpp
        includes/ConfigStruct.h
        includes/json.h
)

include_directories(
        ${IMPLOT_DIR}
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        components
        includes
)

find_package(OpenGL REQUIRED)
add_subdirectory(${GLFW_PATH})

add_executable(GalaxySimulationUI main.cpp ${SOURCES} ${WINDOW_SOURCES} ${IMGUI_SOURCES} ${IMPLOT_SOURCES})
target_link_libraries(GalaxySimulationUI glfw ${OPENGL_gl_LIBRARY})

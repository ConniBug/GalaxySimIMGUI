//
// Created by Conni Bilham on 10/01/2023.
//

#ifndef GALAXYSIMULATIONUI_WINDOW_HANDLER_HPP
#define GALAXYSIMULATIONUI_WINDOW_HANDLER_HPP

#include <string>
#include <imgui.h>
#include "GLFW/glfw3.h"
#include <thread>
#include <libc.h>
#include <LiveLogViewer.h>
#include "run_sim.h"
#include "header.h"
#include "window_logs.h"
#include <cmath>
#include <cassert>

namespace handler {
    void new_frame(GLFWwindow* window) {
        ImGui::ShowDemoWindow();
        ImGui::ShowStyleEditor();

        static bool coloursSet = false;
        if (!coloursSet) {
            ImVec4* colors = ImGui::GetStyle().Colors;
            colors[ImGuiCol_Button]                 = ImVec4(0.18f, 0.43f, 0.73f, 0.40f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.18f, 0.34f, 0.73f, 0.40f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.12f, 0.30f, 0.84f, 0.23f);
            coloursSet = true;
        }
        static bool once = false;
        static int animFrames = 230;
        static ImVec2 tmp_pos = ImVec2(0, 0);
        auto anim_progress = (float)(ImGui::GetFrameCount() % animFrames) / animFrames;
        if(once) {
            anim_progress = 1;
            goto A;
        }
        // Slow down the animation at 75%
        if (anim_progress > 0.75f) {
            anim_progress = 0.75f;
            ++animFrames;
        }
        // Ease out the animation
        anim_progress = 1.0625f - (1.0f - anim_progress) * (1.0f - anim_progress);
        if(anim_progress == 1) {
            once = true;
        }
        std::cout << "anim_progress: " << anim_progress << std::endl;
//        std::clamp(anim_progress, 0.0f, 1.0f);

        A:

        // Update the title bar header
        std::string str = "Galaxy Simulation - " + std::to_string((int)ImGui::GetIO().Framerate) + " FPS" +
                " - Frame: " + std::to_string(ImGui::GetFrameCount());

        glfwSetWindowTitle(window, str.c_str());

        auto header_size = ImVec2(ImGui::GetIO().DisplaySize.x, 100);
        windows::draw_header(
            ImVec2(// From -> To
                    std::lerp(ImGui::GetIO().DisplaySize.x, 0, anim_progress),
                    std::lerp(ImGui::GetIO().DisplaySize.y, 0, anim_progress)
            ),
            ImVec2(// From -> To
                    std::lerp(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.x, anim_progress),
                    std::lerp(0, header_size.y, anim_progress)
            )
        );


        windows::draw_logs(
                ImVec2(0, 100),
                ImVec2(1000, 100)

        );

        windows::run_sim(
            ImVec2(// From -> To
                    std::lerp(ImGui::GetIO().DisplaySize.x, 0, anim_progress),
                    std::lerp(ImGui::GetIO().DisplaySize.y, 100, anim_progress)
            ),
            ImVec2(500, ImGui::GetIO().DisplaySize.y - 100)
        );
    }
}


#endif //GALAXYSIMULATIONUI_WINDOW_HANDLER_HPP

//
// Created by Conni Bilham on 25/01/2023.
//

#ifndef GALAXYSIMULATIONUI_HEADER_H
#define GALAXYSIMULATIONUI_HEADER_H

#include "imgui.h"
#include "Vector3.h"

namespace windows {
    void draw_header(ImVec2 position, ImVec2 size) {
//            auto viewPort = ImGui::GetMainViewport()->Size;
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(position);
        if (ImGui::Begin("Header Window", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
            ImGui::Text("Galaxy Simulation - V0.1.0b");
            ImGui::Spacing();

            int resized_delta = ImGui::GetWindowSize().y - size.y;
            if(std::abs(resized_delta) >= 1)
                globals->top_panel_height = ImGui::GetWindowSize().y;
            int free_space = ImGui::GetMainViewport()->Size.y - globals->top_panel_height - globals->bottom_panel_height;
            if(free_space < 50) {
                globals->top_panel_height = ImGui::GetMainViewport()->Size.y - globals->top_panel_height - 50;
            }
            if(globals->top_panel_height < 50) {
                globals->top_panel_height = 50;
            } else if(globals->top_panel_height > 160) {
                globals->top_panel_height = 160;
            }


            // Get frame number
            ImGui::Text("Frame: %d", ImGui::GetFrameCount());

            // Get compile time info
            ImGui::Text("Compiled on: %s at %s", __DATE__, __TIME__);

            // Get runtime info
            ImGui::Text("Runtime: %s", std::string(std::to_string(__cplusplus) + " C++").c_str());

            ImGui::End();
        }
    }
} // Windows
#endif //GALAXYSIMULATIONUI_HEADER_H

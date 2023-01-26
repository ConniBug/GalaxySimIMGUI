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
        if (ImGui::Begin("Header Window", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
            ImGui::Text("Galaxy Simulation - V0.1.0b");
            ImGui::Spacing();

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

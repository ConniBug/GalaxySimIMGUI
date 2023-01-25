//
// Created by Conni Bilham on 24/01/2023.
//

#ifndef GALAXYSIMULATIONUI_RUN_SIM_H
#define GALAXYSIMULATIONUI_RUN_SIM_H

#include "imgui.h"

namespace handler {
    namespace windows {
        void run_sim() {
            static struct Config {
                struct star_generation {
                    bool enabled = false;

                    int active_mode = 1;
                    const char* modes[2] = {"Gaussian", "Uniform"};

                    int star_count = 0;
                    int stars_per_arm = 0;
                    float mean_mass = 0;
                    float std_mass = 0;


                } star_generation;
            } config;

            auto winSize = ImGui::GetWindowSize();
            ImGui::SetNextWindowSize(ImVec2{800, 400});
            ImGui::SetNextWindowPos(ImVec2{33, 30});
            if (ImGui::Begin("Simulation Window", NULL, /*ImGuiWindowFlags_NoResize |*/ /*ImGuiWindowFlags_NoMove |*/ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
                ImGui::SetCursorPos(ImVec2(25, 17.5));
                ImGui::Text("Galaxy Simulation - V0.1.0b");

                ImGui::SetCursorPos(ImVec2(10, 67.5));
                // Display position and size of window
                ImGui::Text("Window Position: (%.1f, %.1f)", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
                ImGui::Text("Window Size: (%.1f, %.1f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

                ImGui::Text("Cursor Position: (%.1f, %.1f)", ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);

                static int firstColWidth = 300;
                static int secondColWidth = 600;
                ImGui::Columns(2, "SimMainCols##333");  // 2-ways, with border
                ImGui::SetColumnWidth(0, firstColWidth);
                ImGui::SetColumnWidth(1, secondColWidth);

                ImGui::Separator();
                ImGui::Text("Simulation Config");
                ImGui::NextColumn();
                ImGui::Text("Simulation Preview");

                ImGui::Separator();
                ImGui::Columns(3, "sdsada");
                static int bias = 16;
                ImGui::SetColumnWidth(0, (firstColWidth / 2) + bias);
                ImGui::SetColumnWidth(1, (firstColWidth / 2) - bias);
                ImGui::SetColumnWidth(2, secondColWidth);

                ImGui::Text("Generate Stars");
                ImGui::NextColumn();
                static int tmp2 = 0;
                ImGui::Checkbox("##35228", &config.star_generation.enabled);
                ImGui::SameLine();
                ImGui::Combo("##35227", &config.star_generation.active_mode, config.star_generation.modes, IM_ARRAYSIZE(config.star_generation.modes));
                ImGui::NextColumn();
                ImGui::Text("Star Generation Preview");
                ImGui::NextColumn();
                ImGui::NextColumn();
                ImGui::Columns(2, "SimMainCols##323133");  // 2-ways, with border

                ImGui::SetColumnWidth(0, firstColWidth);
                ImGui::SetColumnWidth(1, secondColWidth);

                ImGui::Separator();
                auto cursorPos = ImGui::GetCursorPos();
                auto textSize = ImGui::CalcTextSize("Generation Settings");
                ImGui::SetCursorPos(ImVec2(cursorPos.x + ((firstColWidth - textSize.x) / 2) - 10, cursorPos.y));
                ImGui::Text("Generation Settings");
                ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + textSize.y + 10));
                ImGui::Separator();

                if(config.star_generation.enabled) {
                    ImGui::SliderInt("Star Count", &config.star_generation.star_count, 0, 1000000, "%d");
                    ImGui::SliderInt("Star Count", &config.star_generation.star_count, 0, 1000000, "%e");
                    ImGui::SliderInt("Stars Per Arm", &config.star_generation.stars_per_arm, 0, 10000, "%d");
                    ImGui::SliderFloat("Mean Mass", &config.star_generation.mean_mass, 0, 100, "%.1f Solar Masses");
                    ImGui::SliderFloat("std mass", &config.star_generation.std_mass, 0, 1, "%.1f Solar Masses");
                }

//                Vectorr min_position = Vectorr(-10, -10, -5);
//                Vectorr max_position = Vectorr(10, 10, 5);
//                Vectorr velocityAtOrigin = Vectorr(0.000002556727896654, 0.000002556727896654, 0.00000002);
//                Vectorr variation_velocity = Vectorr(0.00000001056727896654, 0.00000001056727896654, 0.00000000002);
//                long double variation_direction_x = 0.0174533;
//                long double variation_direction_y = 0.0174533;
//
//                int number_of_arms = 0;
//                std::vector<float> angle_of_arms = { };
//                float arm_width = 0.1;
//                float arm_length = 0.1;
//                float arm_height = 0.1;
//                float arm_offset = 0.1;
//
//                Vectorr gaussian_mean = Vectorr(0, 0, 0);
//                Vectorr gaussian_std = Vectorr(0.2, 0.2, 0.2);
//
                ImGui::SliderInt("##35229", &bias, -100, 100);
                ImGui::Separator();

            }
            ImGui::End();
        }
    }
}
#endif //GALAXYSIMULATIONUI_RUN_SIM_H

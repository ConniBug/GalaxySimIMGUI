//
// Created by Conni Bilham on 24/01/2023.
//

#ifndef GALAXYSIMULATIONUI_RUN_SIM_H
#define GALAXYSIMULATIONUI_RUN_SIM_H

#include "imgui.h"
#include "imgui_internal.h"
#include <imgui_custom.h>
#include <iostream>
#include <CommandLine.h>

namespace handler {
    namespace windows {
        void run_sim(ImVec2 position, ImVec2 size) {
            static struct Config {
                struct star_generation {
                    bool enabled = false;

                    int active_mode = 0;
                    const char* modes[3] = {"None", "Gaussian", "Uniform"};

                    int star_count = 0;
                    int stars_per_arm = 0;
                    float mean_mass = 0;
                    float std_mass = 0;


                } star_generation;
            } config;

            ImGui::SetNextWindowSize(size);
            ImGui::SetNextWindowPos(position);
            if (ImGui::Begin("Simulation Config Window", NULL,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                                                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
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

                if (ImGui::Custom::DropdownButton(ImVec2(20,20))) {
                    config.star_generation.enabled = !config.star_generation.enabled;
                }
//                ImGui::Checkbox("##35228", &config.star_generation.enabled);
//                ImGui::SameLine();
                if(ImGui::Combo("##35227", &config.star_generation.active_mode, config.star_generation.modes, IM_ARRAYSIZE(config.star_generation.modes))) {
                    config.star_generation.enabled = config.star_generation.active_mode != 0;
                }
                ImGui::NextColumn();

                ImGui::Text("Star Generation Preview");
                ImGui::NextColumn();
                ImGui::NextColumn();
                ImGui::Columns(2, "SimMainCols##323133");  // 2-ways, with border
//                ImGui::SetColumnWidth(0, firstColWidth);
                ImGui::SetColumnWidth(0, firstColWidth);
                ImGui::SetColumnWidth(1, secondColWidth);

                ImGui::Separator();
                {
                    const auto textSize_title = ImGui::CalcTextSize("Generation Settings");
                    // TODO: Optimise this
                    auto textSize_sub = ImGui::CalcTextSize(config.star_generation.modes[config.star_generation.active_mode]);

                    auto cursorPos = ImGui::GetCursorPos();
                    ImGui::SetCursorPos(ImVec2(cursorPos.x + ((firstColWidth - textSize_title.x) / 2) - 10, cursorPos.y));
                    ImGui::Text("Generation Settings");
                    ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + textSize_title.y + 5));

                    if(config.star_generation.enabled) {
                        cursorPos = ImGui::GetCursorPos();
                        ImGui::SetCursorPos(ImVec2(cursorPos.x + ((firstColWidth - textSize_sub.x) / 2) - 10, cursorPos.y));
                        ImGui::Text(config.star_generation.modes[config.star_generation.active_mode]);
                        ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + textSize_sub.y + 5));

                        ImGui::InputInt("Star Count", &config.star_generation.star_count, 1, 100, ImGuiInputFlags_RepeatRateNavTweak);
//                        ImGui::SliderInt("Star Count", &config.star_generation.star_count, 0, 10000, "%d");
//                        ImGui::SliderFloat("Star Count", (float*)&config.star_generation.star_count, 0, 1000, "%e");
                        ImGui::SliderInt("Stars Per Arm", &config.star_generation.stars_per_arm, 0, 10000, "%d");
                        ImGui::SliderFloat("Mean Mass", &config.star_generation.mean_mass, 0, 100, "%.1f Solar Masses");
                        ImGui::SliderFloat("std mass", &config.star_generation.std_mass, 0, 1, "%.1f Solar Masses");

                        static fVector pos1 = {0, 0, 0};
                        static fVector pos2 = {0, 0, 0};
                        ImGui::Spacing();
                        ImGui::Custom::CentreTextLocal("Simulation Area", firstColWidth);
                        ImGui::Custom::CentreTextLocal("Simulation Area", firstColWidth);
                        ImGui::Custom::CentreTextLocal("Simulation Area", firstColWidth);
                        ImGui::Custom::CentreTextLocal("Simulation Area", firstColWidth);
                        ImGui::Custom::InputVector3("Min", &pos1, "%.0f");
                        ImGui::Custom::InputVector3("Max", &pos2, "%.0f");
                    }
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
//                ImGui::SliderInt("##35229", &bias, -100, 100);
                ImGui::Separator();

                static bool generating = false;
                if(ImGui::Button("Generate")) {
                    if(generating) {
                        generating = false;
                        return;
                    }
                    generating = true;
                    std::cout << "Generating" << std::endl;

//                    std::thread t1(generate, &config);

                    CommandLine cmd = CommandLine("echo");
                    std::cout << cmd.getCommandlineString() << std::endl;
                    std::cout << cmd.executeAndWait() << std::endl;
                }
            }
            ImGui::End();
        }
    }
}
#endif //GALAXYSIMULATIONUI_RUN_SIM_H

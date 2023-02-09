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
#include "Globals.h"
#include <pthread.h>
#include <filesystem>

namespace windows {
    class SimThread {
    private:
        int program_pid = 0;
        static void* sim_thread(void* args) {
            int* program_pid = (int*)args;
            std::string pwd = std::filesystem::current_path().string();
    #if _WIN32 // Windows
            std::string path = pwd + "\\GalaxySimulation.exe";
    #else // Linux, Mac or other Unix based OS
            std::string path = pwd + "/GalaxySimulation";
    #endif
            CommandLine cmd = CommandLine(path);
            std::cout << cmd.getCommandlineString() << std::endl;
            std::cout << cmd.executeAndWait(program_pid) << std::endl;
        }
        pthread_t* thread = new pthread_t();
    public:
        SimThread() {
            pthread_create(thread, NULL, sim_thread, (void*)&program_pid);
        }
        ~SimThread() {
            printf("Killing simulation thread with PID %d\n", program_pid);
#if _WIN32 // Windows
            system(("taskkill /F /PID " + std::to_string(program_pid)).c_str());
            // Check if the process is still running
            if(system(("taskkill /F /PID " + std::to_string(program_pid)).c_str()) == 0)
#else // Linux, Mac or other Unix based OS
            system(("kill -9 " + std::to_string(program_pid) + " << /dev/null").c_str());
            // Check if the process is still running
            if(system(("kill -0 " + std::to_string(program_pid)).c_str()) == 0) {
#endif
                printf("Failed to kill simulation thread with PID %d\n", program_pid);
#ifdef _WIN32
                // Kill by proccess name now
                if(system(("taskkill /F /IM GalaxySimulation.exe").c_str()) == 0) {
#else // Linux, Mac or other Unix based OS
                // Kill by proccess name now
                if(system(std::string("killall GalaxySimulation").c_str()) == 0) {
#endif
                    printf("Killed simulation thread by name %d\n", program_pid);
                }
            }
            else
                printf("Killed simulation thread with PID %d\n", program_pid);
        }
    };
    std::vector<SimThread*> sim_threads = {};

    void run_sim(ImVec2 position, ImVec2 size, int flags) {
        static struct Config {
            int region_divisions[3] = {0, 0, 0};

            struct star_generation {
                int target_height = 0;
                int animation_speed = 5;
                void draw_config(star_generation *config, ImVec2 parent_size, int width) {
                    static int current_height = 0;
                    if(current_height != config->target_height)
                        current_height = std::lerp(current_height, target_height, config->animation_speed / 100.0f);

//                        return;
                    ImGui::Columns(1);

                    if(current_height &&
                            ImGui::BeginChild("Star Generation", ImVec2(parent_size.x - 16, current_height), false,
                                              ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {

                        if(current_height) {
                            ImGui::Custom::CentreTextLocal("Generation Settings", parent_size.x);

                            ImGui::Custom::CentreTextLocal(config->modes[config->active_mode], parent_size.x);

                            ImGui::Columns(2, "ssadopom");
                            ImGui::SetColumnWidth(0, (width / 2));
                            ImGui::Spacing();

                            ImGui::Text("Star Count"); ImGui::NextColumn();
                            ImGui::InputInt("##star_count", &config->star_count); ImGui::NextColumn();

                            ImGui::Text("Stars Per Arm"); ImGui::NextColumn();
                            ImGui::InputInt("##stars_per_arm", &config->stars_per_arm); ImGui::NextColumn();

                            ImGui::Text("Mean Mass"); ImGui::NextColumn();
                            ImGui::InputFloat("##mean_mass", &config->mean_mass); ImGui::NextColumn();

                            ImGui::Text("Std Mass"); ImGui::NextColumn();
                            ImGui::InputFloat("##std_mass", &config->std_mass); ImGui::NextColumn();

                            //                        ImGui::SliderInt("Star Count", &config->star_count, 0, 10000, "%d");
                            //                        ImGui::SliderFloat("Star Count", (float*)&config->star_count, 0, 1000, "%e");

                            ImGui::Spacing();
                            ImGui::Columns(1);
                            ImGui::Custom::CentreTextLocal("Simulation Area", parent_size.x);
                            ImGui::Columns(2, "oinoi");
                            ImGui::SetColumnWidth(0, (width / 2));

                            ImGui::Text("Min"); ImGui::NextColumn();
                            ImGui::InputInt3("##min_position", config->min_position); ImGui::NextColumn();

                            ImGui::Text("Max"); ImGui::NextColumn();
                            ImGui::InputInt3("##max_position", config->max_position); ImGui::NextColumn();
                        }
                        ImGui::EndChild();
                    }
                }

                bool enabled = false;

                int active_mode = 0;
                const char* modes[3] = {"None", "Gaussian", "Uniform"};

                int star_count = 0;
                int stars_per_arm = 0;
                float mean_mass = 0;
                float std_mass = 0;

                int min_position[3] = {0, 0, 0};
                int max_position[3] = {0, 0, 0};

            } star_generation;
        } config;

        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(position);
        if (ImGui::Begin("Simulation Config Window", NULL,  flags)) {
            static int firstColWidth = 300;

            ImGui::Separator();
            ImGui::Text("Simulation Config");
            ImGui::Separator();
            ImGui::Columns(2, "sdsada");
            static int bias = 16;
            ImGui::SetColumnWidth(0, (firstColWidth / 2) + bias);

            ImGui::Text("Generate Stars");
            ImGui::NextColumn();
            static int tmp2 = 0;


            // Star Generation Dropdown Button and Combo
            if (ImGui::ArrowButton("##DOWN_123", config.star_generation.enabled ? ImGuiDir_Down : ImGuiDir_Right)) {
                config.star_generation.enabled = !config.star_generation.enabled;
                config.star_generation.target_height = config.star_generation.enabled ? 215 : 0;

            } ImGui::SameLine();
            if(ImGui::Combo("##35227", &config.star_generation.active_mode, config.star_generation.modes, IM_ARRAYSIZE(config.star_generation.modes))) {
                config.star_generation.enabled = config.star_generation.active_mode != 0;
                config.star_generation.target_height = config.star_generation.enabled ? 215 : 0;
            }

            ImGui::NextColumn();

            {
                config.star_generation.draw_config(&config.star_generation, size, firstColWidth + 15);

                ImGui::Separator();

                ImGui::Columns(2, "sdsada");
                ImGui::SetColumnWidth(0, (firstColWidth / 2) + bias);

                ImGui::Text("Region Divisions");
                ImGui::Custom::HelpMarker(
                        "Controls the number of divisions in each dimension of the simulation region. "
                        "");

                ImGui::NextColumn();
                ImGui::InputInt3("##123", config.region_divisions); ImGui::NextColumn();

                ImGui::Columns(1);
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

                sim_threads.push_back(new SimThread());
                std::string pwd = std::filesystem::current_path().string();
                std::string path = pwd + "/log.txt";
                *globals->active_log_file = path;
            }
        }
        ImGui::End();
    }

    void clean_up() {
        for(auto& thread : sim_threads) {
            delete thread;
        }
    }
}
#endif //GALAXYSIMULATIONUI_RUN_SIM_H

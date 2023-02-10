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
        pthread_t* thread = new pthread_t();
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
    public:
        SimThread() {
            pthread_create(thread, nullptr, sim_thread, (void*)&program_pid);
        }
//        ~SimThread() {
//            printf("Killing simulation thread with PID %d\n", program_pid);
//#if _WIN32 // Windows
//            system(("taskkill /F /PID " + std::to_string(program_pid)).c_str());
//            // Check if the process is still running
//            if(system(("taskkill /F /PID " + std::to_string(program_pid)).c_str()) == 0)
//#else // Linux, Mac or other Unix based OS
//            system(("kill -9 " + std::to_string(program_pid) + " << /dev/null").c_str());
//            // Check if the process is still running
//            if(system(("kill -0 " + std::to_string(program_pid)).c_str()) == 0) {
//#endif
//                printf("Failed to kill simulation thread with PID %d\n", program_pid);
//#ifdef _WIN32
//                // Kill by proccess name now
//                if(system(("taskkill /F /IM GalaxySimulation.exe").c_str()) == 0) {
//#else // Linux, Mac or other Unix based OS
//                // Kill by proccess name now
//                if(system(std::string("killall GalaxySimulation").c_str()) == 0) {
//#endif
//                    printf("Killed simulation thread by name %d\n", program_pid);
//                }
//            }
//                else
//                printf("Killed simulation thread with PID %d\n", program_pid);
//        }
    };
    std::vector<SimThread*> sim_threads = {};

    void run_sim(ImVec2 position, ImVec2 size, int flags) {
        static struct Config {
            int region_divisions[3] = {0, 0, 0};

            struct star_generation {
                // Used to animate the config window's height
                int target_height = 0;
                int animation_speed = 9;

                const int line_height = 20;
                const int config_window_height = line_height * 15; // Line height * number of lines


                const void draw_config(star_generation *config, ImVec2 parent_size, int width) {
                    static float current_height = 0;
                    if(current_height != config->target_height)
                        current_height = std::lerp(current_height, target_height, config->animation_speed / 100.0f);
                    if(current_height < 1)
                        current_height = 0;
//                        return;
                    ImGui::Columns(1);

                    ImVec2 temp_size = ImVec2(parent_size.x - 16, current_height);
                    static int last_height = current_height;
//                    std::cout << "Current Height: " << current_height << std::endl;
//                    std::cout << "Last Height: " << last_height << std::endl;
                    unsigned int flags = current_height >= target_height / 2  ? 0 :
                            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
                    last_height = current_height;

                    if(current_height && ImGui::BeginChild("Star Generation##123", temp_size, false, flags))
                    {
                        int tmp_col_counter = 0;
#define SETUP_COLUMN ++tmp_col_counter; ImGui::Columns(2, "STAR_GEN_COL##" + tmp_col_counter); ImGui::SetColumnWidth(0, width / 2); ImGui::Spacing()

                        ImGui::Custom::CentreTextLocal("Generation Settings", parent_size.x);

                        ImGui::Custom::CentreTextLocal(config->modes[config->active_mode], parent_size.x);

                        SETUP_COLUMN;

                        ImGui::Text("Star Count"); ImGui::NextColumn();
                        ImGui::InputInt("##star_count", &config->star_count); ImGui::NextColumn();

                        ImGui::Text("Stars Per Arm"); ImGui::NextColumn();
                        ImGui::InputInt("##stars_per_arm", &config->stars_per_arm); ImGui::NextColumn();

                        ImGui::Text("Mean Mass"); ImGui::NextColumn();
                        ImGui::InputFloat("##mean_mass", &config->mean_mass, 0, 0, "%f Solar Masses"); ImGui::NextColumn();

                        ImGui::Text("Std Mass"); ImGui::NextColumn();
                        ImGui::InputFloat("##std_mass", &config->std_mass, 0, 0, "%f Solar Masses"); ImGui::NextColumn();
                        std::clamp(config->std_mass, 0.0f, config->mean_mass);

                        //                        ImGui::SliderInt("Star Count", &config->star_count, 0, 10000, "%d");
                        //                        ImGui::SliderFloat("Star Count", (float*)&config->star_count, 0, 1000, "%e");

                        ImGui::Spacing(); ImGui::Columns(1);
                        ImGui::Custom::CentreTextLocal("Simulation Area", parent_size.x);

                        SETUP_COLUMN;

                        ImGui::Text("Min"); ImGui::NextColumn();
                        ImGui::InputInt3("##min_position", config->min_position); ImGui::NextColumn();

                        ImGui::Text("Max"); ImGui::NextColumn();
                        ImGui::InputInt3("##max_position", config->max_position); ImGui::NextColumn();

                        ImGui::Text("Velocity At Origin"); ImGui::NextColumn();
                        ImGui::InputFloat3("##velocityAtOrigin", config->velocity_origin); ImGui::NextColumn();

                        ImGui::Text("Variation Velocity"); ImGui::NextColumn();
                        ImGui::InputFloat3("##variation_velocity", config->variation_velocity); ImGui::NextColumn();

                        ImGui::Text("Variation Direction"); ImGui::NextColumn();
                        ImGui::InputFloat3("##variation_direction", config->variation_direction); ImGui::NextColumn();

                        ImGui::Spacing(); ImGui::Columns(1);
                        ImGui::Custom::CentreTextLocal("Arm Generation", parent_size.x);

                        SETUP_COLUMN;

                        ImGui::Text("Arm Count"); ImGui::NextColumn();
                        if(ImGui::InputInt("##number_of_arms", &config->number_of_arms)) {
                            if(config->number_of_arms < 0)
                                config->number_of_arms = 0;
                            config->angle_of_arms.resize(config->number_of_arms);
                        } ImGui::NextColumn();

                        for(int i = 0; i < config->number_of_arms; ++i) {
                            // Shift cursor to right a little
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                            ImGui::Text(("Arm - " + std::to_string(i)).c_str());
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);

                            ImGui::NextColumn();
                            ImGui::SliderAngle(("##angle_of_arm_" + std::to_string(i)).c_str(), &config->angle_of_arms[i]); ImGui::NextColumn();
                        }

                        ImGui::Text("Width"); ImGui::NextColumn();
                        ImGui::InputFloat("##arm_width", &config->arm_width); ImGui::NextColumn();

                        ImGui::Text("Length"); ImGui::NextColumn();
                        ImGui::InputFloat("##arm_length", &config->arm_length); ImGui::NextColumn();

                        ImGui::Text("Height"); ImGui::NextColumn();
                        ImGui::InputFloat("##arm_height", &config->arm_height); ImGui::NextColumn();

                        ImGui::Text("Offset"); ImGui::NextColumn();
                        ImGui::InputFloat("##arm_offset", &config->arm_offset); ImGui::NextColumn();

                        SETUP_COLUMN;
#undef SETUP_COLUMN
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

                float velocity_origin[3] = {0, 0, 0};
                float variation_velocity[3]= {0, 0, 0};
                float variation_direction[3] = {0, 0, 0};

                std::vector<float> angle_of_arms = {};
                int number_of_arms = 0;
                float arm_width = 0;
                float arm_length = 0;
                float arm_height = 0;
                float arm_offset = 0;

                long double gaussian_mean[3] = {0, 0, 0};
                long double gaussian_std[3] = {0, 0, 0};

            } star_generation;
        } config;

        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(position);
        if (ImGui::Begin("Simulation Config Window", NULL,  flags)) {
            int resized_delta = ImGui::GetWindowSize().x - size.x;
            ImGui::Text("Resized by: %d", resized_delta);
            if(std::abs(resized_delta) >= 1)
                globals->left_panel_width = ImGui::GetWindowSize().x;

            if (ImGui::BeginMenuBar())
            {
                ImGui::Text("Simulation Config");
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                    if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
                    if (ImGui::MenuItem("Close", "Ctrl+W"))  { /*my_tool_active = false;*/ }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Separator();
            ImGui::Columns(2, "sdsada");
            static int bias = 0;
            ImGui::SetColumnWidth(-1, (globals->left_panel_width / 2) + bias);

            ImGui::Text("Generate Stars");
            ImGui::NextColumn();
            static int tmp2 = 0;

            // Star Generation Dropdown Button and Combo
            if (ImGui::ArrowButton("##DOWN_123", config.star_generation.enabled ? ImGuiDir_Down : ImGuiDir_Right)) {
                config.star_generation.enabled = !config.star_generation.enabled;
                config.star_generation.target_height = config.star_generation.enabled ? config.star_generation.config_window_height : 0;

            } ImGui::SameLine();
            if(ImGui::Combo("##35227", &config.star_generation.active_mode, config.star_generation.modes, IM_ARRAYSIZE(config.star_generation.modes))) {
                config.star_generation.enabled = config.star_generation.active_mode != 0;
                config.star_generation.target_height = config.star_generation.enabled ? config.star_generation.config_window_height : 0;
            }

            ImGui::NextColumn();

            {
                config.star_generation.draw_config(&config.star_generation, size, globals->left_panel_width - 15);

                ImGui::Separator();

                ImGui::Columns(2, "sdsada");
                ImGui::SetColumnWidth(-1, (globals->left_panel_width / 2) + bias);

                ImGui::Text("Region Divisions");
                ImGui::Custom::HelpMarker("Controls the number of divisions in each dimension of the simulation region. ");

                ImGui::NextColumn();
                ImGui::InputInt3("##region_divisions", config.region_divisions); ImGui::NextColumn();

                ImGui::Columns(1);
            }
//                Vectorr gaussian_mean = Vectorr(0, 0, 0);
//                Vectorr gaussian_std = Vectorr(0.2, 0.2, 0.2);
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

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

namespace handler {
    namespace windows {

        void newLogLine(widgets::LiveLogViewer* logViewer, std::string line){
            logViewer->addLog("%s", line.c_str());
//            for(int i = 0; i < line.size(); i++) {
////                if(line[i] ==
//            }
        }

        void credits_window() {
            static bool creditsWindows = false;

            auto winSize = ImGui::GetWindowSize();

            ImGui::SetNextWindowPos(ImVec2(winSize.x / 2, winSize.y / 2), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(200, 200));
            ImGui::Begin("T", &creditsWindows,
        //                 ImGuiWindowFlags_NoResize |
        //                 ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
            ImGui::Text("Galaxy Simulation UI");
            ImGui::Text("Version: 0.1b");
            //    ImGui::Text("
            ImGui::End();
        }

        void live_logs() {
            static bool liveLogs = true;
            static widgets::LiveLogViewer logViewer;
            logViewer.logFile = "/Users/connibilham/CLionProject/GalaxySimIMGUI/cmake-build-debug/log.txt";
            logViewer.logFile = "/Users/connibilham/CLionProjects/galaxy-in-cpp/cmake-build-debug/log.txt";

            static bool once =false;
            if(!once){
                once = true;

                pthread_t thr;
                pthread_create( &thr, NULL, logViewer.watch_thread, &logViewer);
            }
            logViewer.Draw("Live Logs", &liveLogs);
        }
    }

    void new_frame(GLFWwindow* window) {
        ImGui::ShowDemoWindow();
        ImGui::ShowStyleEditor();

        // Update the title bar header
        std::string str = "Galaxy Simulation - " + std::to_string((int)ImGui::GetIO().Framerate) + " FPS";
        glfwSetWindowTitle(window, str.c_str());

//        auto winSize = ImGui::GetWindowSize();
//        ImGui::SetNextWindowSize(winSize);
//        if (ImGui::Begin("window_name")) {
//            ImGui::SetCursorPos(ImVec2(10,37.5));
//            ImGui::Text("Galaxy Simulation - V0.1.0b");
//        }
//        ImGui::End();

        windows::live_logs();
        windows::credits_window();

        windows::run_sim();
    }
}


#endif //GALAXYSIMULATIONUI_WINDOW_HANDLER_HPP

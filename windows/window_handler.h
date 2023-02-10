//
// Created by Conni Bilham on 10/01/2023.
//

#ifndef GALAXYSIMULATIONUI_WINDOW_HANDLER_H
#define GALAXYSIMULATIONUI_WINDOW_HANDLER_H

#include "Globals.h"
#include <string>
#include <imgui.h>
#include "GLFW/glfw3.h"
#include <thread>
#include <libc.h>
#include <LiveLogViewer.h>
#include "run_sim.h"
#include "header.h"
#include "window_logs.h"
#include "window_graphics.h"
#include <cmath>
#include <cassert>
#include <random>

int bgStarPos[1000][2];
int bgStarVel[1000][2];

static float vel_range = 50.65f;

void InitStars() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis_x(0, ImGui::GetMainViewport()->Size.x);
    std::uniform_int_distribution<> dis_y(0, ImGui::GetMainViewport()->Size.y);

    std::normal_distribution<float> dis_v(-vel_range / 2, vel_range * 2);
    for (int i = 0; i < 1000; i++) {
//        bgStarVel[i][0] = dis_v(gen);
//        bgStarVel[i][1] = dis_v(gen);

        bgStarPos[i][0] = dis_x(gen);
        bgStarPos[i][1] = dis_y(gen);

        bgStarVel[i][0] = dis_v(gen);
        bgStarVel[i][1] = dis_v(gen);
    }
}
#include <vector>
void DrawDots()
{
    if ( 2 == 1 ){
        auto *draw = ImGui::GetForegroundDrawList();

        struct Region {
            ImVec2 min;
            ImVec2 max;
        };

        std::vector<Region> regions{
                {ImVec2(-50, -50), ImVec2(150, 150)},
                {ImVec2(-50, 50),  ImVec2(150, 250)},
                {ImVec2(-50, 150), ImVec2(150, 350)},
                {ImVec2(-50, 250), ImVec2(150, 450)},
                {ImVec2(50, -50),  ImVec2(250, 150)},
                {ImVec2(50, 50),   ImVec2(250, 250)},
                {ImVec2(50, 150),  ImVec2(250, 350)},
                {ImVec2(50, 250),  ImVec2(250, 450)},
                {ImVec2(150, -50), ImVec2(350, 150)},
                {ImVec2(150, 50),  ImVec2(350, 250)},
                {ImVec2(150, 150), ImVec2(350, 350)},
                {ImVec2(150, 250), ImVec2(350, 450)},
                {ImVec2(250, -50), ImVec2(450, 150)},
                {ImVec2(250, 50),  ImVec2(450, 250)},
                {ImVec2(250, 150), ImVec2(450, 350)},
                {ImVec2(250, 250), ImVec2(450, 450)},
        };

#define COLOUR_RED ImVec4(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOUR_GREEN ImVec4(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOUR_BLUE ImVec4(0.0f, 0.0f, 1.0f, 1.0f)
#define COLOUR_YELLOW ImVec4(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOUR_MAGENTA ImVec4(1.0f, 0.0f, 1.0f, 1.0f)
#define COLOUR_CYAN ImVec4(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOUR_WHITE ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOUR_BLACK ImVec4(0.0f, 0.0f, 0.0f, 1.0f)

        int cnt = 0;
        for (auto &region: regions) {
            ++cnt;
            static std::random_device dev;
            static std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 255); // distribution in range [1, 6]

            region.min.x += 100;
            region.min.y += 100;
            region.max.x += 100;
            region.max.y += 100;

            ImVec4 colour = COLOUR_WHITE;
            switch (cnt % 6) {
                case 0:
                    colour = COLOUR_RED;
                    break;
                case 1:
                    colour = COLOUR_GREEN;
                    break;
                case 2:
                    colour = COLOUR_BLUE;
                    break;
                case 3:
                    colour = COLOUR_YELLOW;
                    break;
                case 4:
                    colour = COLOUR_MAGENTA;
                    break;
                case 5:
                    colour = COLOUR_CYAN;
                    break;
            }
            colour.x *= 255;
            colour.y *= 255;
            colour.z *= 255;
            draw->AddRect(region.min, region.max, IM_COL32(colour.x, colour.y, colour.z, 100));
        }
    }

    static bool once = true;
    if (once) {
        once = false;
        InitStars();
    }
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    static std::random_device rd;
    static std::mt19937 gen(rd()); gen.seed(rand());
    for (int i = 0; i < 1000; i++) {
        std::uniform_int_distribution<> dis_alpha(100, 150);

        ImVec2 dot_pos(bgStarPos[i][0], bgStarPos[i][1]);
        draw_list->AddCircleFilled(dot_pos, 1.0f, ImColor(255, 255, 255, dis_alpha(gen)));

        std::uniform_real_distribution<float> dis_offset(-2.1f, 2.2f);

//        bgStarVel[i][0] += dis_offset(gen);
//        bgStarVel[i][1] += dis_offset(gen);

        bgStarPos[i][0] += (bgStarVel[i][0] + dis_offset(gen)) * ImGui::GetIO().DeltaTime;
        bgStarPos[i][1] += (bgStarVel[i][1] + dis_offset(gen)) * ImGui::GetIO().DeltaTime;

        if(bgStarPos[i][1] < 5) {
//            printf("Star %d went off screen top");
            bgStarPos[i][1] += ImGui::GetMainViewport()->Size.y - 10;
        } else if (bgStarPos[i][1] > ImGui::GetMainViewport()->Size.y - 5) {
            bgStarPos[i][1] -= ImGui::GetMainViewport()->Size.y - 10;
        }
        if(bgStarPos[i][0] < 5) {
//            printf("Star %d went off screen left");
            bgStarPos[i][0] += ImGui::GetMainViewport()->Size.x - 10;
        } else if (bgStarPos[i][0] > ImGui::GetMainViewport()->Size.x - 5) {
            bgStarPos[i][0] -= ImGui::GetMainViewport()->Size.x - 10;
        }

        // Check for collision with left or right wall
//        if (bgStarPos[i][0] < 0 || bgStarPos[i][0] > ImGui::GetMainViewport()->Size.x) {
//            bgStarVel[i][0] = -bgStarVel[i][0] * 3;
//        }
//        // Check for collision with top or bottom wall
//        if (bgStarPos[i][1] < 0 || bgStarPos[i][1] > ImGui::GetMainViewport()->Size.y) {
//            bgStarVel[i][1] = -bgStarVel[i][1] * 4;
//        }

    }

    draw_list->PopClipRect();
}

void init() {
    printf("Initializing...\n");

    printf("Done initializing.\n");
}
void new_frame(GLFWwindow* window) {
//    ImGui::ShowDemoWindow(&globals->show_demo_window);
    if(globals->show_another_window)
        ImGui::ShowStyleEditor();

    static bool coloursSet = false;
    if (!coloursSet) {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.18f, 0.34f, 0.73f, 0.40f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.12f, 0.30f, 0.84f, 0.23f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.03f, 0.00f, 0.07f, 0.50f);
        colors[ImGuiCol_Border]                 = ImVec4(0.70f, 0.23f, 1.00f, 0.50f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.00f, 0.16f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.72f, 0.26f, 0.98f, 0.40f);
        colors[ImGuiCol_Button]                 = ImVec4(0.55f, 0.26f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.72f, 0.26f, 0.98f, 0.40f);


        // Set frame rounding
        ImGui::GetStyle().FrameRounding = 4.0f;
        ImGui::GetStyle().PopupRounding = 6.0f;

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
    if(anim_progress == 1)
        once = true;

    A:

    // Update the title bar header
    std::string str = "Galaxy Simulation - " + std::to_string((int)ImGui::GetIO().Framerate) + " FPS" +
            " - Frame: " + std::to_string(ImGui::GetFrameCount());

    glfwSetWindowTitle(window, str.c_str());

    auto header_size = ImVec2(ImGui::GetIO().DisplaySize.x, globals->top_panel_height);
    windows::draw_header(
        ImVec2(// From -> To
                std::lerp(ImGui::GetIO().DisplaySize.x, 0, anim_progress),
                std::lerp(ImGui::GetIO().DisplaySize.y, 0, anim_progress)
        ),
        ImVec2(ImGui::GetIO().DisplaySize.x, header_size.y)
//        ImVec2(// From -> To
//                std::lerp(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.x, anim_progress),
//                std::lerp(0, header_size.y, anim_progress)
//        )
    );

    windows::draw_logs(
            ImVec2(0, ImGui::GetMainViewport()->Size.y - globals->bottom_panel_height),

            ImVec2(ImGui::GetMainViewport()->Size.x, globals->bottom_panel_height),
            ImGuiWindowFlags_NoTitleBar
    );

//    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - 120));
//    ImGui::ShowDemoWindow();

    windows::run_sim(
//        ImVec2(// From -> To
//                std::lerp(ImGui::GetIO().DisplaySize.x, 0, anim_progress),
//                std::lerp(ImGui::GetIO().DisplaySize.y, globals->top_panel_height, anim_progress)
//        ),
        ImVec2(0, globals->top_panel_height),

        ImVec2(globals->left_panel_width, ImGui::GetIO().DisplaySize.y - globals->top_panel_height - globals->bottom_panel_height),

        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse  |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar
    );

    windows::draw_graphics(
            ImVec2(globals->left_panel_width, globals->top_panel_height),

            ImVec2(ImGui::GetIO().DisplaySize.x - globals->left_panel_width, ImGui::GetIO().DisplaySize.y - globals->top_panel_height - globals->bottom_panel_height),

            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse  | ImGuiWindowFlags_NoTitleBar
    );

    DrawDots();
}
void cleanup() {
    printf("Cleaning up...\n");
    windows::clean_up();
    printf("Done cleaning up.\n");
}

#endif //GALAXYSIMULATIONUI_WINDOW_HANDLER_H

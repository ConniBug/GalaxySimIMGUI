//
// Created by Conni Bilham on 09/02/2023.
//

#ifndef GALAXYSIMULATIONUI_WINDOW_GRAPHICS_H
#define GALAXYSIMULATIONUI_WINDOW_GRAPHICS_H

#include "imgui.h"
#include <implot.h>
#include <cmath>
#include <iostream>

namespace windows {
    void draw_graphics(ImVec2 position, ImVec2 size, int flags) {
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(position);
        ImGui::Begin("Graphics", NULL, flags);

        ImGui::Text("Graphics");
        {
            ImPlot::ShowDemoWindow();
        }
        {
            static int animatedHighestFPS = 0;
            static int highestFps = 0;
            static float timeSince = 0;
            static int frameCount = 0;
            ++frameCount;
            timeSince += ImGui::GetIO().DeltaTime;

            int fps = frameCount / timeSince;
//            ImGui::Text("FPS: %d", fps);
//            ImGui::Text("Highest FPS: %d", highestFps);
//            ImGui::Text("Animated Highest FPS: %d", animatedHighestFPS);
//            ImGui::Text("Time since: %f", timeSince);
//            ImGui::Text("Frame count: %d", frameCount);
            if(timeSince > 5.0f) {
                timeSince = 0;
                frameCount = 0;
            }
            // Plot fps graph
            static int index = 0;
            static float values[300] = {0};
            if(timeSince > ImGui::GetIO().DeltaTime * 2)
                if(fps > 10)
                    values[index] = fps;

            if (values[index] > highestFps)
                highestFps = values[index] + 2;
            if (index > 300) {
                index = 0;
                highestFps = 0;
            } else
                ++index;

            static float smoothedValues[100] = {0};

            for (int i = 0; i < 100; ++i) {
                smoothedValues[i] = (values[i] + values[i + 1] + values[i + 2]) / 3;
            }

            animatedHighestFPS = std::lerp(animatedHighestFPS, highestFps, 50. / 100.);
//            std::cout << "animatedHighestFPS: " <<  animatedHighestFPS << std::endl;
//            ImGui::PlotHistogram("FPS", values, IM_ARRAYSIZE(values), index, "FPS", 0.0f, animatedHighestFPS, ImVec2(0, 80));
//            ImGui::PlotLines("FPS", smoothedValues, IM_ARRAYSIZE(smoothedValues), index, "FPS", 0.0f, highestFps, ImVec2(0, 80));
        }
        if (2 == 3){
            static int highestFps = 0;

            // Plot fps graph
            static int index = 0;
            static float values[120] = {0};
            int fps = 1;
            if (values[index] > highestFps)
                highestFps = values[index];
            if (index > 120 - 2) {
                index = 0;
                highestFps = 0;
            } else
                ++index;

            ImGui::PlotLines("FPS", values, IM_ARRAYSIZE(values), index, "FPS", 0.0f, 100.0f, ImVec2(0, 80));
        }

        struct Star_t {
            Vectorr position;
            Vectorr velocity;
            float mass;
        };

        static std::vector<Star_t> stars;

        if (ImGui::Button("Add Star"))
            stars.push_back({{0, 0, 0}, {0, 0, 0}, 1});
        ImGui::SameLine();
        if (ImGui::Button("Remove Star"))
            if (stars.size() > 0)
                stars.pop_back();
        ImGui::SameLine();
        ImGui::Text("Stars: %d", stars.size());

        static float xs1[1001], ys1[1001];
        for (int i = 0; i < 1001; ++i) {
            xs1[i] = i * 0.001f;
            ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
        }
        static double xs2[20], ys2[20];
        for (int i = 0; i < 20; ++i) {
            xs2[i] = i * 1/19.0f;
            ys2[i] = xs2[i] * xs2[i];
        }
        if (ImPlot::BeginPlot("Line Plots")) {
            ImPlot::SetupAxes("x","y");
            ImPlot::PlotLine("f(x)", xs1, ys1, 1001);
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::PlotLine("g(x)", xs2, ys2, 20,ImPlotLineFlags_Segments);
            ImPlot::EndPlot();
        }

        ImGui::End();
    }
} // Windows

#endif //GALAXYSIMULATIONUI_WINDOW_GRAPHICS_H

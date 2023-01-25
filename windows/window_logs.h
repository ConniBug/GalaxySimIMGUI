//
// Created by Conni Bilham on 25/01/2023.
//

#ifndef GALAXYSIMULATIONUI_WINDOW_LOGS_H
#define GALAXYSIMULATIONUI_WINDOW_LOGS_H

#include "LiveLogViewer.h"

namespace handler {
    namespace windows {
        void draw_logs(ImVec2 position, ImVec2 size) {
//            ImGui::SetNextWindowSize(size);
//            ImGui::SetNextWindowPos(position);

            static bool liveLogs = true;
            static widgets::LiveLogViewer logViewer;
            logViewer.logFile = "/Users/connibilham/CLionProject/GalaxySimIMGUI/cmake-build-debug/log.txt";
            logViewer.logFile = "/Users/connibilham/CLionProjects/galaxy-in-cpp/cmake-build-debug/log.txt";

            static bool once = false;
            if (!once) {
                once = true;

                pthread_t thr;
                pthread_create(&thr, NULL, logViewer.watch_thread, &logViewer);
            }
            logViewer.Draw("Live Logs", &liveLogs);
        }
    } // Windows
} // Handler
#endif //GALAXYSIMULATIONUI_WINDOW_LOGS_H

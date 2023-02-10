//
// Created by Conni Bilham on 25/01/2023.
//

#ifndef GALAXYSIMULATIONUI_WINDOW_LOGS_H
#define GALAXYSIMULATIONUI_WINDOW_LOGS_H

#include "LiveLogViewer.h"

namespace windows {
    void draw_logs(ImVec2 position, ImVec2 size, int flags) {
//            ImGui::SetNextWindowSize(size);
//            ImGui::SetNextWindowPos(position);

        static bool liveLogs = true;
        static widgets::LiveLogViewer* logViewer = new widgets::LiveLogViewer();

        static bool once = false;
        if (!once) { once = true;
            pthread_t thr;
            pthread_create(&thr, NULL, logViewer->watch_thread, (void*)&logViewer);
            logViewer->threadActive = true;
            logViewer->addLog("Watch Thread Starting");
        }
        logViewer->Draw("Live Logs", position, size, flags);
    }
} // Windows

#endif //GALAXYSIMULATIONUI_WINDOW_LOGS_H

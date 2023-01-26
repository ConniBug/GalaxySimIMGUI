//
// Created by Conni Bilham on 26/01/2023.
//
#pragma once

#ifndef GALAXYSIMULATIONUI_GLOBALS_H
#define GALAXYSIMULATIONUI_GLOBALS_H

#include "imgui.h"
#include <string>

struct Globals {
    bool show_demo_window    = true;
    bool show_another_window = true;

    std::string* active_log_file = new std::string("initial");
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    void* logViewer = nullptr;
};

extern Globals* globals;

#endif //GALAXYSIMULATIONUI_GLOBALS_H

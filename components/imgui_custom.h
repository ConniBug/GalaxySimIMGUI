//
// Created by Conni Bilham on 25/01/2023.
//

#ifndef GALAXYSIMULATIONUI_IMGUI_CUSTOM_H
#define GALAXYSIMULATIONUI_IMGUI_CUSTOM_H

#include "imgui.h"
#include <Vector3.h>

namespace ImGui {
    namespace Custom {
        namespace helper {

        }
        // Stolen from imgui_demo.cpp
        //
        // Helper to display a little (?) mark which shows a tooltip when hovered.
        // In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
        static void HelpMarker(const char* desc, float window_rounding = 6.f, const char* text = "(?)")
        {
            // Set rounding
            float initial_rounding = ImGui::GetStyle().WindowRounding;
            ImGui::GetStyle().WindowRounding = window_rounding;

            ImGui::SameLine();
            ImGui::TextDisabled(text);
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                ImGui::TextUnformatted(desc);
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            ImGui::GetStyle().WindowRounding = initial_rounding;
        }

        bool DropdownButton(ImVec2 buttonSize) {
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            float x = ImGui::GetCursorPosX();
            float y = ImGui::GetCursorPosY();

            // Arrow
            fVector center(window->Pos.x + x + 10, window->Pos.y + y + buttonSize.y / 2);
            center.y -= window->Scroll.y;
            float r = 8.f;
            center.y -= r * 0.25f;
            fVector a = center + fVector(0, 1) * r;
            fVector b = center + fVector(-0.866f, -0.5f) * r;
            fVector c = center + fVector(0.866f, -0.5f) * r;

            window->DrawList->AddTriangleFilled(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImVec2(c.x, c.y), ImGui::GetColorU32(ImGuiCol_Text));
            return ImGui::ButtonEx("##9878901", buttonSize, ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_NoHoveredOnFocus);
        }
        void CentreTextLocal(const char* text, int spaceWidth) {
            // TODO: Optimise this
            auto text_Size = ImGui::CalcTextSize(text);
            auto cursorPos = ImGui::GetCursorPos();

            ImGui::SetCursorPos(ImVec2(cursorPos.x + ((spaceWidth - text_Size.x) / 2) - 10, cursorPos.y));
            ImGui::Text(text);
            ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + text_Size.y + 5));
        }

        void InputVector3(const char* label, Vector_t<float>* vec, const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
            ImGui::PushID(label);
            ImGui::PushItemWidth(ImGui::CalcItemWidth() / 3.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

            ImGui::AlignTextToFramePadding();
            ImGui::Text(label);

            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);


        ImGui::SetNextItemWidth(ImGui::CalcItemWidth());
            ImGui::InputFloat("##x214", &vec->x, 0.0f, 0.0f, format, flags);

            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);


        ImGui::SetNextItemWidth(ImGui::CalcItemWidth());
            ImGui::InputFloat("##y436", &vec->y, 0.0f, 0.0f, format, flags);

            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);


        ImGui::SetNextItemWidth(ImGui::CalcItemWidth());
            ImGui::InputFloat("##z586", &vec->z, 0.0f, 0.0f, format, flags);

            ImGui::PopStyleVar();
            ImGui::PopItemWidth();
            ImGui::PopID();

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
        }
    }
}
#endif //GALAXYSIMULATIONUI_IMGUI_CUSTOM_H

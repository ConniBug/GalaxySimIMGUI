//
// Created by Conni Bilham on 24/01/2023.
//

#include "LiveLogViewer.h"
#include <unistd.h>
#include <regex>
#include <iostream>

std::string getFromRegex(std::string s, std::string regex, int start = 0, int trim = 0) {
    s = s.substr(start, s.size());

    std::regex word_regex(regex, std::regex_constants::icase);
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        return match_str.substr(0, match_str.size() -    trim);
    }
    std::cout << "No match found - " << s << std::endl;
    return "";
}

namespace handler {
    namespace windows {
        namespace widgets {
            char *LiveLogViewer::Strdup(const char *s) {
                IM_ASSERT(s);
                size_t len = strlen(s) + 1;
                void* buf = malloc(len);
                IM_ASSERT(buf);
                return (char*)memcpy(buf, (const void*)s, len);
            }
            LiveLogViewer::LiveLogViewer() {
                ClearLog();
                AutoScroll = true;
                ScrollToBottom = false;
                Filter.Clear();
            }
            LiveLogViewer::~LiveLogViewer() {
                ClearLog();
            }

            void* LiveLogViewer::watch_thread(void *arg1) {
                printf("Log file watch_thread started - LiveLogViewer Memory Address: %p\n", arg1);

                // Get the log viewer reference from the memory address passed in as an argument
                widgets::LiveLogViewer *logViewer = (widgets::LiveLogViewer *) arg1;

                // Open file for reading
                FILE *fp = fopen(logViewer->logFile.c_str(), "r");
                if (fp == NULL) {
                    printf("Error opening file: %sbytes\n", logViewer->logFile.c_str());
                    return NULL;
                }

                // Get file size
                fseek(fp, 0L, SEEK_END);
                long int lastSize = ftell(fp);
                printf("Initial log file size: %ldbytes\n", lastSize);

                while (true) {
                    usleep(200 * 1000);

                    // Get file size
                    fseek(fp, 0L, SEEK_END);
                    long int currentSize = ftell(fp);

                    // If size changed
                    if(lastSize != currentSize) {
                        logViewer->addLog("[D] File size has changed: %ldb -> %ld - [ %ldb ]\n", lastSize, currentSize, currentSize - lastSize);
                        printf("File size has changed: %ldb -> %ldb - [ %ldb ]\n", lastSize, currentSize, currentSize - lastSize);

                        // Seek to last read position
                        fseek(fp, lastSize, SEEK_SET);

                        std::string lines[10];
                        char buf[1024];
                        while (fgets(buf, sizeof(buf), fp) != NULL) {
                            int line_count = 0;

                            for(int cnt = 0; cnt <= 1024; ++cnt) {

                                if(buf[cnt] == '\n') {
//                                    printf("Line %d: %s\n", line_count, lines[line_count].c_str());
                                    logViewer->addLog("[N] %s\n", lines[line_count].c_str());
                                    logViewer->addLog("called - %i", line_count);
                                    line_count++;
                                    continue;
                                }
                                if(buf[cnt] == '\u0000') {
                                    break;
                                }
                                lines[line_count] += buf[cnt];
                            }
                        }

                        // Loop through lines
//                        for(auto line : lines) {
//                            if(line.empty()) {
//                                continue;
//                            }
//                            auto s = line;
//                            auto time     = getFromRegex(s, " ..:..:.. "); int cnt = time.size() + 7;
//                            if(time.size() == 0) {
//                                printf("%s", s.c_str());
//                                logViewer->addLog("%s", s.c_str());
//                                continue;
//                            }
//                            auto logLevel = getFromRegex(s, "[A-Z]* \\]", cnt, 2); cnt += logLevel.size() + 7;
//                            auto thread   = getFromRegex(s, "[0-9]*\\]",  cnt, 1); cnt += thread.size() + 8;
//                            auto type     = getFromRegex(s, "^[^-]*", cnt, 1); cnt += type.size() + 3;
//
//                            s = s.substr(cnt, s.size());
//
//
//                            logViewer->addLog("[%s] [ %s ] [ Thread %s ] [ %s ] - %s", time.c_str(), logLevel.c_str(), thread.c_str(), type.c_str()), s.c_str();
//                            printf("[%s] [ %s ] [ Thread %s ] [ %s ] - %s\n", time.c_str(), logLevel.c_str(), thread.c_str(), type.c_str(), s.c_str());
//
//                        }
                        lastSize = currentSize;
                    }
                }

            }

            void LiveLogViewer::ClearLog() {
                for (int i = 0; i < logRecords.Size; i++)
                    free(logRecords[i]);
                logRecords.clear();
            }

            void LiveLogViewer::addLog(const char *fmt, ...) {
                // FIXME-OPT
                char buf[1024];
                va_list args;
                va_start(args, fmt);
                vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
                buf[IM_ARRAYSIZE(buf)-1] = 0;
                va_end(args);
                logRecords.push_back(Strdup(buf));
            }

            void LiveLogViewer::Draw(const char *title, bool *p_open) {
                ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
                if (!ImGui::Begin(title, p_open)) {
                    ImGui::End();
                    return;
                }

                // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
                // So e.g. IsItemHovered() will return true when hovering the title bar.
                // Here we create a context menu only available from the title bar.
                if (ImGui::BeginPopupContextItem()){
                    if (ImGui::MenuItem("Close Console Test Text"))
                        *p_open = false;
                    ImGui::EndPopup();
                }

                // TODO: display items starting from the bottom

                if (ImGui::SmallButton("Add Debug Text"))  {
                    addLog("%d some text", logRecords.Size);
                    addLog("some more text");
                    addLog("display very important message here!"); }
                ImGui::SameLine();
                if (ImGui::SmallButton("Add Debug Error")) { addLog("[error] something went wrong"); }
                ImGui::SameLine();
                if (ImGui::SmallButton("Clear"))           { ClearLog(); }
                ImGui::SameLine();
                bool copy_to_clipboard = ImGui::SmallButton("Copy");
//                    static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

                ImGui::Separator();

                // Options menu
                if (ImGui::BeginPopup("Options"))
                {
                    ImGui::Checkbox("Auto-scroll", &AutoScroll);
                    ImGui::EndPopup();
                }

                // Options, Filter
                if (ImGui::Button("Options"))
                    ImGui::OpenPopup("Options");
                ImGui::SameLine();
                Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
                ImGui::Separator();

                // Reserve enough left-over height for 1 separator + 1 input text
                const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
                {
                    if (ImGui::BeginPopupContextWindow())
                    {
                        if (ImGui::Selectable("Clear")) ClearLog();
                        ImGui::EndPopup();
                    }

                    // Display every line as a separate entry so we can change their color or add custom widgets.
                    // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
                    // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
                    // to only process visible items. The clipper will automatically measure the height of your first item and then
                    // "seek" to display only items in the visible area.
                    // To use the clipper we can replace your standard loop:
                    //      for (int i = 0; i < Items.Size; i++)
                    //   With:
                    //      ImGuiListClipper clipper;
                    //      clipper.Begin(Items.Size);
                    //      while (clipper.Step())
                    //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                    // - That your items are evenly spaced (same height)
                    // - That you have cheap random access to your elements (you can access them given their index,
                    //   without processing all the ones before)
                    // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
                    // We would need random-access on the post-filtered list.
                    // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
                    // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
                    // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
                    // to improve this example code!
                    // If your items are of variable height:
                    // - Split them into same height items would be simpler and facilitate random-seeking into your list.
                    // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
                    if (copy_to_clipboard)
                        ImGui::LogToClipboard();
                    for (int i = 0; i < logRecords.Size; i++)
                    {
                        const char* item = logRecords[i];
                        if (!Filter.PassFilter(item))
                            continue;

                        // Normally you would store more information in your item than just a string.
                        // (e.g. make Items[] an array of structure, store color/type etc.)
                        ImVec4 color;
                        bool has_color = false;
                        if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
                        else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
                        if (has_color)
                            ImGui::PushStyleColor(ImGuiCol_Text, color);
                        ImGui::TextUnformatted(item);
                        if (has_color)
                            ImGui::PopStyleColor();
                    }
                    if (copy_to_clipboard)
                        ImGui::LogFinish();

                    // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
                    // Using a scrollbar or mouse-wheel will take away from the bottom edge.
                    if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                        ImGui::SetScrollHereY(1.0f);
                    ScrollToBottom = false;

                    ImGui::PopStyleVar();
                }
                ImGui::EndChild();

                ImGui::End();
            }

        } // handler
    } // windows
} // widgets
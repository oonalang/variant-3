// ===========================================================================
// ImguiMenuTheme.h - Theme/values/animation core ported verbatim from
// https://github.com/eliwoahzja/imguimenu (src/menu.h).
// Only adapted to the project backend: ImGui feature-check guards (this
// project ships ImGui 1.71) instead of the DX11 host's assumptions.
// Fonts stay project-owned (Fonts/*.h): the reference repo's own "asset
// setup" only declares a 15px UI font and vector icons - both are ported.
// ===========================================================================
#pragma once

#include "imgui.h"
#include "imgui_internal.h"

namespace imguimenu
{
    // ------------------------------------------------------------ constants
    static const ImU32 ACCENT   = IM_COL32(10, 132, 255, 255);
    static const ImU32 GREEN    = IM_COL32(48, 209, 88, 255);
    static const ImU32 COL_TEXT = IM_COL32(245, 245, 247, 255);
    static const ImU32 DIM      = IM_COL32(235, 235, 245, 150);
    static const ImU32 FILL     = IM_COL32(255, 255, 255, 22);
    static const ImU32 LINE     = IM_COL32(255, 255, 255, 38);
    static constexpr float ROW_H = 37.f;
    static constexpr float PAD   = 14.f;

    // ------------------------------------------------------------ helpers
    inline ImU32 LerpCol(ImU32 a, ImU32 b, float t) {
        ImVec4 x = ImGui::ColorConvertU32ToFloat4(a), y = ImGui::ColorConvertU32ToFloat4(b);
        return ImGui::ColorConvertFloat4ToU32(ImVec4(x.x + (y.x - x.x) * t, x.y + (y.y - x.y) * t,
                                                     x.z + (y.z - x.z) * t, x.w + (y.w - x.w) * t));
    }
    inline float Anim(ImGuiID id, bool target, float speed = 14.f) {
        ImGuiStorage* st = ImGui::GetStateStorage();
        float v = st ? st->GetFloat(id, target ? 1.f : 0.f) : (target ? 1.f : 0.f);
        v += ((target ? 1.f : 0.f) - v) * ImMin(1.f, ImGui::GetIO().DeltaTime * speed);
        if (st) st->SetFloat(id, v);
        return v;
    }
    inline void Chevron(ImDrawList* dl, ImVec2 c, bool up, ImU32 col) {
        float d = up ? 1.f : -1.f;
        ImVec2 p[3] = { { c.x - 3.f, c.y + 1.5f * d }, { c.x, c.y - 1.5f * d }, { c.x + 3.f, c.y + 1.5f * d } };
        dl->AddPolyline(p, 3, col, ImDrawFlags_None, 1.5f);
    }

    // ------------------------------------------------------------ style
    inline void Style() {
        ImGuiStyle& st = ImGui::GetStyle();
        st.PopupRounding = 10.f; st.PopupBorderSize = 1.f;
        st.FrameRounding = 8.f; st.WindowRounding = 14.f;
        ImVec4* c = st.Colors;
        c[ImGuiCol_Text]          = ImVec4(0.96f, 0.96f, 0.97f, 1.f);
        c[ImGuiCol_PopupBg]       = ImVec4(0.11f, 0.11f, 0.14f, 0.96f);
        c[ImGuiCol_Border]        = ImVec4(1.f, 1.f, 1.f, 0.14f);
        c[ImGuiCol_Header]        = ImVec4(0.04f, 0.52f, 1.f, 0.55f);
        c[ImGuiCol_HeaderHovered] = ImVec4(0.04f, 0.52f, 1.f, 0.75f);
        c[ImGuiCol_HeaderActive]  = ImVec4(0.04f, 0.52f, 1.f, 0.90f);
        c[ImGuiCol_FrameBg]        = ImVec4(1.f, 1.f, 1.f, 0.11f);
        c[ImGuiCol_FrameBgHovered] = ImVec4(1.f, 1.f, 1.f, 0.16f);
        c[ImGuiCol_FrameBgActive]  = ImVec4(1.f, 1.f, 1.f, 0.20f);
    }
} // namespace imguimenu



#pragma once

#include "ImguiMenuWidgets.h"
#include "ImguiMenuTheme.h"
#include "ImguiMenuRenderPages.h"
#include "../../ImGui/runtime_preview_menu.h"

extern bool HIDEESP;
extern float SnowBsize;
extern float speedHackMultiplier;
extern float jumpHeightMultiplier;
extern float SlideRange;
extern bool dark;
extern bool isMenuVisible;
extern bool windowCollapsed;
extern float tabAlpha;
extern int page;
extern int activeTab;
extern bool g_LogoPreviewMode;
extern int skinSubTab;

namespace imguimenu
{

    struct State
    {
        int tab = 0;
        char search[64] = {};

        bool  aim_enabled     = true;
        bool  visible_only    = true;
        bool  team_check      = false;
        bool  draw_fov        = true;
        float smoothing       = 6.f;
        float fov             = 120.f;
        int   hitbox          = 0;
        ImGuiKey aim_key      = ImGuiKey_MouseRight;
        int   preset          = 0;

        bool  esp_enabled     = true;
        bool  box             = true;
        bool  name            = true;
        bool  health          = true;
        bool  distance        = false;
        bool  skeleton        = false;
        float box_col[3]      = { 0.04f, 0.52f, 1.00f };
        float name_col[3]     = { 0.95f, 0.95f, 0.97f };
        float skel_col[3]     = { 1.00f, 0.23f, 0.19f };
        float max_dist        = 1400.f;
        int   box_style       = 0;
        int   font_size       = 12;
    };

    inline void AimPage(State& s, float childH = 420.f)
    {
        static const char* hitboxes[] = { "Head", "Neck", "Chest" };
        static const char* presets[]  = { "Legit", "Semi", "Custom" };
        static const char* styles[]   = { "Corners", "Full", "Filled" };
        const float colw = 306.f;
        ImGui::SetCursorPos(ImVec2(25.f, 87.f));

        ImGui::BeginChild("##col1", ImVec2(colw, childH), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        {
            Card cd; BeginCard(cd, "AIMBOT");
            { auto r = Row("Enabled", true);        Switch(r, &s.aim_enabled); }
            { auto r = Row("Visible only");         Switch(r, &s.visible_only); }
            { auto r = Row("Team check");           Switch(r, &s.team_check); }
            { auto r = Row("Smoothing");            Slider(r, &s.smoothing, 1.f, 10.f, "%.1f"); }
            { auto r = Row("FOV");                  Slider(r, &s.fov, 1.f, 360.f, "%.0f", true); }
            { auto r = Row("Hitbox");               Segmented(r, hitboxes, 3, &s.hitbox); }
            { auto r = Row("Aim key");              KeyBind(r, (int*)&s.aim_key); }
            { auto r = Row("Draw FOV circle");      Switch(r, &s.draw_fov); }
            { auto r = Row("Preset");               Combo(r, presets, 3, &s.preset); }
            EndCard(cd, "Smoothing and FOV apply while the aim key is held.");
        }
        ImGui::EndChild();
        ImGui::SameLine(0.f, 17.f);

        ImGui::BeginChild("##col2", ImVec2(colw, childH), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        {
            Card cd; BeginCard(cd, "ESP");
            { auto r = Row("Enabled", true);        Switch(r, &s.esp_enabled); }
            { auto r = Row("Box");                  Switch(r, &s.box);       ColorDot(r, s.box_col); }
            { auto r = Row("Name");                 Switch(r, &s.name);      ColorDot(r, s.name_col); }
            { auto r = Row("Health bar");           Switch(r, &s.health); }
            { auto r = Row("Distance");             Switch(r, &s.distance); }
            { auto r = Row("Skeleton");             Switch(r, &s.skeleton);  ColorDot(r, s.skel_col); }
            { auto r = Row("Max distance");         Slider(r, &s.max_dist, 0.f, 2000.f, "%.0f", true); }
            { auto r = Row("Box style");            Combo(r, styles, 3, &s.box_style); }
            { auto r = Row("Font size");            Stepper(r, &s.font_size, 8, 32); }
            EndCard(cd, "Colors apply to every enabled overlay.");
        }
        ImGui::EndChild();
    }
    inline void DataSkinsPage(float contentW, float childH)
    {
        ImGui::SetCursorPos(ImVec2(25.f, 87.f));
        const runtime_preview_menu::ChildFrame skinChild =
            runtime_preview_menu::BeginContentChild("SKINS##IMGUIMENU", ImVec2(contentW, childH));
        {
            {
                auto r = Row("Unlock Blueprints", true);
                Switch(r, &Config.ExtraMenu.Blueprints);
            }
            RenderSkinCategoryContent(skinSubTab, true);
        }
        runtime_preview_menu::EndContentChild(skinChild);
    }

    inline void DataMiscPage(float contentW, float childH)
    {
        ImGui::SetCursorPos(ImVec2(25.f, 87.f));
        const runtime_preview_menu::ChildFrame misc = runtime_preview_menu::BeginContentChild("MISC##IMGUIMENU", ImVec2(contentW, childH));
        const ::misc_tab::LayoutMetrics layout = ::misc_tab::CalculateLayout(contentW, runtime_preview_menu::g_activeChangelogTab);
        {
            const runtime_preview_menu::ChildFrame changelog = runtime_preview_menu::BeginContentChild("CHANGELOG##IMGUIMENU",
                ImVec2(contentW, layout.changelogHeight), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            const float tabGap = 5.0f;
            const float tabH = 40.0f;
            const float safeRight = ImGui::GetStyle().ScrollbarSize + 12.0f;
            const float availW = ImMax(1.0f, ImGui::GetContentRegionAvail().x - safeRight);
            const float tabW = ImMax(1.0f, ImFloor((availW - tabGap * 2.0f) / 3.0f));
            for (int i = 0; i < IM_ARRAYSIZE(::misc_tab::kChangelogTabs); ++i) {
                char buttonId[48] = {};
                std::snprintf(buttonId, sizeof(buttonId), "##imguimenu_changelog_%d", i);
                if (i > 0) ImGui::SameLine(0.0f, tabGap);
                if (::misc_tab::DrawChangelogTab(buttonId, ::misc_tab::kChangelogIcons[i], ::misc_tab::kChangelogTabs[i],
                    runtime_preview_menu::g_activeChangelogTab == i, ImVec2(tabW, tabH)))
                    runtime_preview_menu::g_activeChangelogTab = i;
            }
            ::misc_tab::ContentGap(12.0f);
            const ::misc_tab::ChangelogSelection activeSection =
                ::misc_tab::GetActiveChangelogSelection(runtime_preview_menu::g_activeChangelogTab);
            ImFont* iconFont = F107 ? F107 : ImGui::GetFont();
            ImGui::PushFont(iconFont);
            ImGui::TextColored(activeSection.color, "%s", activeSection.icon);
            ImGui::PopFont();
            ImGui::SameLine(0.0f, 8.0f);
            ImGui::TextColored(activeSection.color, "%s", activeSection.title);
            ImGui::SameLine(0.0f, 10.0f);
            ImGui::TextColored(c::text::text, "March 10, 2026");
            ::misc_tab::ContentGap(8.0f);
            ::misc_tab::DrawSectionItems(activeSection.items, activeSection.count);
            runtime_preview_menu::EndContentChild(changelog);
        }
        ::misc_tab::ContentGap(layout.rowGap);
        {
            const runtime_preview_menu::ChildFrame info = runtime_preview_menu::BeginContentChild("INFO##IMGUIMENU",
                ImVec2(contentW, layout.infoHeight), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ::misc_tab::DrawInfoSummary();
            runtime_preview_menu::EndContentChild(info);
        }
        ::misc_tab::ContentGap(layout.rowGap);
        {
            const runtime_preview_menu::ChildFrame price = runtime_preview_menu::BeginContentChild("PRICELIST##IMGUIMENU",
                ImVec2(contentW, layout.priceHeight), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            ::misc_tab::DrawPriceSummary("##imguimenu_misc_prices");
            runtime_preview_menu::EndContentChild(price);
        }
        runtime_preview_menu::EndContentChild(misc);
    }

    inline void DataConfigPage(float contentW, float childH)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        const float colGap = ImClamp(contentW * 0.022f, 12.f, 18.f);
        const float rowGap = 6.f;
        const float leftW = ImMax(0.f, ImFloor((contentW - colGap) * 0.5f));
        const float rightW = ImMax(0.f, contentW - colGap - leftW);
        const float topH = ImMax(0.f, ImFloor((childH - rowGap) * 0.5f));
        const float botH = ImMax(0.f, childH - rowGap - topH);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, rowGap));
        ImGui::SetCursorPos(ImVec2(25.f, 87.f));
        {
            const runtime_preview_menu::ChildFrame licenseChild = runtime_preview_menu::BeginContentChild("LICENSE INFO##IMGUIMENU", ImVec2(leftW, topH));
            ::settings_tab::RenderLicenseCard();
            runtime_preview_menu::EndContentChild(licenseChild);
            const runtime_preview_menu::ChildFrame logoChild = runtime_preview_menu::BeginContentChild("LOGO SETTINGS##IMGUIMENU", ImVec2(leftW, botH));
            ::settings_tab::RenderLogoCard();
            runtime_preview_menu::EndContentChild(logoChild);
        }
        ImGui::SameLine(0.f, colGap);
        {
            const runtime_preview_menu::ChildFrame configChild = runtime_preview_menu::BeginContentChild("CONFIG MANAGEMENT##IMGUIMENU", ImVec2(rightW, topH));
            ::settings_tab::RenderConfigCard();
            runtime_preview_menu::EndContentChild(configChild);
            const runtime_preview_menu::ChildFrame enhChild = runtime_preview_menu::BeginContentChild("ENHANCEMENT##IMGUIMENU", ImVec2(rightW, botH));
            ::settings_tab::RenderEnhancementCard();
            runtime_preview_menu::EndContentChild(enhChild);
        }
        ImGui::PopStyleVar();
    }


    inline void Render(State& s, bool* open = nullptr)
    {
        const ImVec2 size(880.f, 560.f);
        ImGuiIO& io = ImGui::GetIO();
        if (io.Fonts == nullptr || io.Fonts->Fonts.empty() || io.DisplaySize.x <= 0.f || io.DisplaySize.y <= 0.f) {
            return;
        }
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                                ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 14.f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const ImGuiWindowFlags wf = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
                                    ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;
        ImGui::Begin("##imguimenu", nullptr, wf);
        ImDrawList* dl = ImGui::GetWindowDrawList();
        const ImVec2 wp = ImGui::GetWindowPos();
        DrawBackground(dl, wp, size);

        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::BeginChild("##sb", ImVec2(200.f, size.y), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        {
            ImGui::SetCursorPos(ImVec2(11.f, 19.f));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(34.f, 6.5f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 9.f);
            ImGui::SetNextItemWidth(178.f);
            ImGui::InputTextWithHint("##search", "Search", s.search, sizeof s.search);
            ImVec2 a = ImGui::GetItemRectMin(), b = ImGui::GetItemRectMax();
            ImGui::PopStyleVar(2);
            ImVec2 ic(a.x + 16.f, (a.y + b.y) * 0.5f - 1.f);
            ImDrawList* cdl = ImGui::GetWindowDrawList();
            cdl->AddCircle(ic, 4.5f, DIM, 0, 1.5f);
            cdl->AddLine(ImVec2(ic.x + 3.5f, ic.y + 3.5f), ImVec2(ic.x + 7.f, ic.y + 7.f), DIM, 1.5f);

            struct T { const char* n; int icon; ImU32 c; };
            static const T tabs[] = {
                { "Aim",     0, IM_COL32(10, 132, 255, 255) },
                { "Visuals", 1, IM_COL32(50, 173, 230, 255) },
                { "Players", 2, IM_COL32(191, 90, 242, 255) },
                { "Skins",   3, IM_COL32(255, 159, 10, 255) },
                { "Misc",    4, IM_COL32(255, 159, 10, 255) },
                { "Config",  5, IM_COL32(142, 142, 147, 255) },
            };
            for (int i = 0; i < 6; i++) {
                ImGui::SetCursorPos(ImVec2(11.f, 61.f + i * 32.f));
                ImU32 col = (i == 0 && s.tab == 0) ? IM_COL32(255, 255, 255, 40) : tabs[i].c;
                if (Tab(tabs[i].n, tabs[i].icon, col, s.tab == i, 178.f)) s.tab = i;
            }
            cdl->AddCircleFilled(ImVec2(wp.x + 30.f, wp.y + size.y - 26.f), 4.5f, GREEN, 16);
            char fps[16]; snprintf(fps, sizeof fps, "%.0f FPS", io.Framerate);
            cdl->AddText(ImVec2(wp.x + 44.f, wp.y + size.y - 34.f), DIM, fps);
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(200.f, 0));
        ImGui::BeginChild("##content", ImVec2(680.f, size.y), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
        {
            ImDrawList* cdl = ImGui::GetWindowDrawList();
            ImVec2 cp = ImGui::GetWindowPos();
            static const char* titles[] = { "Aim", "Visuals", "Players", "Skins", "Misc", "Config" };
            static const char* subs[] = {
                "Target selection and assist behavior", "Overlay and rendering options",
                "Player list and filters", "Unlock and equip skins",
                "Miscellaneous options", "Save and load profiles"
            };
            int pageCursor = ImClamp(s.tab, 0, 5);
            cdl->AddText(ImGui::GetFont(), 30.f, ImVec2(cp.x + 25.f, cp.y + 22.f), COL_TEXT, titles[pageCursor]);
            cdl->AddText(ImGui::GetFont(), 30.f, ImVec2(cp.x + 25.7f, cp.y + 22.f), COL_TEXT, titles[pageCursor]);
            cdl->AddText(ImGui::GetFont(), 16.f, ImVec2(cp.x + 25.f, cp.y + 56.f), DIM, subs[pageCursor]);

            struct WB { float px; ImU32 col; bool close; };
            static const WB wbs[] = {
                { 638.f, IM_COL32(255, 189, 46, 255), false },
                { 658.f, IM_COL32(255, 69, 58, 255), true },
            };
            for (int wi = 0; wi < 2; ++wi) {
                const WB& btn = wbs[wi];
                ImVec2 ctr(cp.x + btn.px, cp.y + 21.f);
                ImGui::SetCursorScreenPos(ImVec2(ctr.x - 9.f, ctr.y - 9.f));
                ImGui::PushID(wi);
                bool hit = ImGui::InvisibleButton("wb", ImVec2(18.f, 18.f));
                ImGui::PopID();
                cdl->AddCircleFilled(ctr, 8.f, btn.col, 20);
                if (btn.close) {
                    cdl->AddLine(ImVec2(ctr.x - 3.f, ctr.y - 3.f), ImVec2(ctr.x + 3.f, ctr.y + 3.f), IM_COL32_WHITE, 1.8f);
                    cdl->AddLine(ImVec2(ctr.x + 3.f, ctr.y - 3.f), ImVec2(ctr.x - 3.f, ctr.y + 3.f), IM_COL32_WHITE, 1.8f);
                    if (hit && open) {
                        *open = false;
                        windowCollapsed = true;
                        isMenuVisible = false;
                    }
                } else {
                    cdl->AddLine(ImVec2(ctr.x - 3.5f, ctr.y), ImVec2(ctr.x + 3.5f, ctr.y), IM_COL32_WHITE, 1.8f);
                }
            }

            const float contentW = 630.f;
            const float childH = 420.f;
            if (pageCursor == 0) data::VisualsPage(contentW, childH);
            else if (pageCursor == 1) data::PlayersPage(contentW, childH);
            else if (pageCursor == 2) data::AimbotPage(contentW, childH);
            else if (pageCursor == 3) DataSkinsPage(contentW, childH);
            else if (pageCursor == 4) DataMiscPage(contentW, childH);
            else if (pageCursor == 5) DataConfigPage(contentW, childH);

            ImVec2 hc(cp.x + 25.f + 11.f, cp.y + size.y - 38.f);
            cdl->AddCircleFilled(hc, 14.f, IM_COL32(120, 120, 128, 140), 24);
            ImVec2 ts = ImGui::CalcTextSize("?");
            cdl->AddText(ImVec2(hc.x - ts.x * 0.5f, hc.y - ts.y * 0.5f), COL_TEXT, "?");
        }
        ImGui::EndChild();
        ImGui::End();
        ImGui::PopStyleVar(4);
    }

}

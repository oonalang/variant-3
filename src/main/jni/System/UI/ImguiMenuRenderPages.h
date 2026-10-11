// ===========================================================================
// ImguiMenuRenderPages.h - per-page content wiring (widgets -> Config).
// Depends on: ImguiMenuWidgets.h + SaveConfig.h (Config) + Misc/Settings modules.
// ===========================================================================
#pragma once
#include "ImguiMenuTheme.h"
#include "ImguiMenuWidgets.h"
#include "imgui_internal.h"

extern bool HIDEESP;
extern float SnowBsize;
extern float speedHackMultiplier;
extern float jumpHeightMultiplier;
extern float SlideRange;

// Standalone syntax-check support only: in the real build SaveConfig.h ->
// Init_Hacks.h declares these enums (incl. LineTarget/EAim*), so this block
// is inert there.
#if defined(IMGUI_MENU_STANDALONE) && !defined(IMGUI_MENU_STANDALONE_ENUMS)
#define IMGUI_MENU_STANDALONE_ENUMS
enum EspBoxType { Fill = 0, Outline = 1, Corner = 2, ThreeD = 3 };
enum EspHealthPosition { HealthTop = 0, HealthSide = 1 };
enum EspStyleTarget { EspStyleNone = 0, EspStyle3DSphere = 1, EspStylePlayerSignal = 2 };
enum EAim { DistanceAim = 0, CrosshairAim = 1 };
enum EAimTarget { Heads = 0, Chests = 1, Body = 2 };
enum EAimTrigger { NoneTrig = 0, Shooting = 1, Scoping = 2 };
enum LineTarget { Top = 0, Mid = 1, Bottom = 2 };
#endif

namespace imguimenu
{
namespace data
{
        // The reuse map: every activeTab page body from the old render block,
        // re-rendered through imguimenu widgets with the same pointers/enums.
        inline void VisualsPage(float contentW, float childH)
        {
            const float gap = 17.0f;
            const float colW = ImFloor((contentW - gap) * 0.5f);
            ImGui::SetCursorPos(ImVec2(25.f, 87.f));
            ImGui::BeginChild("##ivis1", ImVec2(colW, childH), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
            {
                Card cd; BeginCard(cd, "ESP");
                { auto r = Row("ESP Line", true);      Switch(r, &Config.ESPMenu.isPlayerLine); }
                { auto r = Row("ESP Box");             Switch(r, &Config.ESPMenu.Box); }
                { auto r = Row("ESP Skeleton");        Switch(r, &Config.ESPMenu.Skeleton); }
                { auto r = Row("ESP Health");          Switch(r, &Config.ESPMenu.Health); }
                { auto r = Row("ESP Name");            Switch(r, &Config.ESPMenu.Name); }
                { auto r = Row("ESP Distance");        Switch(r, &Config.ESPMenu.Distance); }
                { auto r = Row("ESP Count");           Switch(r, &Config.ESPMenu.Count); }
                { auto r = Row("360 Alert");           Switch(r, &Config.ESPMenu.Alert); }
                { auto r = Row("Show AimLine");        Switch(r, &Config.ESPMenu.Aimline); }
                { auto r = Row("Hide Stream");         Switch(r, &HIDEESP); }
                { auto r = Row("Yellow Wallhack");     Switch(r, &Config.ExtraMenu.WallHack); }
                { auto r = Row("Red Wallhack");        Switch(r, &Config.ExtraMenu.RedWallhack); }
                EndCard(cd);
            }
            ImGui::EndChild();
            ImGui::SameLine(0.f, gap);
            ImGui::BeginChild("##ivis2", ImVec2(colW, childH), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
            {
                Card cd; BeginCard(cd, "DISPLAY OPTIONS");
                static const char *boxTypes[] = { "Fill", "Outline", "Corner", "3D" };
                static const char *linePositions[] = { "Top", "Mid", "Bottom" };
                static const char *healthPositions[] = { "Top", "Side" };
                static const char *espStyles[] = { "None", "3D Sphere", "Player Signal" };
                int boxType = (int)Config.ESPMenu.BoxType;
                int target = (int)Config.ESPMenu.Target;
                int healthPos = (int)Config.ESPMenu.HealthPosition;
                int style = (int)Config.ESPMenu.EspStyle;
                { auto r = Row("Box Type");      Combo(r, boxTypes, IM_ARRAYSIZE(boxTypes), &boxType); }
                { auto r = Row("Line Position"); Combo(r, linePositions, IM_ARRAYSIZE(linePositions), &target); }
                { auto r = Row("Health Pos");    Combo(r, healthPositions, IM_ARRAYSIZE(healthPositions), &healthPos); }
                { auto r = Row("ESP Style");     Combo(r, espStyles, IM_ARRAYSIZE(espStyles), &style); }
                Config.ESPMenu.BoxType = (EspBoxType)boxType;
                Config.ESPMenu.Target = (LineTarget)target;
                Config.ESPMenu.HealthPosition = (EspHealthPosition)healthPos;
                Config.ESPMenu.EspStyle = (EspStyleTarget)style;
                EndCard(cd);
            }
            ImGui::EndChild();
        }

        inline void AimbotPage(float contentW, float childH)
        {
            const float gap = 17.0f;
            const float colW = ImFloor((contentW - gap) * 0.5f);
            ImGui::SetCursorPos(ImVec2(25.f, 87.f));
            ImGui::BeginChild("##iaim1", ImVec2(colW, childH), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
            {
                Card cd; BeginCard(cd, "AIMBOT");
                { auto r = Row("Aimbot 360", true); Switch(r, &Config.Aim.Aimbot360); }
                { auto r = Row("Bullet Track");     Switch(r, &Config.Aim.AimSilent); }
                { auto r = Row("Aim Assist Size");  Slider(r, &Config.Aim.AimAssistSize, 0.f, 100.f, "%.0f", true); }
                EndCard(cd);
            }
            ImGui::EndChild();
            ImGui::SameLine(0.f, gap);
            ImGui::BeginChild("##iaim2", ImVec2(colW, childH), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
            {
                Card cd; BeginCard(cd, "COMBAT OPTIONS");
                static const char *targets[] = { "Head", "Chest", "Body" };
                static const char *triggers[] = { "None", "Shooting", "Scoping" };
                static const char *targetBy[] = { "Distance", "FOV" };
                int target = ImClamp((int)Config.Aim.Target, 0, 2);
                int trigger = ImClamp((int)Config.Aim.Trigger, 0, 2);
                int aimBy = ImClamp((int)Config.Aim.By, 0, 1);
                { auto r = Row("Location");  Combo(r, targets, 3, &target); }
                { auto r = Row("Trigger");   Combo(r, triggers, 3, &trigger); }
                { auto r = Row("Target By"); Combo(r, targetBy, 2, &aimBy); }
                Config.Aim.Target = (EAimTarget)target;
                Config.Aim.Trigger = (EAimTrigger)trigger;
                Config.Aim.By = (EAim)aimBy;
                { auto r = Row("FOV Size", true); Slider(r, &Config.Aim.Cross, 0.f, 100.f, "%.0f", true); }
                EndCard(cd);
            }
            ImGui::EndChild();
        }

        inline void PlayersPage(float contentW, float childH)
        {
            const float gap = 17.0f;
            const float colW = ImFloor((contentW - gap) * 0.5f);
            ImGui::SetCursorPos(ImVec2(25.f, 87.f));
            ImGui::BeginChild("##imos1", ImVec2(colW, childH), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
            {
                Card cd; BeginCard(cd, "MEMORY HACKS");
                { auto r = Row("Hitbox", true);        Switch(r, &Config.ExtraMenu.Hit); }
                { auto r = Row("No Recoil");           Switch(r, &Config.ExtraMenu.Recoil); }
                { auto r = Row("No Spread");           Switch(r, &Config.ExtraMenu.Spread); }
                { auto r = Row("No Shake");            Switch(r, &Config.ExtraMenu.Shake); }
                { auto r = Row("No Overheat");         Switch(r, &Config.ExtraMenu.Rpd); }
                { auto r = Row("No Parachute");        Switch(r, &Config.ExtraMenu.Parachute); }
                { auto r = Row("Anti Flashbang");      Switch(r, &Config.ExtraMenu.Flash); }
                { auto r = Row("Firerate");            Switch(r, &Config.ExtraMenu.Fire); }
                { auto r = Row("Fast Dive");           Switch(r, &Config.ExtraMenu.Diving); }
                { auto r = Row("Fast Reload");         Switch(r, &Config.ExtraMenu.Reload); }
                { auto r = Row("Fast Scope");          Switch(r, &Config.ExtraMenu.Scope); }
                { auto r = Row("Quick Switch");        Switch(r, &Config.ExtraMenu.Switch); }
                { auto r = Row("Weapon Kinetic");      Switch(r, &Config.ExtraMenu.Kinetic); }
                EndCard(cd);
            }
            ImGui::EndChild();
            ImGui::SameLine(0.f, gap);
            ImGui::BeginChild("##imos2", ImVec2(colW, childH), 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
            {
                Card cd; BeginCard(cd, "MOVEMENT");
                { auto r = Row("Snowboard Speed", true); Slider(r, &SnowBsize, 0.f, 100.f, "%.1f"); }
                { auto r = Row("Slide Distance");        Slider(r, &SlideRange, 0.f, 30.f, "%.1f"); }
                { auto r = Row("SpeedHack");             Slider(r, &speedHackMultiplier, 0.5f, 2.f, "%.1fx"); }
                { auto r = Row("High Jump");             Slider(r, &jumpHeightMultiplier, 0.5f, 5.f, "%.2fx"); }
                EndCard(cd);
            }
            ImGui::EndChild();
        }
    } // namespace data

} // namespace imguimenu

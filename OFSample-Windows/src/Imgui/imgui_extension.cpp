#include "imgui_extension.h"
#include "imgui_internal.h"

#include <ctype.h>      // toupper, isprint
#include <stdio.h>      // vsnprintf, sscanf, printf
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif

namespace ImGui
{
    // frame_padding < 0: uses FramePadding from style (default)
// frame_padding = 0: no framing
// frame_padding > 0: set framing size
// The color used are the button colors.
    bool ImGui::ImageRoundButton(ImGuiID& gui_id, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Default to using texture ID as ID. User can still push string/integer prefixes.
        // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
        PushID((void *)user_texture_id);
        ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
        const ImVec2 bbmaxIm(window->DC.CursorPos.x + size.x + padding.x * 2, window->DC.CursorPos.y + size.y + padding.y * 2);
        const ImRect bb(window->DC.CursorPos, bbmaxIm);
        const ImVec2 image_bbminIm(window->DC.CursorPos.x + padding.x, window->DC.CursorPos.y + padding.y);
        const ImVec2 image_bbmaxIm(window->DC.CursorPos.x + padding.x + size.x, window->DC.CursorPos.y + padding.y + size.y);
        const ImRect image_bb(image_bbminIm, image_bbmaxIm);
        ItemSize(bb);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);

        // Render
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(252.f / 255.f, 253.f / 255.f, 255.f / 255.f, .82f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(119.f / 255.f, 135.f / 255.f, 233.f / 255.f, 1.0f));
        const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

        RenderNavHighlight(bb, id);

        //printf("w: %f h: %f\n", bb.GetWidth(), bb.GetHeight());

        float rounding = bb.GetWidth() / 2.f;
        int circleSeg = 64;
        {
            window->DrawList->AddCircleFilled(bb.GetCenter(), bb.GetWidth() / 2.f, col, circleSeg);
            const float border_size = 3.f;
            if (gui_id == id && border_size > 0.0f)
            {
                const ImVec2 bbCenIm(bb.GetCenter().x + 1.0f, bb.GetCenter().y + 1.0f);
                window->DrawList->AddCircle(bbCenIm, bb.GetWidth() / 2.f, GetColorU32(ImGuiCol_BorderShadow), circleSeg, border_size);
                window->DrawList->AddCircle(bb.GetCenter(), bb.GetWidth() / 2.f, GetColorU32(ImGuiCol_Border), circleSeg, border_size);
            }
        }
        ImGui::PopStyleColor(2);
        if (pressed)
        {
            if (gui_id == id)
            {
                gui_id = 0;
            }
            else
            {
                gui_id = id;
            }
        }
        if (bg_col.w > 0.0f)
            window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
        window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

        return pressed;
    }

    // frame_padding < 0: uses FramePadding from style (default)
    // frame_padding = 0: no framing
    // frame_padding > 0: set framing size
    // The color used are the button colors.
    bool ImGui::ImageRoundButton2(ImTextureID user_texture_id, ImTextureID user_texture_id2, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Default to using texture ID as ID. User can still push string/integer prefixes.
        // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
        PushID((void *)user_texture_id);
        ImGuiID id = window->GetID("#image");
        PopID();

        const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
        const ImVec2 bbmaxIm(window->DC.CursorPos.x + size.x + padding.x * 2, window->DC.CursorPos.y + size.y + padding.y * 2); 
        const ImRect bb(window->DC.CursorPos, bbmaxIm);
        const ImVec2 image_bbminIm(window->DC.CursorPos.x + padding.x, window->DC.CursorPos.y + padding.y);
        const ImVec2 image_bbmaxIm(window->DC.CursorPos.x + padding.x + size.x, window->DC.CursorPos.y + padding.y + size.y);
        const ImRect image_bb(image_bbminIm, image_bbmaxIm);
        ItemSize(bb);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held);

        // Render	
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(119.f / 255.f, 135.f / 255.f, 233.f / 255.f, 1.0f));
        const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

        RenderNavHighlight(bb, id);

        float rounding = bb.GetWidth() / 2.f;
        int circleSeg = 64;
        window->DrawList->AddCircleFilled(bb.GetCenter(), bb.GetWidth() / 2.f, col, circleSeg);
        ImGui::PopStyleColor(1);

        if (bg_col.w > 0.0f)
            window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
        if (hovered)
        {
            window->DrawList->AddImage(user_texture_id2, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
        }
        else
        {
            window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));
        }

        return pressed;
    }
}
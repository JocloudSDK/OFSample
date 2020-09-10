#ifndef IMGUI_API
#define IMGUI_API
#endif
#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API      IMGUI_API
#endif

#include "imgui.h"

namespace ImGui
{
    IMGUI_API bool  ImageRoundButton(ImGuiID& id, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));    // <0 frame_padding uses default frame padding settings. 0 for no padding
    IMGUI_API bool  ImageRoundButton2(ImTextureID user_texture_id, ImTextureID user_texture_id2, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));// <0 frame_padding uses default frame padding settings. 0 for no padding
}
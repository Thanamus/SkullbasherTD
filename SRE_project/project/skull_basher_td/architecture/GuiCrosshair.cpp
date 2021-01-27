
#include "GuiCrosshair.hpp"
#include <sre/Renderer.hpp>

GuiCrosshair::GuiCrosshair(GameObject* gameObject) : Component(gameObject)
{
    crosshair = sre::Texture::create().withFile("assets/crosshair.png").withFilterSampling(false).build();
}

GuiCrosshair::~GuiCrosshair()
{
}

void GuiCrosshair::drawGui(sre::RenderPass *renderPass) {
    auto r = sre::Renderer::instance;
    auto winsize = r->getWindowSize();
    ImVec2 size = {64, 64};
    ImVec2 pos = {(winsize.x / 2.0f) - 32 ,(winsize.y / 2.0f) - 32};
    auto cond = ImGuiCond_Always;
    ImVec2 pivot = {0,0};
    ImGui::SetNextWindowPos(pos, cond, pivot);

    ImGui::SetNextWindowSize(size, cond);
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar;
    bool* open = nullptr;
    ImGui::PushStyleColor(ImGuiCol_WindowBg,
                          ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Border,
                          ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    ImGui::Begin("#crosshair", open, flags);

    ImVec2 uv0(0,1); // flip y axis coordinates
    ImVec2 uv1(1,0);

    // Draw background
    ImGui::Image(crosshair->getNativeTexturePtr(),size, uv0, uv1);

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}
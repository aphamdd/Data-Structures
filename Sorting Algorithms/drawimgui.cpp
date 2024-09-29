#pragma once
#include "DrawImgui.h"

void DrawImgui::run() {
  dockspace();
  //ImGui::ShowDemoWindow();
}

void DrawImgui::dockspace() {
  // Dockspace TODO: set up defaults and max/min sizes
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  // updates dockspace if window resizes
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(window.getSize().x, window.getSize().y));
  ImGui::SetNextWindowBgAlpha(0.0f);
  // ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowPadding = ImVec2(0, 0);
  ImGui::Begin("DockSpace", nullptr, window_flags);
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
  ImGui::End();
}

sf::FloatRect DrawImgui::calcViewport() {
  float guiWidth = ImGui::GetWindowSize().x;
  float winWidth = window.getSize().x;
  float viewWidth = winWidth - guiWidth;
  return sf::FloatRect(
    0.f, // x left
    0.f, // y top
    viewWidth / winWidth, // x right
    1.f // y bottom
  );
}

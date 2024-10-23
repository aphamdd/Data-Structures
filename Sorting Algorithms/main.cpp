#include "DrawImgui.h"
#include <SFML/Graphics.hpp>

void imguiStyle() {
  ImGuiStyle& style = ImGui::GetStyle();
  style.Alpha = 1.0;
  //style.WindowFillAlphaDefault = 0.83;
  //style.ChildWindowRounding = 3;
  style.WindowRounding = 3;
  style.GrabRounding = 1;
  style.GrabMinSize = 20;
  style.FrameRounding = 3;


  style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  //style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
  //style.Colors[ImGuiCol_ComboBg] = ImVec4(0.16f, 0.24f, 0.22f, 0.60f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
  //style.Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.50f, 0.50f, 0.33f);
  //style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.00f, 0.50f, 0.50f, 0.47f);
  //style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
  //style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.78f, 0.78f, 0.35f);
  //style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.00f, 0.78f, 0.78f, 0.47f);
  //style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.00f, 0.78f, 0.78f, 1.00f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
  //style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
  //style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);
}

int main() {
  // window and gui params
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(GLOBAL::SCREEN_WIDTH, GLOBAL::SCREEN_HEIGHT), "DSA Visualizer", sf::Style::Default);
  sf::View view = window.getDefaultView();
  window.setFramerateLimit(GLOBAL::FPS);
  ImGui::SFML::Init(window);
  Control control;

  // sorting algorithm params 
  Graph graph(15);
  Algorithms algo(control);

  // linked list params
  LinkedList linkedList(window);

  // tree params
  BST bst(window);

  // majority of imgui handling done here, with lots of DI
  DrawImgui img(window, algo, graph, linkedList, bst, control, view);

  while (window.isOpen()) {
    sf::Event event;

    // event takes all values in the program event queue and handles it until clear
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);
      control.mpos = sf::Mouse::getPosition(window);
      switch (event.type) {
        case sf::Event::Closed: {
          window.close();
        } break;

        // TODO: check if on linkedlist context, useless operations if not
        case sf::Event::MouseButtonPressed: {
          if (event.mouseButton.button == sf::Mouse::Left) {
            if (control.isLinkedList) {
              if (linkedList.search(control.mpos))
                control.isDragging = true;
            }
            else if (control.isTree) {
              if (bst.search(control.mpos)) {
                control.isDragging = true;
              }
            }
          }
        } break;

        case sf::Event::MouseButtonReleased: {
          if (control.isLinkedList) {
            if (event.mouseButton.button == sf::Mouse::Left) {
              control.isDragging = false;
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
              if (linkedList.resetActiveNode())
                control.isDragging = false;
            }
            else if (event.mouseButton.button == sf::Mouse::Middle) {
              if (linkedList.mouseInBounds(control.mpos))
                control.isPopup = true;
            }
          }
          else if (control.isTree) {
            if (event.mouseButton.button == sf::Mouse::Left) {
              control.isDragging = false;
            }
          }
        } break;

        default:
          break;
      }
    }

    // GUI
    imguiStyle();
    img.run();

    // rendering
    if (!control.isTree)
      window.setView(view);
    else
      window.setView(window.getDefaultView());
    window.clear(sf::Color(18, 18, 18));

    if (control.isAlgorithm) {
      window.draw(graph);
    }
    else if (control.isLinkedList) {
      linkedList.draw();
    }
    else if (control.isTree) {
      bst.draw();
    }
    else
      throw("DRAW PROBLEM");

    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
#include "DrawImgui.h"
#include <SFML/Graphics.hpp>


int main() {
  // window and gui params
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Algorithm Visualizer", sf::Style::Default);
  sf::View view = window.getDefaultView();
  window.setFramerateLimit(FPS);
  ImGui::SFML::Init(window);
  Control control;

  // sorting algorithm params 
  Graph graph(15);
  Algorithms algo(control);

  // linked list params
  sf::Font font;
  if (!font.loadFromFile("./Fonts/kongtext.ttf"))
    throw("COULDN'T LOAD FONT");
  sf::Text LLText;
  LLText.setFont(font);
  LinkedList linkedList(window, LLText);

  // majority of imgui handling done here, with lots of DI
  DrawImgui img(window, algo, graph, linkedList, control, view);

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
            if (linkedList.search(control.mpos))
              control.isDragging = true;
          }
        } break;

        case sf::Event::MouseButtonReleased: {
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
        } break;

        default:
          break;
      }
    }

    // GUI
    img.run();

    // rendering
    window.setView(view);
    window.clear(sf::Color(18, 18, 18));

    if (control.isAlgorithm) {
      window.draw(graph);
    }
    else if (control.isLinkedList) {
      linkedList.draw();
    }
    else
      throw("DRAW PROBLEM");

    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
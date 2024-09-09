#include "common.h"
#include "Graph.h"
#include "Algorithms.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Algorithm Visualizer", sf::Style::Default);
  window.setFramerateLimit(FPS);
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;
  AlgorithmControl control;

  // set graph
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> num(100, 500);
  const int numBars = 15;
  Graph graph(numBars);
  Algorithms algo(window, control);

  // show timer
  sf::Clock clock; // declaring clock starts the timer
  sf::Font font;
  if (!font.loadFromFile("./Fonts/Retale-Regular.ttf"))
    throw("COULDN'T LOAD FONT");
  sf::Text elapsedTime;
  elapsedTime.setFont(font);
  elapsedTime.setCharacterSize(50);
  elapsedTime.setFillColor(sf::Color::Cyan);

  cout << "B: Bubble\nS: Selection\nI: Insertion\nQ: Quick\nEsc: Shuffle\n" << endl;
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::B: {
            cout << "Bubble Sort... ";
            algo.bubbleSort(graph);
            cout << "DONE" << endl;
          } break;
          case sf::Keyboard::S: {
            cout << "Selection Sort... ";
            algo.selectionSort(graph);
            cout << "DONE" << endl;
          } break;
          case sf::Keyboard::I: {
            cout << "Insertion Sort... ";
            algo.insertionSort(graph);
            cout << "DONE" << endl;
          } break;
          case sf::Keyboard::Q: {
            cout << "Quick Sort... ";
            algo.quickSort(graph, 0, graph.m_histogram.size() - 1);
            cout << "DONE" << endl;
          } break;
          case sf::Keyboard::Escape: {
            cout << "Shuffling... ";
            graph.shuffle();
            cout << "DONE" << endl;
          } break;
          default: {
          } break;
        }
      }
    }

    // temporary timer
    sf::Time elapsed = clock.getElapsedTime();
    elapsedTime.setString(to_string(elapsed.asSeconds()));

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
    ImGui::SFML::Update(window, deltaClock.restart());

    // Dockspace
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(window.getSize().x, window.getSize().y));
    ImGui::SetNextWindowBgAlpha(0.0f);
    // ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    // ImGui::ShowDemoWindow();

    // GUI
    ImGui::Begin("Settings");
    ImGui::SliderFloat("BarSpeed", &control.speedMult, 1.f, 3.f);
    ImGui::SliderFloat("AniDelay", &DELAY, 0.f, 0.5f);
    ImGui::End();

    // rendering
    window.clear(sf::Color::Black);
    window.draw(graph);
    window.draw(elapsedTime);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
#include "common.h"
#include "Graph.h"
#include "Algorithms.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Algorithm Visualizer", sf::Style::Titlebar | sf::Style::Close);
  window.setVerticalSyncEnabled(true);
  sf::View view = window.getDefaultView();
  window.setFramerateLimit(FPS);
  ImGui::SFML::Init(window);
  sf::Clock imguiClock;
  AlgorithmControl control;

  // set graph
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> num(100, 500);
  int numBars = 15;
  Graph graph(numBars);
  Algorithms algo(window, control);

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

      /*if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
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
          default: {
          } break;
        }
      }*/
    }

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
    ImGui::SFML::Update(window, imguiClock.restart());

    // Dockspace
    // TODO: set up imgui to mimic a sidebar
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    //ImGui::SetNextWindowPos(ImVec2(0, 0));
    //ImGui::SetNextWindowSize(ImVec2(window.getSize().x, window.getSize().y));
    ImGui::SetNextWindowBgAlpha(0.0f);
    // ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    // ImGui::ShowDemoWindow();

    // GUI
    //ImGui::SetNextWindowSize(ImVec2(400, 300));
    //ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(600, 400));
    ImGui::Begin("GUI");

    // TODO: disable the button somehow when sorting
    if (ImGui::Button(!control.isBubble ? "Bubble Sort" : "In Progress...")) {
      if (!control.isSorting) {
        algo.reset();
        control.isSorting = true;
        control.isBubble = true;
        cout << "Bubble Sort... ";
      }
    }
    if (ImGui::Button(!control.isSelection ? "Selection Sort" : "In Progress...")) {
      if (!control.isSorting) {
        algo.reset();
        control.isSorting = true;
        control.isSelection = true;
        cout << "Selection Sort... ";
      }
    }

    if (control.isSorting)
      ImGui::BeginDisabled();
    // TODO: implement skip animation
    ImGui::Button("Skip (WIP)");
    if (ImGui::Button("Shuffle")) {
      graph.shuffle();
    }
    int prev = numBars;
    ImGui::SliderInt("# of Bars", &numBars, 2, 100);
    if (prev != numBars) {
      graph.build(numBars);
      prev = numBars;
    }
    if (control.isSorting)
      ImGui::EndDisabled();

    if (ImGui::Button(control.isPaused ? "Play" : "Pause")) {
      control.isPaused = !control.isPaused;
      algo.delayClock.restart();
    }
    // TODO: properly reset algorithm when in the middle of swapping
    if (ImGui::Button("Stop (WIP)")) {
      control.isSorting = false;
      control.isPaused = false;
      algo.reset();
    }
    // TODO: insert fancy graph of algorithm data
    // TODO: have a popout window that shows the graph in an array form
    ImGui::SliderFloat("Bar Speed", &control.speedMult, 0.1f, 10.0f);
    ImGui::SliderFloat("Pause Timer", &DELAY, 0.f, 0.5f);
    ImGui::Text("# of comparisons: %d", algo.mCompares);

    // problem with this approach, it's just scaling the view instead of actually
    // measuring the widths properly
    float guiWidth = ImGui::GetWindowSize().x;
    float winWidth = window.getSize().x;
    float viewWidth = winWidth - guiWidth;
    view.setViewport(sf::FloatRect(
      0.f, // x left
      0.f, // y top
      viewWidth / winWidth, // x right
      1.f // y bottom
    ));

    ImGui::End(); // End GUI Block

    // start bubble sorting
    if (control.isSorting && control.isBubble && !control.isPaused) {
      if (algo.bubbleSort(graph)) {
        control.isSorting = false;
        control.isBubble = false;
      }
    }
    // start selection sorting
    if (control.isSorting && control.isSelection && !control.isPaused) {
      if (algo.selectionSort(graph)) {
        control.isSorting = false;
        control.isSelection = false;
      }
    }

    // rendering
    window.setView(view);

    window.clear(sf::Color::Black);
    window.draw(graph);
    window.draw(elapsedTime);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
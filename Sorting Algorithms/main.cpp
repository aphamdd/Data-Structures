#include "common.h"
#include "Graph.h"
#include "Algorithms.h"
#include "LLNode.h"
#include "LinkedList.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
using namespace std;

int main() {
  // window and gui params
  //sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Algorithm Visualizer", sf::Style::Titlebar | sf::Style::Close);
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Algorithm Visualizer", sf::Style::Default);
  sf::View view = window.getDefaultView();
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(FPS);
  ImGui::SFML::Init(window);
  sf::Clock imguiClock;
  Control control;

  // sorting algorithm params 
  int numBars = 15;
  Graph graph(numBars);
  Algorithms algo(control);

  // linked list params
  sf::Font font;
  if (!font.loadFromFile("./Fonts/Retale-Regular.ttf"))
    throw("COULDN'T LOAD FONT");
  sf::Text LLText;
  LLText.setFont(font);
  LinkedList linkedList(window, LLText);
  sf::Vector2i mpos;
  LLNode* pUser = NULL;

  while (window.isOpen()) {
    sf::Event event;

    // event takes all values in the program event queue and handles it until clear
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);
      mpos = sf::Mouse::getPosition(window);
      switch (event.type) {
        case sf::Event::Closed: {
          window.close();
        } break;

        // TODO: check if on linkedlist context, useless operations if not
        case sf::Event::MouseButtonPressed: {
          if (event.mouseButton.button == sf::Mouse::Left) {
            pUser = linkedList.search(mpos);
            if (pUser)
              control.isDragging = true;
          }
        } break;

        case sf::Event::MouseButtonReleased: {
          if (event.mouseButton.button == sf::Mouse::Left) {
            control.isDragging = false;
          }
        } break;

        default:
          break;
      }
    }

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
    ImGui::SFML::Update(window, imguiClock.restart());

    // Dockspace TODO: set up defaults and max/min sizes
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    //ImGui::SetNextWindowPos(ImVec2(0, 0));
    //ImGui::SetNextWindowSize(ImVec2(window.getSize().x, window.getSize().y));
    ImGui::SetNextWindowBgAlpha(0.0f);
    // ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0, 0);
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    //ImGui::ShowDemoWindow();

    // GUI
    ImGui::Begin("Visualizer");
    
    // scales viewport with gui width
    float guiWidth = ImGui::GetWindowSize().x;
    float winWidth = window.getSize().x;
    float viewWidth = winWidth - guiWidth;
    view.setViewport(sf::FloatRect(
      0.f, // x left
      0.f, // y top
      viewWidth / winWidth, // x right
      1.f // y bottom
    ));

    if (ImGui::BeginTabBar("Tab Bar")) {
      if (ImGui::BeginTabItem("Algorithms")) {
        control.isAlgorithm = true;
        control.isLinkedList = false;

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

        if (ImGui::Button(!control.isInsertion ? "Insertion Sort" : "In Progress...")) {
          if (!control.isInsertion) {
            algo.reset();
            control.isSorting = true;
            control.isInsertion = true;
            cout << "Insertion Sort... ";
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
        ImGui::SliderInt("# of Bars", &numBars, 2, 50);
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
        ImGui::SliderFloat("Bar Speed", &control.speedMult, 0.1f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("Pause Timer", &DELAY, 0.f, 0.5f);

        // probably use some var to handle the status message
        if (!control.isSorting && !control.isPaused)
          ImGui::Text("Status: IDLE");
        else if (control.isSorting && !control.isPaused)
          ImGui::Text("Status: RUNNING");
        else if (control.isSorting && control.isPaused)
          ImGui::Text("Status: PAUSED");

        // %s doesn't take std::string, but rather char ptrs aka char[size]
        const char* curState(algo.displayState[algo.getState()].c_str());
        ImGui::Text("STATE: %s", curState);

        // TODO: include min compares as well?
        ImGui::Text("# of comparisons: %d", algo.mCompares);
        if (control.isBubble)
          control.bubbleCompares = max(algo.mCompares, control.bubbleCompares);
        else if (control.isSelection)
          control.selectionCompares = max(algo.mCompares, control.selectionCompares);
        else if (control.isInsertion)
          control.insertionCompares = max(algo.mCompares, control.insertionCompares);
        ImGui::Text("Bubble Sort MAX Compares: %d", control.bubbleCompares);
        ImGui::Text("Selection Sort MAX Compares: %d", control.selectionCompares);
        ImGui::Text("Insertion Sort MAX Compares: %d", control.insertionCompares);

        // sorting algorithms
        if (control.isSorting && control.isBubble && !control.isPaused) {
          if (algo.bubbleSort(graph)) {
            control.isSorting = false;
            control.isBubble = false;
            algo.reset();
          }
        }

        if (control.isSorting && control.isSelection && !control.isPaused) {
          if (algo.selectionSort(graph)) {
            control.isSorting = false;
            control.isSelection = false;
            algo.reset();
          }
        }

        if (control.isSorting && control.isInsertion && !control.isPaused) {
          if (algo.insertionSort(graph)) {
            control.isSorting = false;
            control.isInsertion = false;
            algo.reset();
          }
        }

        //algo.quickSort(graph, 0, graph.m_histogram.size() - 1);

        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Linked Lists")) {
        control.isAlgorithm = false;
        control.isLinkedList = true;

        if (ImGui::Button("Add")) {
          linkedList.add();
        }
        if (ImGui::Button("Remove")) {
          linkedList.remove();
        }

        // Mouse position scales with the view transformation (gui width)
        // TODO: properly handle ptr when a node gets deleted
        ImVec4 green(0.0f, 1.0f, 0.0f, 1.0f), white(1.0f, 1.0f, 1.0f, 1.0f);
        sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
        if (control.isDragging)
          linkedList.move(pUser, mpos);
        ImGui::TextColored(linkedList.isInBounds(pUser, mpos) ? green : white, "Mouse PixelToCoords (x:%0.0f, y:%0.0f)", convertMPos.x, convertMPos.y);

        ImGui::EndTabItem();
      }

      /*
      if (ImGui::BeginTabItem("Binary Trees")) {
        ImGui::Text("test1");
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Graphs")) {
        ImGui::Text("test2");
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Mazes")) {
        ImGui::Text("test3");
        ImGui::EndTabItem();
      }
      */
      ImGui::EndTabBar();
    }
    ImGui::End(); // End GUI Block

    // rendering
    window.setView(view);
    window.clear(sf::Color::Black);

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
#include "DrawImgui.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;


int main() {
  // window and gui params
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Algorithm Visualizer", sf::Style::Default);
  sf::View view = window.getDefaultView();
  window.setFramerateLimit(FPS);
  ImGui::SFML::Init(window);
  sf::Clock imguiClock;
  Control control;
  float fontScale = 2;

  // sorting algorithm params 
  int numBars = 15;
  Graph graph(numBars);
  Algorithms algo(control);

  // linked list params
  sf::Font font;
  if (!font.loadFromFile("./Fonts/kongtext.ttf"))
    throw("COULDN'T LOAD FONT");
  sf::Text LLText;
  LLText.setFont(font);
  LinkedList linkedList(window, LLText);
  sf::Vector2i mpos;
  int nodeNum = 0;

  DrawImgui img(window, algo, graph, linkedList);

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
            if (linkedList.search(mpos))
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
            if (linkedList.mouseInBounds(mpos))
              control.isPopup = true;
          }
        } break;

        default:
          break;
      }
    }

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
    ImGui::SFML::Update(window, imguiClock.restart());

    img.dockspace();

    // GUI
    ImGui::Begin("Visualizer");
    ImGui::SetWindowFontScale(fontScale);
    
    // scales viewport with gui width
    view.setViewport(img.calcViewport());

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
        ImGui::TextWrapped("# of comparisons: %d", algo.mCompares);
        if (control.isBubble)
          control.bubbleCompares = max(algo.mCompares, control.bubbleCompares);
        else if (control.isSelection)
          control.selectionCompares = max(algo.mCompares, control.selectionCompares);
        else if (control.isInsertion)
          control.insertionCompares = max(algo.mCompares, control.insertionCompares);
        ImGui::TextWrapped("Bubble Sort MAX Compares: %d", control.bubbleCompares);
        ImGui::TextWrapped("Selection Sort MAX Compares: %d", control.selectionCompares);
        ImGui::TextWrapped("Insertion Sort MAX Compares: %d", control.insertionCompares);

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
        if (ImGui::Button("Clear")) {
          linkedList.clear();
        }

        // Mouse position scales with the view transformation (gui width)
        // TODO: properly handle ptr when a node gets deleted
        ImVec4 green(0.0f, 1.0f, 0.0f, 1.0f), white(1.0f, 1.0f, 1.0f, 1.0f);
        sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
        if (control.isDragging) {
          if (!linkedList.move(mpos)) {
            throw("idk how I'm moving nothing");
          }
        }
        ImGui::TextColored(linkedList.mouseInBounds(mpos) ? green : white, "Mapped Mouse (x:%0.0f, y:%0.0f)", convertMPos.x, convertMPos.y);

        ImGui::TextWrapped("pActive: %p", linkedList.mActive);

        ImGui::Text("Find Value: %d", nodeNum);
        if (ImGui::Button(control.isSearching ? "Searching..." : "Find"))
          control.isSearching = true;
        if (control.isSearching) {
          if (linkedList.findValueAnimated(nodeNum))
            control.isSearching = false;
        }
        else
          linkedList.updateCursor(linkedList.mActive);

        //ImGui::SliderFloat("Cursor Speed", &, 1.0f, 5.0f);
        ImGui::SliderFloat("Pause Timer", &DELAY, 0.1f, 0.5f);
        
        ImGui::TextWrapped(
          "This data structure is an actual linked list, not a simulated one\n"
          "Left click: click to select the node\n"
          "Right click: anywhere to de-select node\n"
          "Middle click: select node first, then middle click on node to popup\n"
          "Add: create node at tail or after active node\n"
          "Remove: removes node at tail or the active node\n"
          "Clear: removes all nodes\n"
          "Set Value: select a node and set value to change node value\n"
          "Find Value: select node and type a # to search for a node\n"
        );

        if (control.isPopup) {
          ImGui::OpenPopup("Node_popup");
          control.isPopup = false;
        }
        if (ImGui::BeginPopup("Node_popup")) {
          ImGui::Text("I'm a popup!");
          ImGui::InputInt("Set Node Value:", &nodeNum);
          if (ImGui::Button("Set"))
            linkedList.mActive->setText(nodeNum);
          ImGui::EndPopup();
        }

        // Note: this is so fking scuffed
        ImGui::Begin("Linked List Data");
        std::vector<std::string> text;
        if (control.isSearching)
          text = linkedList.parseString(linkedList.mStatePtr);
        else
          text = linkedList.parseString(linkedList.mActive);

        linkedList.transformText(text);

        if (text.size() > 0) {
          if (text.size() == 1) {
            ImGui::Text("head->%s", text.at(0).c_str());
            ImGui::SameLine(0, 0);
            ImGui::Text("null");
          }
          else if (text.size() == 2) {
            if (text.at(0).at(0) == 'f') {
              text.at(0).erase(text.at(0).begin());
              ImGui::Text("head->");
              ImGui::SameLine(0, 0);
              ImGui::TextColored(green, "%s", text.at(0).c_str());
              ImGui::SameLine(0, 0);
              ImGui::Text("->");
              ImGui::SameLine(0, 0);
              ImGui::Text("%s", text.at(1).c_str());
              ImGui::SameLine(0, 0);
              ImGui::Text("null");
            }
            else if (text.at(0).at(0) == 'b') {
              text.at(0).erase(text.at(0).begin());
              ImGui::Text("head->%s", text.at(1).c_str());
              ImGui::SameLine(0, 0);
              ImGui::TextColored(green, "%s", text.at(0).c_str());
              ImGui::SameLine(0, 0);
              ImGui::Text("->");
              ImGui::SameLine(0, 0);
              ImGui::Text("null");
            }
          }
          else if (text.size() == 3) {
            ImGui::Text("head->%s", text.at(0).c_str());
            ImGui::SameLine(0, 0);
            ImGui::TextColored(green, "%s", text.at(1).c_str());
            ImGui::SameLine(0, 0);
            ImGui::Text("->");
            ImGui::SameLine(0, 0);
            ImGui::Text("%s", text.at(2).c_str());
            ImGui::SameLine(0, 0);
            ImGui::Text("null");
          }
        }
        ImGui::End();

        linkedList.dtRestart();
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
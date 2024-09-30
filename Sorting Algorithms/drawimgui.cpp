#pragma once
#include "DrawImgui.h"

void DrawImgui::run() {
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
  ImGui::SFML::Update(window, imguiClock.restart());

  dockspace();
  //ImGui::ShowDemoWindow();

  ImGui::Begin("Visualizer");
  ImGui::SetWindowFontScale(fontScale);

  // scales viewport with gui width
  view.setViewport(calcViewport());

  if (ImGui::BeginTabBar("Tab Bar")) {
    sortingAlgsTab();
    linkedListTab();
    ImGui::EndTabBar();
  }

  ImGui::End(); // End GUI Block
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

void DrawImgui::sortingAlgsTab() {
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
    int prev = control.numBars;
    ImGui::SliderInt("# of Bars", &control.numBars, 2, 100);
    if (prev != control.numBars) {
      graph.build(control.numBars);
      prev = control.numBars;
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
}

void DrawImgui::linkedListTab() {
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
    sf::Vector2f convertMPos = window.mapPixelToCoords(control.mpos, window.getView());
    if (control.isDragging) {
      try {
        if (!linkedList.move(control.mpos))
          throw std::runtime_error("idk how I'm moving nothing");
      }
      catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
    ImGui::TextColored(linkedList.mouseInBounds(control.mpos) ? green : white, "Mapped Mouse (x:%0.0f, y:%0.0f)", convertMPos.x, convertMPos.y);

    ImGui::TextWrapped("pActive: %p", linkedList.rawptr.active);

    ImGui::Text("Find Value: %d", control.nodeNum);
    if (ImGui::Button(control.isSearching ? "Searching..." : "Find"))
      control.isSearching = true;
    if (control.isSearching) {
      if (linkedList.findValueAnimated(control.nodeNum))
        control.isSearching = false;
    }
    else
      linkedList.updateCursor(linkedList.rawptr.active);

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
      ImGui::InputInt("Set Node Value:", &control.nodeNum);
      if (ImGui::Button("Set"))
        linkedList.rawptr.active->setText(control.nodeNum);
      ImGui::EndPopup();
    }

    // Note: this is so fking scuffed
    ImGui::Begin("Linked List Data");
    std::vector<std::string> text;
    if (control.isSearching)
      text = linkedList.parseString(linkedList.rawptr.currAnimate);
    else
      text = linkedList.parseString(linkedList.rawptr.active);

    try {
      linkedList.transformText(text);
    }
    catch (invalid_argument& e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }

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
}

void DrawImgui::binaryTreeTab() {
  if (ImGui::BeginTabItem("Binary Trees")) {
    ImGui::Text("test1");
    ImGui::EndTabItem();
  }
}

void DrawImgui::graphTab() {
  if (ImGui::BeginTabItem("Graphs")) {
    ImGui::Text("test2");
    ImGui::EndTabItem();
  }
}

void DrawImgui::mazeTab() {
  if (ImGui::BeginTabItem("Mazes")) {
    ImGui::Text("test3");
    ImGui::EndTabItem();
  }
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

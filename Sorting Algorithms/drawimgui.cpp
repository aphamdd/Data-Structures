#pragma once
#pragma warning( disable : 4244 )
#include "DrawImgui.h"

void DrawImgui::run() {
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
  ImGui::SFML::Update(window, imguiClock.restart());

  dockspace();
  ImGui::ShowDemoWindow();

  ImGui::Begin("Visualizer");
  ImGui::SetWindowFontScale(fontScale);

  // scales viewport with gui width
  view.setViewport(calcViewport());

  if (ImGui::BeginTabBar("Tab Bar")) {
    treeTab();
    linkedListTab();
    sortingAlgsTab();
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
  if (ImGui::BeginTabItem("Arrays")) {
    control.isAlgorithm = true;
    control.isLinkedList = false;
    control.isTree = false;

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
    ImGui::SliderFloat("Pause Timer", &GLOBAL::DELAY, 0.f, 0.5f);

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
  if (ImGui::BeginTabItem("LL")) {
    control.isAlgorithm = false;
    control.isLinkedList = true;
    control.isTree = false;

    // drag node
    if (control.isDragging) {
      try {
        if (!linkedList.move(control.mpos))
          throw std::runtime_error("moving NULL node");
      }
      catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }

    if (ImGui::Button("Add")) {
      linkedList.add();
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
      //ImGui::SetItemTooltip("hello"); // used if I just need text
      // this is used if I need more than just text in my tooltip
      if (ImGui::BeginItemTooltip()) {
        ImGui::SeparatorText("Help");
        ImGui::Text("Insert nodes at the end by default.\n"
                    "Click on a node to insert after that node.");
        ImGui::EndTooltip();
      }
    }

    if (ImGui::Button("Remove")) {
      linkedList.remove();
    }
    if (ImGui::Button("Clear")) {
      linkedList.clear();
    }

    ImVec4 green(0.0f, 1.0f, 0.0f, 1.0f), white(1.0f, 1.0f, 1.0f, 1.0f);
    sf::Vector2f convertMPos = window.mapPixelToCoords(control.mpos, window.getView());
    ImGui::TextColored(linkedList.mouseInBounds(control.mpos) ? green : white, "Mapped Mouse (x:%0.0f, y:%0.0f)", convertMPos.x, convertMPos.y);
    ImGui::TextWrapped("pActive: %p", linkedList.raw.active);

    // edit delay between highlights and search
    //ImGui::SliderFloat("Cursor Speed", &, 1.0f, 5.0f);
    ImGui::SliderFloat("Pause Timer", &GLOBAL::DELAY, 0.1f, 0.5f);

    // animated search
    ImGui::InputInt("Find Node", &control.findNum);
    if (ImGui::Button(control.isSearching ? "Searching..." : "Find"))
      control.isSearching = true;
    if (control.isSearching) {
      if (linkedList.findValueAnimated(control.findNum))
        control.isSearching = false;
    }
    else
      linkedList.updateCursor(linkedList.raw.active); // cursor sprite

    ImGui::SeparatorText("Info");

    ImGui::TextWrapped(
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
      ImGui::InputInt("Node Number", &control.setNum);
      if (ImGui::Button("Set"))
        linkedList.raw.active->setText(control.setNum);
      ImGui::EndPopup();
    }

    // Note: this is so fking scuffed
    ImGui::Begin("Linked List Data");
    std::vector<std::string> text;
    if (control.isSearching)
      text = linkedList.parseString(linkedList.raw.currAnimate);
    else
      text = linkedList.parseString(linkedList.raw.active);

    try {
      linkedList.transformText(text);
    }
    catch (invalid_argument& e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }

    if (text.size() > 0) {
      if (text.size() == 1) {
        ImGui::Text("head->%s", text.at(0).c_str());
        ImGui::SameLine(0, 0); ImGui::Text("null");
      }
      else if (text.size() == 2) {
        if (text.at(0).at(0) == 'f') {
          text.at(0).erase(text.at(0).begin());
          ImGui::Text("head->"); 
          ImGui::SameLine(0, 0); ImGui::TextColored(green, "%s", text.at(0).c_str()); 
          ImGui::SameLine(0, 0); ImGui::Text("->"); 
          ImGui::SameLine(0, 0); ImGui::Text("%s", text.at(1).c_str()); 
          ImGui::SameLine(0, 0); ImGui::Text("null");
        }
        else if (text.at(0).at(0) == 'b') {
          text.at(0).erase(text.at(0).begin());
          ImGui::Text("head->%s", text.at(1).c_str()); 
          ImGui::SameLine(0, 0); ImGui::TextColored(green, "%s", text.at(0).c_str()); 
          ImGui::SameLine(0, 0); ImGui::Text("->"); 
          ImGui::SameLine(0, 0); ImGui::Text("null");
        }
      }
      else if (text.size() == 3) {
        ImGui::Text("head->%s", text.at(0).c_str()); 
        ImGui::SameLine(0, 0); ImGui::TextColored(green, "%s", text.at(1).c_str()); 
        ImGui::SameLine(0, 0); ImGui::Text("->");
        ImGui::SameLine(0, 0); ImGui::Text("%s", text.at(2).c_str());
        ImGui::SameLine(0, 0); ImGui::Text("null");
      }
    }
    ImGui::End();

    linkedList.dtRestart();
    ImGui::EndTabItem();
  }
}

void DrawImgui::treeTab() {
  if (ImGui::BeginTabItem("Trees")) {
    control.isAlgorithm = false;
    control.isLinkedList = false;
    control.isTree = true;

    if (control.isDragging && control.isDrag)
      bst.move(control.mpos);

    ImGui::SeparatorText("Actions");
    ImGui::InputInt("Node Val", &control.setTreeNum);
    if (ImGui::Button("Add")) {
      bst.insert(control.setTreeNum, sf::Vector2f(900, 100));
    }
    if (ImGui::Button("Delete Node")) {
      bst.remove();
    }
    if (ImGui::Button("Find")) {
      if (bst.findValue(control.setTreeNum)) {
        std::cout << "Found" << std::endl;
      }
    }
    if (ImGui::Button("Clear")) { 
      bst.clear();
    }
    ImGui::Checkbox("Enable Drag", &control.isDrag);

    ImGui::SeparatorText("Tree Animations");
    // TODO: prevent clicks after initial click
    if (ImGui::Button("DFS Traverse"))
      control.animateDFS = true;
    if (ImGui::Button("BFS Traverse"))
      control.animateBFS = true;
    if (ImGui::Button("Search"))
      control.animateSearch = true;
    if (ImGui::Button("Insert"))
      control.animateInsert = true;
    if (control.animateDFS) {
      if (bst.dfsAnimate()) {
        std::cout << "Done DFS Traverse" << std::endl;
        control.animateDFS = false;
      }
    }
    else if (control.animateBFS) {
      if (bst.bfsAnimate()) {
        std::cout << "Done BFS Traverse" << std::endl;
        control.animateBFS = false;
      }
    }
    else if (control.animateSearch) {
      if (bst.traverseToAnimate(control.setTreeNum, 0)) {
        std::cout << "Done Searching" << std::endl;
        control.animateSearch = false;
      }
    }
    else if (control.animateInsert) {
      if (bst.traverseToAnimate(control.setTreeNum, 1)) {
        std::cout << "Done Inserting" << std::endl;
        control.animateInsert = false;
      }
    }
    else
      bst.updateCursor(bst.raw.active);
    if (ImGui::Button(control.isPaused ? "Play" : "Pause")) {
      control.isPaused = !control.isPaused;
    }
    if (control.isPaused)
      bst.delayClock.restart();
    ImGui::SliderFloat("Speed", &GLOBAL::DELAY, 0.f, 1.f);

    ImGui::SeparatorText("Tree Info");
    if (ImGui::Button("Leaves") && !bst.animate.inUse) {
      bst.findLeaves();
      bst.animate.inUse = true;
    }
    if (ImGui::Button("Children") && !bst.animate.inUse) {
      bst.findChildren();
      bst.animate.inUse = true;
    }
    if (ImGui::Button("Parent") && !bst.animate.inUse) {
      ImGui::SameLine(); bst.findParent();
      bst.animate.inUse = true;
    }

    if (bst.animate.inUse)
      bst.nodeBlink();

    // TODO: only do checks if tree state ever changes
    ImGui::SeparatorText("Tree State");
    ImVec4 green(0.0f, 1.0f, 0.0f, 1.0f), red(1.0f, 0.0f, 0.0f, 1.0f);
    ImGui::Text("isBalanced");
    ImGui::SameLine(); ImGui::TextDisabled("(?)");
    control.balanced = bst.isBalanced();
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
      if (ImGui::BeginItemTooltip()) {
        ImGui::SeparatorText("Info");
        ImGui::Text("A balanced binary tree, also referred to as a "
          "height-balanced binary tree, is defined as a "
          "\nbinary tree in which the height of the left and right subtree "
          "of any node differ by not more than 1."
          "\n1. difference between the left and the right subtree for any node is <= 1."
          "\n2. the left subtree is balanced"
          "\n3. the right subtree is balanced");
        ImGui::EndTooltip();
      }
    }
    ImGui::SameLine(); ImGui::TextColored(
      control.balanced ? green : red, 
      control.balanced ? "True" : "False"
    );

    ImGui::Text("isComplete");
    ImGui::SameLine(); ImGui::TextDisabled("(?)");
    control.complete = bst.isComplete(bst.treeSize());
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
      if (ImGui::BeginItemTooltip()) {
        ImGui::SeparatorText("Info");
        ImGui::Text("A complete binary tree is a binary tree in which all " 
          "the levels are completely \nfilled except possibly the lowest one, "
          "which is filled from the left."
          "\n1. All the leaf elements must lean towards the left."
          "\n2. The last leaf element might not have a right sibling i.e. complete != full");
        ImGui::EndTooltip();
      }
    }
    ImGui::SameLine(); ImGui::TextColored(
      control.complete ? green : red, 
      control.complete ? "True" : "False"
    );

    ImGui::Text("isFull");
    ImGui::SameLine(); ImGui::TextDisabled("(?)");
    control.full = bst.isFull();
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
      if (ImGui::BeginItemTooltip()) {
        ImGui::SeparatorText("Info");
        ImGui::Text("A full Binary tree is a special type of binary tree in which every "
          "\nparent node / internal node has either two or no children.");
        ImGui::EndTooltip();
      }
    }
    ImGui::SameLine(); ImGui::TextColored(
      control.full ? green : red, 
      control.full ? "True" : "False"
    );

    ImGui::Text("isPerfect");
    ImGui::SameLine(); ImGui::TextDisabled("(?)");
    control.perfect = bst.isPerfect(bst.treeHeight());
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
      if (ImGui::BeginItemTooltip()) {
        ImGui::SeparatorText("Info");
        ImGui::Text("A perfect binary tree is a type of binary tree in which every internal node "
          "\nhas exactly two child nodes and all the leaf nodes are at the same level.");
        ImGui::EndTooltip();
      }
    }
    ImGui::SameLine(); ImGui::TextColored(
      control.perfect ? green : red, 
      control.perfect ? "True" : "False"
    );
    ImGui::Text("Tree Height: %d", bst.treeHeight()-1);
    ImGui::Text("Tree Size: %d", bst.treeSize());

    ImGui::SeparatorText("State");
    ImGui::Text("Step: %s", bst.currStep.c_str());

    ImGui::Begin("Stack Frames");
    // TODO: make this upside-down
    for (int i = 0; i < bst.animate.treeStack.size(); ++i) {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 15.0f));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.2f, 0.6f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.3f, 0.7f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.1f, 0.5f, 1.0f));
      float buttonWidth = ImGui::GetContentRegionAvail().x;
      std::string label = "Frame " + std::to_string(i);
      ImGui::Button(label.c_str(), ImVec2(buttonWidth, 0));
      if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary)) {
        // auto pause when hovering and highlight the node
        bst.animate.treeStack[i].statePtr->sprite.setColor(sf::Color::Green);
        if (ImGui::BeginItemTooltip()) {
          // i could've used the struct but it's private
          int data = bst.animate.treeStack[i].statePtr->data;
          bool l = bst.animate.treeStack[i].l;
          bool r = bst.animate.treeStack[i].r;
          ImGui::SeparatorText("Stack Frame");
          ImVec4 green(0.0f, 1.0f, 0.0f, 1.0f), red(1.0f, 0.0f, 0.0f, 1.0f);
          ImGui::Text("Visited Left: ");
          ImGui::SameLine(0, 0); ImGui::TextColored(l ? green : red, "%s", l ? "True" : "False");
          ImGui::Text("Visited Right: ");
          ImGui::SameLine(0, 0); ImGui::TextColored(r ? green : red, "%s", r ? "True" : "False");
          ImGui::Text("Data: %d", data);
          ImGui::EndTooltip();
        }
      }
      else {
        bst.animate.treeStack[i].statePtr->sprite.setColor(sf::Color::White);
      }
      ImGui::PopStyleColor(3);
      ImGui::PopStyleVar(2);

    }
    ImGui::End();

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

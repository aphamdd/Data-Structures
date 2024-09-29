#pragma once
#include "SFML/Graphics.hpp"
#include "common.h"
#include "Graph.h"
#include "Algorithms.h"
#include "LLNode.h"
#include "LinkedList.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
using namespace std;

class DrawImgui {
public:
  DrawImgui(sf::RenderWindow& win, Algorithms& alg, Graph& g, LinkedList& LL, Control& con, sf::View& vw) : 
    window(win),
    algo(alg),
    graph(g),
    linkedList(LL),
    control(con), 
    view(vw) {
  };

  void dockspace();
  void run();
  void sortingAlgsTab();
  void linkedListTab();
  void binaryTreeTab();
  void graphTab();
  void mazeTab();
  sf::FloatRect calcViewport();


private:
  sf::Clock imguiClock;
  float fontScale = 2;

  // lots of DI (this is probably bad)
  sf::RenderWindow& window;
  Algorithms& algo;
  Graph& graph;
  LinkedList& linkedList;
  Control& control;
  sf::View& view;
};
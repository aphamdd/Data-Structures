#pragma once
#include "SFML/Graphics.hpp"
#include "common.h"
#include "Graph.h"
#include "Algorithms.h"
#include "LLNode.h"
#include "LinkedList.h"
#include "imgui.h"
#include "imgui-SFML.h"

class DrawImgui {
public:
  DrawImgui(sf::RenderWindow& win, Algorithms& alg, Graph& g, LinkedList& LL) : 
    window(win),
    algo(alg),
    graph(g),
    linkedList(LL) {
  };

  void run();
  void dockspace();
  sf::FloatRect calcViewport();


private:
  // lots of DI (this is probably bad)
  sf::RenderWindow& window;
  Algorithms& algo;
  Graph& graph;
  LinkedList& linkedList;
  float fontScale = 2;
};
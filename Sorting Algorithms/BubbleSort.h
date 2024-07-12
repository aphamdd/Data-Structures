#pragma once
#include "Algorithms.h"
#include "Graph.h"

class BubbleSort : public Algorithms {
public:
  void initialize() override {};
  void step() override {};
  bool isComplete() const override { return false; };

  void test(Graph& graph, sf::RenderWindow& window);
};

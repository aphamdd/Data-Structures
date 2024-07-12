#pragma once
#include "BubbleSort.h"
#include <iostream>
using namespace std;

void BubbleSort::test(Graph& graph) {
  for (auto& bar : graph.getBars()) {
    bar.getShape().setFillColor(sf::Color::Blue);
  }
}

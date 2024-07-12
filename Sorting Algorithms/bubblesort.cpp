#pragma once
#include "BubbleSort.h"
#include <iostream>
using namespace std;

void BubbleSort::test(Graph& graph) {
  for (auto& bar : graph.m_histogram) {
    bar.m_shape.setFillColor(sf::Color::Blue);
  }
}

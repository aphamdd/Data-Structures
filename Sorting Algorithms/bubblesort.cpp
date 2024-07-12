#pragma once
#include "BubbleSort.h"
#include <iostream>
using namespace std;

void BubbleSort::test(Graph& graph, sf::RenderWindow& window) {
  int n = graph.m_histogram.size();
  for (int i = 0; i < n; ++i) {
    bool swap = false;
    for (int j = 0; j < n - i - 1; ++j) {
      int curr = graph.m_histogram[j].m_shape.getSize().y;
      int adj = graph.m_histogram[j + 1].m_shape.getSize().y;
      if (curr > adj) {
        sf::Vector2f temp = graph.m_histogram[j].m_shape.getPosition();
        graph.m_histogram[j].m_shape.setPosition(graph.m_histogram[j + 1].m_shape.getPosition());
        graph.m_histogram[j + 1].m_shape.setPosition(temp);
        std::swap(graph.m_histogram[j], graph.m_histogram[j + 1]);
        swap = true;

        window.clear(sf::Color::Black);
        window.draw(graph);
        window.display();
      }
    }
    if (!swap)
      break;
  }
}

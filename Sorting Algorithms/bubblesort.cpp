#pragma once
#include "BubbleSort.h"
#include <iostream>
using namespace std;

void BubbleSort::test(Graph& graph) {
  /*
  for (int i = 0; i < graph.m_histogram.size() - 1; ++i) {
    sf::Vector2f currPos = graph.m_histogram[i].m_shape.getPosition();
    sf::Vector2f nextPos = graph.m_histogram[i+1].m_shape.getPosition();
    graph.m_histogram[i].m_shape.setPosition(nextPos);
    graph.m_histogram[i + 1].m_shape.setPosition(currPos);
  }
  */
  int n = graph.m_histogram.size();
  for (int i = 0; i < n - 1; ++i) {
    cout << "entry" << endl;
    graph.m_histogram[i].m_shape.setFillColor(sf::Color::Yellow);
    for (int j = 0; j < n - i - 1; ++j) {
      int curr = graph.m_histogram[j].m_shape.getSize().y;
      int adj = graph.m_histogram[j + 1].m_shape.getSize().y;
      cout << curr << endl;
      if (curr > adj) {
        sf::Vector2f currPos = graph.m_histogram[j].m_shape.getPosition();
        sf::Vector2f nextPos = graph.m_histogram[j + 1].m_shape.getPosition();
        graph.m_histogram[j].m_shape.setPosition(nextPos);
        graph.m_histogram[j + 1].m_shape.setPosition(currPos);
      }
    }
  }
}

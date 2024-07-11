#pragma once
#include "Graph.h"
#include <iostream>
using namespace std;

Graph::Graph(const int size) : n(size) {
  m_histogram.reserve(n);
}

void Graph::structure() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> d1(100, 700);
  std::uniform_int_distribution<std::mt19937::result_type> d2(100, 700);
  std::uniform_int_distribution<std::mt19937::result_type> d3(0, 100);
  std::uniform_int_distribution<std::mt19937::result_type> d4(0, 100);
  sf::Vector2f pos(d1(rng), d2(rng));
  sf::Vector2f size(d3(rng), d4(rng));
  
  for (int i = 0; i < n; ++i) {
    m_histogram.push_back(Bars(sf::Vector2f(d1(rng), d2(rng)), sf::Vector2f(d3(rng), d4(rng))));
    cout << "test" << endl;
  }

}

void Graph::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = NULL;
  for (int i = 0; i < m_histogram.size(); ++i) {
    target.draw(m_histogram[i], states);
  }
}

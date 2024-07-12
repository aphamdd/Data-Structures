#pragma once
#include "Graph.h"
#include <iostream>
using namespace std;

Graph::Graph(const int size) {
  build(size);
}

void Graph::build(const int size) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> ranHeight(10, 600);
  float barWidth = SCREEN_WIDTH / size;
  int xPos = 0;
  sf::Vector2f position(xPos, SCREEN_HEIGHT);
  sf::Vector2f ranSize(barWidth, ranHeight(rng));
  
  for (int i = 0; i < size; ++i) {
    m_histogram.push_back(Bars(position, ranSize));
    xPos += barWidth + 1;
    position = sf::Vector2f(xPos, SCREEN_HEIGHT);
    ranSize = sf::Vector2f(barWidth, ranHeight(rng));
  }

}

void Graph::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = NULL;
  for (auto& shape : m_histogram) {
    target.draw(shape, states);
  }
}

#pragma once
#include "Graph.h"
#include <iostream>
using namespace std;

Graph::Graph(const int size) {
  build(size);
}

sf::RectangleShape Graph::initShape(const sf::Vector2f pos, const sf::Vector2f size) {
  sf::RectangleShape shape(size);
  shape.setPosition(pos);
  shape.setFillColor(sf::Color::White);
  shape.setOutlineThickness(1.f);
  shape.setOutlineColor(sf::Color::Black);
  shape.setOrigin(sf::Vector2f(0, size.y));

  return shape;
}

void Graph::build(const int size) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> ranHeight(1, SCREEN_HEIGHT-10);
  float barWidth = SCREEN_WIDTH / size;
  int xPos = 0;
  sf::Vector2f position(xPos, SCREEN_HEIGHT);
  sf::Vector2f ranSize(barWidth, ranHeight(rng));
  
  for (int i = 0; i < size; ++i) {
    m_histogram.push_back(initShape(position, ranSize));
    xPos += barWidth;
    position = sf::Vector2f(xPos, SCREEN_HEIGHT);
    ranSize = sf::Vector2f(barWidth, ranHeight(rng));
  }
}

void Graph::reset() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> num(0, m_histogram.size()-1);
  for (int i = 0; i < m_histogram.size(); ++i) {
    int val = num(rng);
    while (val == i)
      val = num(rng);
    std::swap(m_histogram.at(i), m_histogram.at(val));
    sf::Vector2f temp = m_histogram[i].getPosition();
    m_histogram[i].setPosition(m_histogram[val].getPosition());
    m_histogram[val].setPosition(temp);
  }
}

void Graph::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.texture = NULL;
  for (const auto& bar : m_histogram) {
    target.draw(bar, states);
  }
}

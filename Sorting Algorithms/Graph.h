#pragma once
#include "Container.h"
#include "Bars.h"
#include <random>

class Graph : public Container {
public:
  Graph(const int size);

private:
  void build(const int size);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  std::vector<Bars> m_histogram;
};
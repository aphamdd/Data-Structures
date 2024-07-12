#pragma once
#include <SFML/Graphics.hpp>

class Algorithms {
public:
  virtual void initialize() = 0;
  virtual void step() = 0;
  virtual bool isComplete() const = 0;
};

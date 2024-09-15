#pragma once
#include "LLNode.h"

class LinkedList {
public:
  LinkedList(sf::RenderWindow& win);
  ~LinkedList();

  void add();
  bool remove();
  void insert();
  void draw() const;
  
private:
  // TODO: will probably make a vector of LLNode* head ptrs
  LLNode* head;
  sf::RenderWindow& window; // DI
};

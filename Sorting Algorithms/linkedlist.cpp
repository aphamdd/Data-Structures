#pragma once
#include "LinkedList.h"

LinkedList::LinkedList(sf::RenderWindow& win) : window(win), head(NULL), pActive(NULL) {
}
LinkedList::~LinkedList() {
  LLNode* temp = head;
  while (head) {
    head = head->next;
    delete temp;
    temp = head;
  }
  temp = NULL;
  head = NULL;
  pActive = NULL;
}

void LinkedList::add() {
  sf::Vector2f tempPos(75.f, 400.f);
  if (!head) {
    head = new LLNode(tempPos);
    head->shape.setFillColor(sf::Color::Red);
  }
  else {
    LLNode* current = head;
    while (current->next) {
      current = current->next;
    }
    sf::Vector2f temp = current->shape.getPosition();
    temp.x += 110.f;
    current->next = new LLNode(temp);
    current->next->ID += current->ID;
    current->next->shape.setFillColor(sf::Color::Green);
  }
}

bool LinkedList::remove() {
  if (!head)
    return true;
  else if (!head->next) {
    delete head;
    head = NULL;
    return true;
  }
  LLNode* prev = NULL;
  LLNode* current = head;
  while (current->next) {
    prev = current;
    current = current->next;
  }
  delete current;
  prev->next = NULL;

  return true;
}

void LinkedList::draw() const {
  if (!head)
    return;
  LLNode* current = head;
  while (current) {
    window.draw(*current); // invokes overridden draw function in LLNode
    current = current->next;
  }
}
#pragma once
#include "LinkedList.h"

// TODO: is there a way to skip having to pass in my text through LinkedList->LLNode
LinkedList::LinkedList(sf::RenderWindow& win, sf::Text& text) : 
  window(win),
  LLText(text),
  head(NULL), 
  pActive(NULL) {
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
    head = new LLNode(tempPos, LLText);
    head->shape.setFillColor(sf::Color::Red);
  }
  else {
    LLNode* current = head;
    while (current->next) {
      current = current->next;
    }
    sf::Vector2f temp = current->shape.getPosition();
    temp.x += 110.f;
    current->next = new LLNode(temp, LLText);
    current->next->ID += current->ID;
    // TODO: create a function to update any changes to text/shape
    current->next->dataText.setString(std::to_string(current->next->ID));
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
  if (current == pActive)
    pActive = NULL;
  delete current;
  prev->next = NULL;

  return true;
}

// find the node the cursor clicks on based on coordinates
LLNode* LinkedList::search(const sf::Vector2i mpos) {
  // mapCoordsToPixel goes from world coordinates(game world, the relative view, objects in the view) to pixel coordinates(window / actual screen).
  // mapPixelsToCoords goes from pixel coordinates(window / actual screen) to world coordinates(game world, relative view, objects in the view).
  // Coordinates are relative (sfml getPositions() returns relative coords)
  // Pixels are absolute
  //
  // I likely need to use mapPixelsToCoords because shapes are all drawn
  // with relative positioning (coords). So I need to map my cursor's absolute
  // position (pixels) to the relative pixels of the view (coords). Also, since
  // I want to keep track of the cursor's positioning within the bounds of the
  // shape, I have to keep track of the relative sizing of the shape as well,
  // so converting my absolute mouse position to the relative position is the way to go.
  
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (!head)
    return NULL;

  // check if we clicked on the same active node
  if (pActive) {
    sf::FloatRect activeBounds = pActive->shape.getGlobalBounds();
    if (activeBounds.contains(convertMPos.x, convertMPos.y)) {
      return pActive;
    }
  }
  // otherwise, search all nodes and return if found
  sf::FloatRect currentBounds;
  for (LLNode* current = head; current; current = current->next) {
    currentBounds = current->shape.getGlobalBounds();
    if (currentBounds.contains(convertMPos.x, convertMPos.y)) {
      // set pactive color to white, set new pactive, highlight new pactive
      if (pActive)
        pActive->shape.setFillColor(sf::Color::White);
      pActive = current;
      pActive->shape.setFillColor(sf::Color::Green);
      return pActive;
    }
  }
  // null if not found
  return NULL;
}

bool LinkedList::move(LLNode* p, const sf::Vector2i mpos) {
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (p) {
    p->update(convertMPos);
    return true;
  }
  return false;
}

bool LinkedList::isInBounds(const LLNode* p, const sf::Vector2i mpos) const {
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  if (p) {
    sf::FloatRect globalBounds = p->shape.getGlobalBounds();
    return globalBounds.contains(convertMPos.x, convertMPos.y);
  }
  return false;
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
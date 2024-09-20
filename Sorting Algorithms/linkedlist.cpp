#pragma once
#include "LinkedList.h"

// TODO: is there a way to skip having to pass in my text through LinkedList->LLNode
LinkedList::LinkedList(sf::RenderWindow& win, sf::Text& text) :
  window(win),
  LLText(text),
  nBounds(),
  head(NULL), 
  pActive(NULL), 
  pPrev(NULL),
  state(LLState::ENTRY),
  prevState(LLState::ENTRY),
  current(NULL) {
}
LinkedList::~LinkedList() {
  clear();
}

void LinkedList::clear() {
  LLNode* temp = head;
  while (head) {
    head = head->next;
    delete temp;
    temp = head;
  }
  temp = NULL;
  head = NULL;
  pActive = NULL;
  pPrev = NULL;
  current = NULL;
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
  if (pPrev == current)
    pPrev = NULL;
  if (pActive == current)
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
  LLNode* current = head;
  while (current) {
    currentBounds = current->shape.getGlobalBounds();
    if (currentBounds.contains(convertMPos.x, convertMPos.y)) {
      // set pactive color to white, set new pactive, highlight new pactive
      if (pActive)
        pActive->shape.setFillColor(sf::Color::White);
      pActive = current;
      pActive->shape.setFillColor(sf::Color::Green);
      return pActive;
    }
    pPrev = current;
    current = current->next;
  }
  // null if not found
  return NULL;
}

bool LinkedList::move(LLNode* p, const sf::Vector2i mpos) {
  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  sf::Vector2f lastPos = p->shape.getPosition();
  if (p) {
    p->update(convertMPos, pPrev);
    // handle collision
    sf::FloatRect currentShape = p->shape.getGlobalBounds();
    findNodeBounds(p);
    for (const auto& anyShape : nBounds) {
      if (currentShape.intersects(anyShape))
        p->update(lastPos, pPrev);
    }
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

bool LinkedList::findNodeBounds(LLNode* p) {
  nBounds.clear();
  if (!head)
    return false;

  LLNode* current = head;
  while (current) {
    // don't insert the active node 
    if (current != p)
      nBounds.push_back(current->shape.getGlobalBounds());
    current = current->next;
  }
  return true;
}

bool LinkedList::findValue(const int val) {
  // true means found or finished search space
  if (!head) {
    resetState();
    return true;
  }

  switch (state) {
    case LLState::ENTRY: {
      current = head;
      state = LLState::COMPARE;
      delayClock.restart();
    } break;
    case LLState::HIGHLIGHT: {
      if (current) {
        current->shape.setFillColor(sf::Color::Yellow);
      }
      else {
        resetState();
        return true;
      }
      prevState = state;
      state = LLState::WAIT;
      delayClock.restart();
    } break;
    case LLState::WAIT: {
      if (delayClock.getElapsedTime().asSeconds() >= DELAY) {
        if (prevState == LLState::HIGHLIGHT) {
          prevState = state;
          state = LLState::COMPARE;
        }
        else if (prevState == LLState::COMPARE) {
          prevState = state;
          state = LLState::HIGHLIGHT;
        }
      }
    } break;
    case LLState::COMPARE: {
      if (current->ID == val) {
        current->shape.setFillColor(sf::Color::Magenta);
        resetState();
        std::cout << "found " << val << std::endl;
        return true;
      }
      if (current->next) {
        current->shape.setFillColor(sf::Color::White);
        current = current->next;
      }
      else {
        current->shape.setFillColor(sf::Color::White);
        resetState();
        return true;
      }
      prevState = state;
      state = LLState::WAIT;
      delayClock.restart();
    } break;
  }
  return false;
}

void LinkedList::resetState() {
  current = NULL;
  state = LLState::ENTRY;
  prevState = state;
}
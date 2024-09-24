#pragma once
#include "LLNode.h"

// handles linked list operations
class LinkedList {
public:
  LinkedList(sf::RenderWindow& win, sf::Text& text);
  ~LinkedList();
  void clear();

  void add();
  bool remove();

  LLNode* search(const sf::Vector2i mpos); // find what node im clicking
  void updatePrev(); // moves mPrev to the node behind mActive
  bool move(const sf::Vector2i mpos); // handles LLNode positioning
  bool isInBounds(const sf::Vector2i mpos) const; // checks if mouse is inside mActive Node
  bool findValue(const int val);
  void updateCursor(LLNode* ptr); // cursor follows ptr
  void dtRestart();
  bool resetActive();

  void draw() const;

private:
  bool findNodeBounds();
  void resetState();
  void shiftForward(); // move nodes forward after inserting

 // TODO: composition, compose some of this stuff into structs or delegate to other classes
public:
  LLNode* mActive =  nullptr; // the active selected node

private:
  float dt = 0;
  std::vector<sf::FloatRect> nBounds; // vector of all node bounds
  sf::CircleShape cursor;

  // TODO: maybe make a vector of LLNode* head ptrs?
  LLNode* mHead = nullptr;
  LLNode* mPrev = nullptr; // handles the previous next pointer line

  // LinkedList Operation Animation State Machine
  enum class LLState {
    ENTRY,
    HIGHLIGHT,
    WAIT,
    COMPARE,
    RESET
  };
  LLState state = LLState::ENTRY;
  LLState prevState = LLState::ENTRY;
  sf::Clock delayClock;
  LLNode* mStatePtr = nullptr;

  sf::RenderWindow& window; // DI
  sf::Text LLText; // passes in text object into LLNode
};

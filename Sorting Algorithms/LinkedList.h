#pragma once
#include "LLNode.h"
#include "imgui.h"
#include "imgui-SFML.h"

// handles linked list operations
class LinkedList {
public:
  LinkedList(sf::RenderWindow& win, sf::Text& text);
  ~LinkedList();
  void clear();

  int add();
  bool remove();
  LLNode* search(const sf::Vector2i pos); // find what node im clicking
  bool move(const sf::Vector2i pos); // handles LLNode positioning

  void updateCursor(LLNode* target); // cursor follows ptr
  bool mouseInBounds(const sf::Vector2i mpos) const; // is mouse in mActive node
  bool findValueAnimated(const int val);
  std::vector<std::string> parseString(LLNode* follow); // highlights the followed ptr
  int transformText(const std::vector<std::string>& text);

  int draw() const;
  bool resetActiveNode();
  void dtRestart();

private:
  int updatePrevPtr(LLNode* ptr); // moves prev ptr to a given node
  bool findAllNodeBounds(LLNode* ptr); // excludes passed ptr's nodes bounds 
  bool shiftForward(LLNode* prev, LLNode* curr); // move nodes forward after inserting
  void resetState();

 // TODO: composition, compose some of this stuff into structs or delegate to other classes
public:
  LLNode* mActive =  nullptr; // the active selected node
  LLNode* mStatePtr = nullptr;
  float textScale = 6;
  float prevTextSize = 0;

private:
  sf::Texture texture;
  std::vector<sf::FloatRect> nBounds; // vector of all node bounds
  sf::CircleShape cursor;

  // TODO: maybe make a vector of LLNode* head ptrs?
  LLNode* mHead = nullptr;
  LLNode* mPrev = nullptr; // handles the previous next pointer line
  LLNode* mTail = nullptr; 

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

  sf::RenderWindow& window; // DI
  sf::Text LLText; // passes in text object into LLNode
};

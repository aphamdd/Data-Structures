#pragma once
#include "LLNode.h"
#include "imgui.h"
#include "imgui-SFML.h"

// handles linked list operations
class LinkedList {
private:
  std::unique_ptr<LLNode> head = nullptr;

  // raw traversal pointers
  struct LinkedListPtrs {
    friend class LinkedList;
  public:
    LLNode* active =  nullptr;
    LLNode* currAnimate = nullptr;
  private:
    LLNode* prev = nullptr;
    LLNode* tail = nullptr; 
  };

  sf::CircleShape cursor; 
  // animated timer starts when the cursor reaches its goal
  bool cursorReached = false;

  std::vector<sf::FloatRect> nBounds; // vector of all node bounds

  // scales linked list text data
  float textScale = 5;
  float prevTextSize = 0;

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

public:
  LinkedListPtrs raw;

public:
  LinkedList(sf::RenderWindow& win);
  void clear();

  int add();
  bool remove();
  bool search(const sf::Vector2i pos); // find what node im clicking
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
  void addTail();
};

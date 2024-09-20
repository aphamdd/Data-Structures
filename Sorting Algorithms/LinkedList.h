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
  void insert();

  LLNode* search(const sf::Vector2i mpos); // find what node im clicking
  bool move(LLNode* p, const sf::Vector2i mpos);
  bool isInBounds(const LLNode* p, const sf::Vector2i mpos) const;
  bool findValue(const int val);

  void draw() const;

private:
  bool findNodeBounds(LLNode* p);
  void resetState();

public:
  LLNode* pActive; // the active selected node
private:
  std::vector<sf::FloatRect> nBounds;

  // TODO: maybe make a vector of LLNode* head ptrs?
  LLNode* head;
  LLNode* pPrev; // handles the previous vectorarray line
  sf::RenderWindow& window; // DI
  sf::Text LLText; // passes in text object into LLNode

  enum class LLState {
    ENTRY,
    HIGHLIGHT,
    WAIT,
    COMPARE,
    RESET
  };
  LLState state;
  LLState prevState;
  LLNode* current;
  sf::Clock delayClock;
};

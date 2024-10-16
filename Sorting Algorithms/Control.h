#pragma once

struct Control {
  // Tabs 
  bool isAlgorithm = false;
  bool isLinkedList = false;
  bool isTree = false;
  bool isGraph = false;
  bool isMaze = false;
  
  // Sorting Algs
  bool isPaused = false;
  bool isSorting = false;
  bool isBubble = false;
  bool isSelection = false;
  bool isInsertion = false;
  int bubbleCompares = 0;
  int selectionCompares = 0;
  int insertionCompares = 0;
  float speedMult = 1.0f;
  int numBars = 10;

  // Linked List
  bool isDragging = false;
  bool isSearching = false;
  bool isPopup = false;
  int setNum = 0;
  int findNum = 0;
  sf::Vector2i mpos;

  // Trees
  int setTreeNum = 0;
  int findTreeNum = 0;
  bool isAnimate = false;
  bool isDrag = false;
  bool balanced = false;
  bool full = false;
  bool complete = false;
  bool perfect = false;
};

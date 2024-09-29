#pragma once

struct Control {
  // Tabs 
  bool isAlgorithm = false;
  bool isLinkedList = false;
  bool isBinaryTree = false;
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
  int nodeNum = 0;
  sf::Vector2i mpos;
};

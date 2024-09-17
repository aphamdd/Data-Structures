#pragma once

struct Control {
  bool isAlgorithm = false;
  bool isLinkedList = false;
  bool isBinaryTree = false;
  bool isGraph = false;
  bool isMaze = false;
  
  bool isPaused = false;
  bool isSorting = false;
  bool isBubble = false;
  bool isSelection = false;
  bool isInsertion = false;

  bool isDragging = false;

  int bubbleCompares = 0;
  int selectionCompares = 0;
  int insertionCompares = 0;
  float speedMult = 1.0f;
  int numBars = 10;
};

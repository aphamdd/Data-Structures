#pragma once
struct AlgorithmControl {
  bool isPaused = false;
  bool isSorting = false;
  bool isBubble = false;
  bool isSelection = false;
  float speedMult = 1.0f;
  int numBars = 10;
  int bubbleCompares = 0;
  int selectionCompares = 0;
};

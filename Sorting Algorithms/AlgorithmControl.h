#pragma once
struct AlgorithmControl {
  bool isPaused = false;
  bool isSorting = false;
  bool isBubble = false;
  bool isSelection = false;
  float speedMult = 1.0f;
  int numBars = 10;
};

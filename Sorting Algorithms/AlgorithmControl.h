#pragma once
struct AlgorithmControl {
  bool isPaused = false;
  bool isSorting = false;
  float speedMult = 1.0f;
  int numBars = 10;
  bool pausePlay = false;
};

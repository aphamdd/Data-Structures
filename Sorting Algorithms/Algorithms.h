#pragma once
#include "Graph.h"
#include "AlgorithmControl.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>

class Algorithms {
public:
  // dependency injection, problem is this creates complexity and coupling
  Algorithms(sf::RenderWindow& w, AlgorithmControl& c) :
    window(w),
    control(c),
    i(0), j(0),
    sorted(false),
    lGoal(0.f, 0.f),
    rGoal(0.f, 0.f),
    state(SortState::HIGHLIGHT),
    prevState(state)
  {};
  bool bubbleSort(Graph& graph);
  void selectionSort(Graph& graph);
  void insertionSort(Graph& graph);
  void quickSort(Graph& graph, int low, int high);
  int partition(Graph& graph, int low, int high);
  void reset();
private:
  void swapAnimate(sf::RectangleShape& l, sf::RectangleShape& r, Graph& graph);
  void swap(sf::RectangleShape& l, sf::RectangleShape& r); // swap rectangles
  void copy(sf::RectangleShape& l, const sf::RectangleShape& r); // copy src into dest
  void setGoal(sf::RectangleShape& l, sf::RectangleShape& r);

public:
  sf::Clock delayClock;
private:
  sf::RenderWindow& window;   // DI
  AlgorithmControl& control;  // DI
  int i, j;
  bool sorted;
  sf::Vector2f lGoal;
  sf::Vector2f rGoal;

  // state machine
  enum class SortState {
    HIGHLIGHT,
    WAIT,
    COMPARE,
    SWAP,
    RESET
  };
  SortState state;
  SortState prevState;
};

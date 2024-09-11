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
    i(0), j(0), min(0),
    mCompares(0),
    sorted(false),
    lGoal(0.f, 0.f),
    rGoal(0.f, 0.f),
    state(SortState::ENTRY),
    prevState(state)
  {};
  bool bubbleSort(Graph& graph);
  bool selectionSort(Graph& graph);
  void insertionSort(Graph& graph);
  void quickSort(Graph& graph, int low, int high);
  void binarySearch();
  int partition(Graph& graph, int low, int high);
  void reset();
private:
  void swapAnimate(sf::RectangleShape& l, sf::RectangleShape& r);
  void swap(sf::RectangleShape& l, sf::RectangleShape& r); // swap rectangles
  void copy(sf::RectangleShape& l, const sf::RectangleShape& r); // copy src into dest
  void setGoal(sf::RectangleShape& l, sf::RectangleShape& r);

 // remember to add members to reset() variable state 
public:
  sf::Clock delayClock;
  int mCompares;
private:
  sf::RenderWindow& window;   // DI
  AlgorithmControl& control;  // DI
  // TODO: managing state with all these int members is messy
  int i, j, min;
  bool sorted;
  sf::Vector2f lGoal;
  sf::Vector2f rGoal;

  // state machine
  // NOTE: having an enum element for only one use case is kinda iffy 
  enum class SortState {
    ENTRY,
    HIGHLIGHT,
    WAIT,
    COMPARE,
    SWAP,
    RESET
  };
  SortState state;
  SortState prevState;
};

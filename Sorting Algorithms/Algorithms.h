#pragma once
#include "Graph.h"
#include "AlgorithmControl.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>
#include <map>
#include <string>

// TODO: this is messy
class Algorithms {
private:
  // state machine
  enum class SortState {
    ENTRY,
    HIGHLIGHT,
    WAIT,
    COMPARE,
    SWAP,
    X, // some random temporary state (this is scuffed)
    RESET
  };

  SortState state;
  SortState prevState;
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
    prevState(state),
    displayState({
      {SortState::ENTRY, "ENTRY"},
      {SortState::HIGHLIGHT, "HIGHLIGHT"},
      {SortState::WAIT, "WAIT"},
      {SortState::COMPARE, "COMPARE"},
      {SortState::SWAP, "SWAP"},
      {SortState::RESET, "RESET"}
    })
  {};

  bool bubbleSort(Graph& graph);
  bool selectionSort(Graph& graph);
  bool insertionSort(Graph& graph);
  void quickSort(Graph& graph, int low, int high);
  int partition(Graph& graph, int low, int high);
  void binarySearch();
  SortState getState() { return state; };
  void reset();
private:
  void swap(sf::RectangleShape& l, sf::RectangleShape& r);
  void copy(sf::RectangleShape& dest, sf::RectangleShape& src);// copy src into dest
  bool setGoal(sf::RectangleShape& l, sf::RectangleShape& r);

 // remember to add members to reset() variable state 
public:
  sf::Clock delayClock;
  int mCompares;
  std::map<SortState, std::string> displayState;
private:
  sf::RenderWindow& window;   // DI
  AlgorithmControl& control;  // DI

  // TODO: managing state with all these members is messy
  int i, j, min;
  sf::RectangleShape temp;
  //
  bool sorted;
  sf::Vector2f lGoal;
  sf::Vector2f rGoal;
};

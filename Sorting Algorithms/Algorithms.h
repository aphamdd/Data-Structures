#pragma once
#include "Graph.h"
#include "AlgorithmControl.h"

class Algorithms {
public:
  // dependency injection, problem is this creates complexity and coupling
  Algorithms(sf::RenderWindow& w, AlgorithmControl& c) :
    window(w),
    control(c), 
    i(0), j(0), 
    sorted(false), 
    swapping(false),
    lGoal(0.f, 0.f),
    rGoal(0.f, 0.f)
  {};
  bool bubbleSort(Graph& graph, sf::Clock& dtClock);
  void selectionSort(Graph& graph);
  void insertionSort(Graph& graph);
  void quickSort(Graph& graph, int low, int high);
  int partition(Graph& graph, int low, int high);
  void reset();

private:
  void swapAnimate(sf::Clock& clock, sf::RectangleShape& l, sf::RectangleShape& r, Graph& graph);
  void swap(sf::RectangleShape& l, sf::RectangleShape& r); // swap rectangles
  void copy(sf::RectangleShape& l, const sf::RectangleShape& r); // copy src into dest
  void delay(sf::Clock& clock, const float delay);
  void setGoal(sf::RectangleShape& l, sf::RectangleShape& r);

private:
  sf::RenderWindow& window;
  AlgorithmControl& control;
  int i, j;
  bool sorted;
  bool swapping;
  sf::Vector2f lGoal;
  sf::Vector2f rGoal;
};

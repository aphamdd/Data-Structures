#pragma once
#include "Graph.h"
#include "AlgorithmControl.h"

class Algorithms {
public:
  // dependency injection, problem is this creates complexity and coupling
  Algorithms(sf::RenderWindow& w, AlgorithmControl& c) : window(w), control(c) {}
  void bubbleSort(Graph& graph);
  void selectionSort(Graph& graph);
  void insertionSort(Graph& graph);
  void quickSort(Graph& graph, int low, int high);
  int partition(Graph& graph, int low, int high);

private:
  void swapAnimate(sf::Clock& clock, sf::RectangleShape& l, sf::RectangleShape& r, Graph& graph);
  void swap(sf::RectangleShape& l, sf::RectangleShape& r); // swap rectangles
  void copy(sf::RectangleShape& l, const sf::RectangleShape& r); // copy src into dest
  void delayRedraw(sf::Clock& clock, Graph& graph, const float delay);

private:
  sf::RenderWindow& window;
  AlgorithmControl& control;
};

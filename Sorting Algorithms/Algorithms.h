#pragma once
#include "Graph.h"

class Algorithms {
public:
  void bubbleSort(Graph& graph, sf::RenderWindow& window);
  void selectionSort(Graph& graph, sf::RenderWindow& window);
  void insertionSort(Graph& graph, sf::RenderWindow& window);
  void quickSort(Graph& graph, sf::RenderWindow& window, int low, int high);
  int partition(Graph& graph, sf::RenderWindow& window, int low, int high);

private:
  void swap(Bars& l, Bars& r);
  void copy(Bars& l, const Bars& r);
};

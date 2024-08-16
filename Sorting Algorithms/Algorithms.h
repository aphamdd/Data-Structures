#pragma once
#include "Graph.h"

class Algorithms {
public:
  void bubbleSort(Graph& graph, sf::RenderWindow& window);
  void selectionSort(Graph& graph, sf::RenderWindow& window);
  void insertionSort(Graph& graph, sf::RenderWindow& window);
  void quickSort(Graph& graph, sf::RenderWindow& window, int low, int high);
  int partition(Graph& graph, sf::RenderWindow& window, int low, int high);
  void countingSort(Graph& graph, sf::RenderWindow& window);
  void radixSort(Graph& graph, sf::RenderWindow& window);
  void mergeSort(Graph& graph, sf::RenderWindow& window);

private:
  void swap(Bars& l, Bars& r);
  void copy(Bars& l, const Bars& r);
};

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
  void aniTimer(sf::Clock& clock, const float delay);
  void swap(sf::RectangleShape& l, sf::RectangleShape& r);
  void copy(sf::RectangleShape& l, const sf::RectangleShape& r);
};

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
  void swapAnimate(sf::RenderWindow& window, sf::Clock& clock, sf::RectangleShape& l, sf::RectangleShape& r, Graph& graph);
  void swap(sf::RectangleShape& l, sf::RectangleShape& r); // swap rectangles
  void copy(sf::RectangleShape& l, const sf::RectangleShape& r); // copy src into dest
  void delayRedraw(sf::RenderWindow& window, sf::Clock& clock, Graph& graph, const float delay);
};

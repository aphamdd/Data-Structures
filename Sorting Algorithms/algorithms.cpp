#pragma once
#include "Algorithms.h"
#include <iostream>
using namespace std;

void Algorithms::bubbleSort(Graph& graph, sf::RenderWindow& window) {
  int n = graph.m_histogram.size();
  for (int i = 0; i < n; ++i) {
    bool flag = false;
    for (int j = 0; j < n - i - 1; ++j) {
      int curr = graph.m_histogram[j].m_shape.getSize().y;
      int adj = graph.m_histogram[j + 1].m_shape.getSize().y;
      if (curr > adj) {
        swap(graph.m_histogram[j], graph.m_histogram[j + 1]);
        flag = true;

        window.clear(sf::Color::Black);
        window.draw(graph);
        window.display();
      }
    }
    if (!flag)
      break;
  }
}

void Algorithms::selectionSort(Graph& graph, sf::RenderWindow& window) {
  int n = graph.m_histogram.size();
  for (int i = 0; i < n; ++i) {
    int min = i;
    for (int j = i + 1; j < n; ++j) {
      int curr = graph.m_histogram[j].m_shape.getSize().y;
      int compare = graph.m_histogram[min].m_shape.getSize().y;
      if (curr < compare)
        min = j;
    }
    swap(graph.m_histogram[i], graph.m_histogram[min]);
    window.clear(sf::Color::Black);
    window.draw(graph);
    window.display();
  }
}

void Algorithms::insertionSort(Graph& graph, sf::RenderWindow& window) {
  /* THIS SUCCESSFULLY SWAPS 2 BARS
  cout << "\n" << graph.m_histogram[0].m_shape.getSize().y << "," <<
                  graph.m_histogram[2].m_shape.getSize().y << "\n";

  swap(graph.m_histogram[0], graph.m_histogram[2]);

  cout << "SWAPPED" << endl;
  cout << graph.m_histogram[0].m_shape.getSize().y << "," <<
                  graph.m_histogram[2].m_shape.getSize().y << "\n";
  */

  /* TODO: HOW DO I COPY AN INDEX WITH ANOTHER?
  // using std::swap makes the obj at original index and target index properly fill its data
  // however, when trying to use an assignment =, the original index doesnt draw the shape.
  cout << "\n" << graph.m_histogram[0].m_shape.getSize().y << "," <<
                  graph.m_histogram[2].m_shape.getSize().y << "\n";

  // ok so, with the current code below, I'm able to see that index 0 DOES
  // copy index 2. However, whatever was inside of index 2 disappears.
  // I expected index 0 to overlap whatever is in index 2, but it just
  // REPLACED whatever is inside index 2 instead. HMMMM.
  // index 0 IS index 2, including its position and all that good stuff
  Bars temp = graph.m_histogram[0];
  graph.m_histogram[0] = graph.m_histogram[2];
  for (int i = 0; i < 1000; ++i) {
    window.clear(sf::Color::Black);
    window.draw(graph.m_histogram[0].m_shape);
    window.display();
  }
  //
  
  //copy(graph.m_histogram[0], graph.m_histogram[2]);

  cout << "COPIED" << endl;
  cout << graph.m_histogram[0].m_shape.getSize().y << "," <<
                  graph.m_histogram[2].m_shape.getSize().y << "\n";
  cout << graph.m_histogram[0].m_shape.getPosition().y << endl;
  */

  int size = graph.m_histogram.size();
  for (int step = 1; step < size; step++) {
    Bars k = graph.m_histogram[step];
    int key = graph.m_histogram[step].m_shape.getSize().y;
    int j = step - 1;
    while (key < graph.m_histogram[j].m_shape.getSize().y && j >= 0) {
      copy(graph.m_histogram[j + 1], graph.m_histogram[j]);
      --j;
      window.clear(sf::Color::Black);
      window.draw(graph);
      window.display();
    }
    copy(graph.m_histogram[j + 1], k);
    window.clear(sf::Color::Black);
    window.draw(graph);
    window.display();
  }
}

void Algorithms::swap(Bars& l, Bars& r) {
  sf::Vector2f temp = l.m_shape.getPosition();
  l.m_shape.setPosition(r.m_shape.getPosition());
  r.m_shape.setPosition(temp);
  std::swap(l, r);
}

// copy the characteristics of a Bars, but not the position
void Algorithms::copy(Bars& l, const Bars& r) {
  l.m_pos = r.m_pos;
  l.m_size = r.m_size;
  l.m_shape.setFillColor(r.m_shape.getFillColor());
  l.m_shape.setOutlineThickness(r.m_shape.getOutlineThickness());
  l.m_shape.setOutlineColor(r.m_shape.getOutlineColor());
  l.m_shape.setSize(r.m_shape.getSize());
  l.m_shape.setOrigin(r.m_shape.getOrigin());
}

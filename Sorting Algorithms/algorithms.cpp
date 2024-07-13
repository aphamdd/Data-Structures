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

void Algorithms::swap(Bars& l, Bars& r) {
  sf::Vector2f temp = l.m_shape.getPosition();
  l.m_shape.setPosition(r.m_shape.getPosition());
  r.m_shape.setPosition(temp);
  std::swap(l, r);
}

void Algorithms::insertionSort(Graph& graph, sf::RenderWindow& window) {
  // THIS SUCCESSFULLY SWAPS 2 BARS
  cout << "\n" << graph.m_histogram[0].m_shape.getSize().y << "," <<
                  graph.m_histogram[2].m_shape.getSize().y << "\n";

  swap(graph.m_histogram[0], graph.m_histogram[2]);

  cout << "SWAPPED" << endl;
  cout << graph.m_histogram[0].m_shape.getSize().y << "," <<
                  graph.m_histogram[2].m_shape.getSize().y << "\n";
  //

  /* TODO: HOW DO I COPY AN INDEX WITH ANOTHER?
  // using std::swap makes the obj at original index and target index properly fill its data
  // however, when trying to use an assignment =, the original index doesnt draw the shape.
  cout << "\n" << graph.m_histogram[0].m_shape.getSize().y << "," <<
                  graph.m_histogram[2].m_shape.getSize().y << "\n";

  graph.m_histogram[0] = graph.m_histogram[2];

  cout << "COPIED" << endl;
  cout << graph.m_histogram[0].m_shape.getSize().y << "," <<
                  graph.m_histogram[2].m_shape.getSize().y << "\n";
  */

  /* insertion sort implementation, incomplete
  int n = graph.m_histogram.size();
  for (int i = 1; i < n; ++i) {
    int insert_index = i;
    int current_value = graph.m_histogram[i].m_shape.getSize().y;
    for (int j = i - 1; j >= 0; --j) {
      if (graph.m_histogram[j].m_shape.getSize().y > current_value) {
        graph.m_histogram[j + 1].m_shape.setPosition(graph.m_histogram[j].m_shape.getPosition());
        graph.m_histogram[j + 1] = graph.m_histogram[j];
        insert_index = j;
      }
      else
        break;
    }
    graph.m_histogram[insert_index].m_shape.setPosition(graph.m_histogram[i].m_shape.getPosition());
    graph.m_histogram[insert_index] = graph.m_histogram[i];

    window.clear(sf::Color::Black);
    window.draw(graph);
    window.display();
  }
  */
}

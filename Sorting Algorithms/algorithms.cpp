#pragma once
#include "Algorithms.h"
#include <iostream>
using namespace std;

void Algorithms::bubbleSort(Graph& graph, sf::RenderWindow& window) {
  int n = graph.m_histogram.size();
  for (int i = 0; i < n; ++i) {
    bool swap = false;
    for (int j = 0; j < n - i - 1; ++j) {
      int curr = graph.m_histogram[j].m_shape.getSize().y;
      int adj = graph.m_histogram[j + 1].m_shape.getSize().y;
      if (curr > adj) {
        sf::Vector2f temp = graph.m_histogram[j].m_shape.getPosition();
        graph.m_histogram[j].m_shape.setPosition(graph.m_histogram[j + 1].m_shape.getPosition());
        graph.m_histogram[j + 1].m_shape.setPosition(temp);
        std::swap(graph.m_histogram[j], graph.m_histogram[j + 1]);
        swap = true;

        window.clear(sf::Color::Black);
        window.draw(graph);
        window.display();
      }
    }
    if (!swap)
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
    sf::Vector2f temp = graph.m_histogram[i].m_shape.getPosition();
    graph.m_histogram[i].m_shape.setPosition(graph.m_histogram[min].m_shape.getPosition());
    graph.m_histogram[min].m_shape.setPosition(temp);
    std::swap(graph.m_histogram[i], graph.m_histogram[min]);
    window.clear(sf::Color::Black);
    window.draw(graph);
    window.display();
  }
}

// PROBLEM: I think I'm not updating the vector properly when I reassign
void Algorithms::insertionSort(Graph& graph, sf::RenderWindow& window) {
  int n = graph.m_histogram.size();
  for (int step = 1; step < n; ++step) {
    Bars k = graph.m_histogram[step];
    int key = k.m_shape.getSize().y;
    int j = step - 1;
    int curr = graph.m_histogram[j].m_shape.getSize().y;
    while (key < curr && j >= 0) {
      graph.m_histogram.at(j + 1).m_shape.setPosition(graph.m_histogram.at(j).m_shape.getPosition());
      graph.m_histogram.at(j + 1) = graph.m_histogram.at(j);
      --j;
      window.clear(sf::Color::Black);
      window.draw(graph);
      window.display();
    }
    graph.m_histogram[j + 1].m_shape.setPosition(graph.m_histogram[step].m_shape.getPosition());
    graph.m_histogram[j + 1] = k;

    window.clear(sf::Color::Black);
    window.draw(graph);
    window.display();
  }
}

#pragma once
#include "Algorithms.h"
#include <iostream>
#include <cmath>
using namespace std;

void Algorithms::bubbleSort(Graph& graph, sf::RenderWindow& window) {
  int n = graph.m_histogram.size();
  sf::Clock m_clock; // starts the timer
  for (int i = 0; i < n; ++i) {
    bool flag = false;
    for (int j = 0; j < n - i - 1; ++j) {
      int curr = graph.m_histogram[j].getSize().y;
      int adj = graph.m_histogram[j + 1].getSize().y;

      // highlight selected bars
      graph.m_histogram[j].setFillColor(sf::Color::Yellow);
      graph.m_histogram[j + 1].setFillColor(sf::Color::Yellow);
      delayRedraw(window, m_clock, graph, DELAY);

      if (curr > adj) {
        delayRedraw(window, m_clock, graph, DELAY); // make this a standalone delay
        swapAnimate(window, m_clock, graph.m_histogram[j], graph.m_histogram[j + 1], graph);
        flag = true;
      }

      // update with swap/no swap
      delayRedraw(window, m_clock, graph, DELAY);

      // revert highlight
      graph.m_histogram[j].setFillColor(sf::Color::White);
      graph.m_histogram[j + 1].setFillColor(sf::Color::White);
      delayRedraw(window, m_clock, graph, DELAY);
    }
    if (!flag)
      break;

    // rerender end of the pass
    delayRedraw(window, m_clock, graph, DELAY);
  }
}

void Algorithms::selectionSort(Graph& graph, sf::RenderWindow& window) {
  int n = graph.m_histogram.size();
  for (int i = 0; i < n; ++i) {
    int min = i;
    for (int j = i + 1; j < n; ++j) {
      int curr = graph.m_histogram[j].getSize().y;
      int compare = graph.m_histogram[min].getSize().y;
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
  int size = graph.m_histogram.size();
  for (int step = 1; step < size; step++) {
    sf::RectangleShape k = graph.m_histogram.at(step);
    int key = graph.m_histogram.at(step).getSize().y;
    int j = step - 1;
    while (j >= 0 && key < graph.m_histogram.at(j).getSize().y) {
      copy(graph.m_histogram.at(j + 1), graph.m_histogram.at(j));
      --j;
    }
    copy(graph.m_histogram.at(j + 1), k);
    window.clear(sf::Color::Black);
    window.draw(graph);
    window.display();
  }
}

int Algorithms::partition(Graph& graph, sf::RenderWindow& window, int low, int high) {
  int pivot = graph.m_histogram[high].getSize().y;
  int i = low - 1;

  for (int j = low; j < high; ++j) {
    if (graph.m_histogram[j].getSize().y <= pivot) {
      ++i;
      swap(graph.m_histogram[i], graph.m_histogram[j]);
      window.clear(sf::Color::Black);
      window.draw(graph);
      window.display();
    }
  }
  swap(graph.m_histogram[i + 1], graph.m_histogram[high]);
  return i + 1;
}

void Algorithms::quickSort(Graph& graph, sf::RenderWindow& window, int low, int high) {
  if (low < high) {
    int pivot = partition(graph, window, low, high);
    quickSort(graph, window, low, pivot - 1);
    quickSort(graph, window, pivot + 1, high);
  }
}

void Algorithms::swapAnimate(sf::RenderWindow& window, sf::Clock& clock, sf::RectangleShape& l, sf::RectangleShape& r, Graph& graph) {
  sf::Vector2f rGoal = l.getPosition(); // r -> l
  sf::Vector2f lGoal = r.getPosition(); // l -> r
  sf::Vector2f lPos = rGoal, rPos = lGoal;
  sf::Clock dtClock;
  float dt;
  // TODO: make these values user adjustable
  float lVelocity = 500, rVelocity = 500;

  // if the r object is on the right of the l object
  if (lPos.x < rPos.x) {
    rVelocity *= -1;
    while (lPos.x < lGoal.x && rPos.x > rGoal.x) {
      dt = dtClock.restart().asSeconds();
      lPos.x += lVelocity*dt;
      rPos.x += rVelocity*dt;
      l.setPosition(lPos);
      r.setPosition(rPos);

      // if the velocity sends the objects past their spots, 
      // just set them to their intended positions
      if (lPos.x > lGoal.x || rPos.x < rGoal.x) {
        l.setPosition(lGoal);
        r.setPosition(rGoal);
      }

      delayRedraw(window, clock, graph, 0);
    }
  }
  else if (lPos.x > rPos.x) {
    lVelocity *= -1;
    while (lPos.x > lGoal.x && rPos.x < rGoal.x) {
      dt = dtClock.restart().asSeconds();
      lPos.x += lVelocity*dt;
      rPos.x += rVelocity*dt;
      l.setPosition(lPos);
      r.setPosition(rPos);

      if (lPos.x < lGoal.x || rPos.x > rGoal.x) {
        l.setPosition(lGoal);
        r.setPosition(rGoal);
      }

      delayRedraw(window, clock, graph, 0);
    }
  }
  else
    std::cout << "erm i'm swapping the same thing" << std::endl;

  std::swap(l, r); // the actual swap in memory is done here
}

void Algorithms::swap(sf::RectangleShape& l, sf::RectangleShape& r) {
  sf::Vector2f temp = l.getPosition();
  l.setPosition(r.getPosition());
  r.setPosition(temp);

  std::swap(l, r); // the actual swap in memory is done here
}

// copy the characteristics of a Bar, but not the position
void Algorithms::copy(sf::RectangleShape& l, const sf::RectangleShape& r) {
  l.setFillColor(r.getFillColor());
  l.setOutlineThickness(r.getOutlineThickness());
  l.setOutlineColor(r.getOutlineColor());
  l.setSize(r.getSize());
  l.setOrigin(r.getOrigin());
}

void Algorithms::delayRedraw(sf::RenderWindow& w, sf::Clock& c, Graph& g, const float d) {
  while (c.getElapsedTime().asSeconds() < d) { continue; }
  c.restart();

  w.clear(sf::Color::Black);
  w.draw(g);
  w.display();
}

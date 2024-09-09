#pragma once
#include "Algorithms.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
using namespace std;

// TODO: need pauses
bool Algorithms::bubbleSort(Graph& graph, sf::Clock& clock) {
  if (sorted) return true;
  int n = graph.m_histogram.size();
  if (i < n) {
    if (!swapping) {
      if (j < n - i - 1) {
        int curr = graph.m_histogram[j].getSize().y;
        int adj = graph.m_histogram[j + 1].getSize().y;

        // highlight selected bars
        graph.m_histogram[j].setFillColor(sf::Color::Yellow);
        graph.m_histogram[j + 1].setFillColor(sf::Color::Yellow);

        // if swapping
        if (curr > adj) {
          swapping = true;
          setGoal(graph.m_histogram[j], graph.m_histogram[j + 1]);
        } else {
          // next element if no swap
          graph.m_histogram[j].setFillColor(sf::Color::White);
          graph.m_histogram[j + 1].setFillColor(sf::Color::White);
          ++j;
        }
      } else {
        // reset inner loop, move to next pass
        j = 0;
        ++i;
      }
    } else {
      swapAnimate(clock, graph.m_histogram[j], graph.m_histogram[j + 1], graph);
    }
  } else {
    sorted = true;
  }

  return false;
}

void Algorithms::selectionSort(Graph& graph) {
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

void Algorithms::insertionSort(Graph& graph) {
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

int Algorithms::partition(Graph& graph, int low, int high) {
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

void Algorithms::quickSort(Graph& graph, int low, int high) {
  if (low < high) {
    int pivot = partition(graph, low, high);
    quickSort(graph, low, pivot - 1);
    quickSort(graph, pivot + 1, high);
  }
}

void Algorithms::setGoal(sf::RectangleShape& l, sf::RectangleShape& r) {
  // set this up to ensure which shapes needs to move left or right
  if (l.getPosition().x < r.getPosition().x) {
    lGoal = r.getPosition();
    rGoal = l.getPosition();
  } else {
    lGoal = l.getPosition();
    rGoal = r.getPosition();
  }
}

void Algorithms::swapAnimate(sf::Clock& clock, sf::RectangleShape& l, sf::RectangleShape& r, Graph& graph) {
  sf::Vector2f lPos = l.getPosition();
  sf::Vector2f rPos = r.getPosition();
  float lVelocity = 100 * control.speedMult;
  float rVelocity = 100 * -1 * control.speedMult; // right object will always move backwards
  float dt;

  // TODO: deltatime is scuffed af (maybe set floor of velocity to be at least 1?)
  // swapping
  if (lPos.x != lGoal.x && rPos.x != rGoal.x) {
    dt = clock.restart().asSeconds();
    lPos.x += lVelocity * dt;
    rPos.x += rVelocity * dt;
    //lPos.x += round(lVelocity);
    //rPos.x += round(rVelocity);
    l.setPosition(lPos);
    r.setPosition(rPos);
  }
 
  // TODO: check if algorithm is paused/stopped, then finish the mem swap and sprite swap
  // if at or past their goals, then finished
  if (lPos.x >= lGoal.x && rPos.x <= rGoal.x) {
    swapping = false; // done
    l.setPosition(lGoal);
    r.setPosition(rGoal);
    l.setFillColor(sf::Color::White);
    r.setFillColor(sf::Color::White);
    std::swap(l, r); // the actual swap in memory is done here
  }
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

void Algorithms::delay(sf::Clock& clock, const float delay) {
  while (clock.getElapsedTime().asSeconds() < delay) { continue; }
  clock.restart();
}

void Algorithms::reset() { 
  i = 0; j = 0; 
  sorted = false; swapping = false; 
  lGoal = (sf::Vector2f(0.f, 0.f)); rGoal = (sf::Vector2f(0.f, 0.f));
};

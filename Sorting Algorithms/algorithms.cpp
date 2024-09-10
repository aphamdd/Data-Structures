#pragma once
#include "Algorithms.h"
using namespace std;

bool Algorithms::bubbleSort(Graph& graph) {
  if (sorted) return true;
  int n = graph.m_histogram.size();
  if (i < n) {
    switch (state) {
      case SortState::HIGHLIGHT: {
        // highlight selected bars and then wait
        graph.m_histogram[j].setFillColor(sf::Color::Yellow);
        graph.m_histogram[j + 1].setFillColor(sf::Color::Yellow);

        prevState = state;
        state = SortState::WAIT;
        delayClock.restart();
      } break;
      case SortState::WAIT: {
        if (delayClock.getElapsedTime().asSeconds() >= DELAY) {
          // highlight -> compare
          if (prevState == SortState::HIGHLIGHT) {
            prevState = state;
            state = SortState::COMPARE;
          }
          // compare done -> reset state
          else if (prevState == SortState::COMPARE || prevState == SortState::SWAP) {
            prevState = state;
            state = SortState::RESET;
          }
          // in the middle of sorting -> highlight
          else if (prevState == SortState::RESET) {
            prevState = state;
            state = SortState::HIGHLIGHT;
          }
        }
      } break;
      case SortState::COMPARE: {
        int curr = graph.m_histogram[j].getSize().y;
        int adj = graph.m_histogram[j + 1].getSize().y;
        // if swapping
        if (curr > adj) {
          prevState = state;
          state = SortState::SWAP;
          setGoal(graph.m_histogram[j], graph.m_histogram[j + 1]);
        }
        else {
          // next element if no swap
          prevState = state;
          state = SortState::WAIT;
        }
        delayClock.restart();
      } break;
      case SortState::SWAP: {
        swapAnimate(graph.m_histogram[j], graph.m_histogram[j + 1], graph);
        delayClock.restart();
        // wait after finishing swap animation
      } break;
      case SortState::RESET: {
        // wait after finishing step
        graph.m_histogram[j].setFillColor(sf::Color::White);
        graph.m_histogram[j + 1].setFillColor(sf::Color::White);
        ++j;
        if (j >= n - i - 1) {
          j = 0;
          ++i;
        }
        if (i >= n - 1)
          sorted = true;

        prevState = state;
        state = SortState::WAIT;
        delayClock.restart();
      } break;
    }
  }
  else {
    sorted = true;
    return true;
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
  lGoal = r.getPosition();
  rGoal = l.getPosition();
}

// this is under the assumption that l starts on the left of r
// solution is to check before going into the function if l is actually left of r
void Algorithms::swapAnimate(sf::RectangleShape& l, sf::RectangleShape& r, Graph& graph) {
  sf::Vector2f lPos = l.getPosition();
  sf::Vector2f rPos = r.getPosition();
  float lVelocity = 100 * control.speedMult;
  float rVelocity = 100 * -1 * control.speedMult;
  float dt;

  // swapping
  if (lPos.x < lGoal.x && rPos.x > rGoal.x) {
    dt = delayClock.restart().asSeconds();
    lPos.x += lVelocity * dt;
    rPos.x += rVelocity * dt;
    l.setPosition(lPos);
    r.setPosition(rPos);
  } else {
  // TODO: check if algorithm is paused/stopped, then finish the mem swap and sprite swap
  // if at or past their goals, then finished
    prevState = state;
    state = SortState::WAIT; // done
    l.setPosition(lGoal);
    r.setPosition(rGoal);
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

void Algorithms::reset() { 
  i = 0; j = 0; 
  sorted = false;
  lGoal = (sf::Vector2f(0.f, 0.f)); rGoal = (sf::Vector2f(0.f, 0.f));
  delayClock.restart();
  state = SortState::HIGHLIGHT;
  prevState = state;
};

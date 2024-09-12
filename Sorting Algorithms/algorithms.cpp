#pragma once
#include "Algorithms.h"
using namespace std;

// TODO: bubble sort improvement
bool Algorithms::bubbleSort(Graph& graph) {
  int n = graph.m_histogram.size();
  if (sorted || n <= 1) {
    sorted = true;
    return true;
  }
  if (i < n) {
    switch (state) {
      // standardized entry point
      case SortState::ENTRY: {
        prevState = state;
        state = SortState::HIGHLIGHT;
        delayClock.restart();
      } break;
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
            state = SortState::ENTRY;
          }
        }
      } break;
      case SortState::COMPARE: {
        ++mCompares;
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
        swapAnimate(graph.m_histogram[j], graph.m_histogram[j + 1]);
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

// TODO: colors and pauses aren't quite done yet
bool Algorithms::selectionSort(Graph& graph) {
  int n = graph.m_histogram.size();
  if (sorted || n <= 1) {
    sorted = true;
    return true;
  }
  if (i < n) {
    switch (state) {
    // proceed outer loop
    case SortState::ENTRY: {
      min = i;
      j = i + 1;

      prevState = state;
      state = SortState::HIGHLIGHT;
      delayClock.restart();
    } break;
    case SortState::HIGHLIGHT: {
      // highlight anchor, else highlight current value to compare
      if (prevState == SortState::ENTRY) {
        prevState = state;
        graph.m_histogram[min].setFillColor(sf::Color::Yellow);
      }
      else if (prevState == SortState::WAIT)
        graph.m_histogram[j].setFillColor(sf::Color::Yellow);

      state = SortState::WAIT;
      delayClock.restart();
    } break;
    case SortState::WAIT: {
      if (delayClock.getElapsedTime().asSeconds() >= DELAY) {
        if (prevState == SortState::HIGHLIGHT || prevState == SortState::COMPARE) {
          prevState = state;
          state = SortState::HIGHLIGHT;
        }
        else if (prevState == SortState::WAIT) {
          prevState = state;
          state = SortState::COMPARE;
        }
        else if (prevState == SortState::SWAP) {
          prevState = state;
          state = SortState::RESET;
        }
      }
    } break;
    case SortState::COMPARE: {
      ++mCompares;
      int curr = graph.m_histogram[j].getSize().y;
      int anchor = graph.m_histogram[min].getSize().y;
      if (curr < anchor) // update smallest index
        min = j;
      graph.m_histogram[j].setFillColor(sf::Color::White);
      ++j;
      // continue inner loop
      if (j < n) {
        prevState = state;
        state = SortState::WAIT;
        delayClock.restart();
      }
      else {
        prevState = state;
        state = SortState::SWAP;
        graph.m_histogram[min].setFillColor(sf::Color::Green);
        setGoal(graph.m_histogram[i], graph.m_histogram[min]);
        delayClock.restart(); 
      }
    } break;
    case SortState::SWAP: {
      swapAnimate(graph.m_histogram[i], graph.m_histogram[min]);
      delayClock.restart(); 
    } break;
    case SortState::RESET: {
      graph.m_histogram[min].setFillColor(sf::Color::White);
      graph.m_histogram[i].setFillColor(sf::Color::White);
      j = 0;
      ++i;
      if (i >= n - 1)
        sorted = true;

      prevState = state;
      state = SortState::ENTRY;
      delayClock.restart(); 
    } break;
    }
  }
  else {
    sorted = true;
    return true;
  }
  return false;

  /* code without state machine approach lmao
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
  }
  */
}

bool Algorithms::insertionSort(Graph& graph) {
  int n = graph.m_histogram.size();
  if (sorted || n <= 1) {
    sorted = true;
    return true;
  }

  // it's the first entry to the method
  if (i == 0)
    i = 1;
  if (i < n) {
    switch (state) {
    case SortState::ENTRY: {
      j = i - 1;
      current = graph.m_histogram.at(i); 
      prevState = state;
      state = SortState::COMPARE;
      delayClock.restart();
    } break;
    case SortState::COMPARE: {
      if (j >= 0 && current.getSize().y < graph.m_histogram.at(j).getSize().y) {
        // copy(j+1, j); loop;
        copy(graph.m_histogram.at(j + 1), graph.m_histogram.at(j));
        --j;
        state = SortState::COMPARE;
        // JUMP BACK TO THIS CONDITIONAL
      }
      else { // FOUND THE ISSUE, THIS SHOULDNT EVEN BE IN ANY CONDITIONAL
        // copy(j+1, i); outer loop;
        copy(graph.m_histogram.at(j + 1), current);
        state = SortState::RESET;
      }
      delayClock.restart();
    } break;
    case SortState::RESET: {
      ++i;
      if (i >= n) {
        sorted = true;
        return true;
      }
      prevState = state;
      state = SortState::ENTRY;
      delayClock.restart();
    } break;
    }
  }
  else {
    sorted = true;
    return true;
  }
  return false;

  /*
  // insertion sort uses 2 pointers
  // a i pointer to hold onto the value currently being sorted
  // a j pointer that goes backwards to compare
  int n = graph.m_histogram.size();
  for (int i = 1; i < n; ++i) {
    int current = graph.m_histogram.at(i).getSize().y;
    int j = i - 1;
    while (j >= 0 && current < graph.m_histogram.at(j).getSize().y) {
      // copy shifts the values greater than the compared value forward
      // until we find the proper spot for the value at index i
      copy(graph.m_histogram.at(j + 1), graph.m_histogram.at(j));
      --j;
    }
    // if the inner loop ever gets entered, then we've created a slot
    // for the smaller value on the right (unsorted side) to slot into the 
    // left (sorted side) by shifting j back x amount of times
    // otherwise it does nothing and continues cuz j+1 == i since j never changes
    copy(graph.m_histogram.at(j + 1), graph.m_histogram.at(i));
  }
  */
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
void Algorithms::swapAnimate(sf::RectangleShape& l, sf::RectangleShape& r) {
  sf::Vector2f lPos = l.getPosition();
  sf::Vector2f rPos = r.getPosition();
  // TODO: handle velocity in relation to width of bars
  // I can do this by calculating the distance between two bars and scale it
  float lVelocity = 100 * control.speedMult;
  float rVelocity = 100 * -1 * control.speedMult;
  float dt;

  // swapping, else done swapping
  if (lPos.x < lGoal.x && rPos.x > rGoal.x) {
    dt = delayClock.restart().asSeconds();
    lPos.x += lVelocity * dt;
    rPos.x += rVelocity * dt;
    l.setPosition(lPos);
    r.setPosition(rPos);
  } else {
  // TODO: check if algorithm is paused/stopped, then finish the mem swap and sprite swap
  // if at or past their goals, then finished
    prevState = state; // prevState = SWAP
    state = SortState::WAIT; 
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
  i = 0; j = 0; min = 0;
  sorted = false;
  lGoal = (sf::Vector2f(0.f, 0.f)); rGoal = (sf::Vector2f(0.f, 0.f));
  delayClock.restart();
  state = SortState::ENTRY;
  prevState = state;
  mCompares = 0;
};

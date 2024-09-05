#include "common.h"
#include "Graph.h"
#include "Algorithms.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Algorithm Visualizer", sf::Style::Default);
  window.setFramerateLimit(FPS);

  // set graph
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> num(100, 500);
  const int numBars = 150;
  Graph graph(numBars);
  Algorithms algo;

  // show timer
  sf::Clock clock;
  sf::Font font;
  if (!font.loadFromFile("./Fonts/Retale-Regular.ttf"))
    throw("COULDN'T LOAD FONT");
  sf::Text elapsedTime;
  elapsedTime.setFont(font);
  elapsedTime.setCharacterSize(50);
  elapsedTime.setFillColor(sf::Color::Cyan);

  cout << "B: Bubble\nS: Selection\nI: Insertion\nQ: Quick\nEsc: Shuffle\n" << endl;
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::B:
          cout << "Bubble Sort... ";
          algo.bubbleSort(graph, window);
          cout << "DONE" << endl;
          break;
        case sf::Keyboard::S:
          cout << "Selection Sort... ";
          algo.selectionSort(graph, window);
          cout << "DONE" << endl;
          break;
        case sf::Keyboard::I:
          cout << "Insertion Sort... ";
          algo.insertionSort(graph, window);
          cout << "DONE" << endl;
          break;
        case sf::Keyboard::Q:
          cout << "Quick Sort... ";
          algo.quickSort(graph, window, 0, graph.m_histogram.size() - 1);
          cout << "DONE" << endl;
          break;
        case sf::Keyboard::Escape:
          cout << "Shuffling... ";
          graph.shuffle();
          cout << "DONE" << endl;
          break;
        case sf::Keyboard::D:
          cout << "Debug... ";
          for (int i = 0; i < graph.m_histogram.size(); ++i) {
            cout << i << " ";
          }
          cout << "DONE" << endl;
          break;
        default:
          break;
        }
      }
    }

    sf::Time elapsed = clock.getElapsedTime();
    elapsedTime.setString(to_string(elapsed.asSeconds()));

    // rendering
    window.clear(sf::Color::Black);
    window.draw(graph);
    window.draw(elapsedTime);
    window.display();
  }
  return 0;
}
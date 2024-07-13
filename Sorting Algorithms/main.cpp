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
  const int numBars = 5;
  Graph graph(numBars);
  Algorithms algo;

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::B)
          algo.bubbleSort(graph, window);
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::S)
          algo.selectionSort(graph, window);
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::I)
          algo.insertionSort(graph, window);
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape)
          graph.reset();
      }
    }

    // rendering
    window.clear(sf::Color::Black);
    window.draw(graph);
    window.display();
  }
  return 0;
}
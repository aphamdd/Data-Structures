#include "common.h"
#include "Graph.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Algorithm Visualizer", sf::Style::Default);
  window.setFramerateLimit(FPS);

  // set graph
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> num(5, 50);
  const int numBars = num(rng);
  Graph graph(numBars);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }


    // rendering
    window.clear(sf::Color::Black);
    window.draw(graph);
    window.display();
  }
  return 0;
}
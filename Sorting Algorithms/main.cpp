#include "common.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Boid Simluator", sf::Style::Default);
  window.setFramerateLimit(FPS);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // rendering
    window.clear(sf::Color::Black);
    window.display();
  }
  return 0;
}
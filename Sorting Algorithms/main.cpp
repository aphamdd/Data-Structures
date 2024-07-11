#include "common.h"
#include "Bars.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Boid Simluator", sf::Style::Default);
  window.setFramerateLimit(FPS);

  // set Bars
  sf::Vector2f pos(10, 10);
  sf::Vector2f size(10, 100);
  Bars bars(pos, size);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // rendering
    window.clear(sf::Color::Black);
    window.draw(bars);
    window.display();
  }
  return 0;
}
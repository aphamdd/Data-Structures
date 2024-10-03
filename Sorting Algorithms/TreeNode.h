#pragma once
#include "SFML/Graphics.hpp"
#include "Control.h"
#include "common.h"
#include <iostream>

// handles the node object and drawing
class TreeNode : public sf::Drawable, public sf::Transformable {
  friend class BST;
public:
  TreeNode(const sf::Vector2f pos, sf::Text& text, sf::Texture& texture);

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  // node data
  // how can inheritance work here? i only have a left and right ptr,
  // will I need a vector of ptrs to expand from?
  std::unique_ptr<TreeNode> left = nullptr;
  std::unique_ptr<TreeNode> right = nullptr;
  int data = 0;

  // rendering 
  sf::Sprite sprite;
  sf::Text dataText;
  sf::VertexArray leftLine;
  sf::VertexArray rightLine;
  sf::Vector2f size;
};
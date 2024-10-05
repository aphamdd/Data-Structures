#pragma once
#include "SFML/Graphics.hpp"
#include "Control.h"
#include "common.h"
#include <iostream>

// handles the node object and drawing
// Tree node (int data, sf::Vector2f position)
class TreeNode : public sf::Drawable, public sf::Transformable {
  friend class BST;
public:
  TreeNode(const int val, const sf::Vector2f pos);

private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  // node data
  // how can inheritance work here? i only have a left and right ptr,
  // will I need a vector of ptrs to expand from?
  std::unique_ptr<TreeNode> left;
  std::unique_ptr<TreeNode> right;
  int data;

  // rendering 
  sf::Sprite sprite;
  sf::VertexArray leftLine;
  sf::VertexArray rightLine;
  sf::Vector2f size;

  sf::Font font;
  sf::Text dataText; // TODO: this could be a template?
};
#pragma once
#include "SFML/Graphics.hpp"
#include "Control.h"
#include "common.h"
#include <iostream>

// handles the node object and drawing
// Tree node (int data, sf::Vector2f position)
class TreeNode : public sf::Drawable, public sf::Transformable {
  friend class BST;
private:
  // node data
  // fuck inheritance that shit sucks
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

  // determine node placement when rendering
  enum class D {
    LEFT,
    ROOT,
    RIGHT
  };

public:
  TreeNode(const int val, const sf::Vector2f pos);
  TreeNode& operator=(const int val) {
    std::cout << "OVERLOADED =" << std::endl;
    data = val;
    dataText.setString(std::to_string(val));
    return *this;
  }

private:
  sf::Vector2f calcCircleEdge(const float angle);
  void updateLine(TreeNode* prev, D direction);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
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
  std::unique_ptr<TreeNode> left;
  std::unique_ptr<TreeNode> right;

  // rendering 
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
  sf::Sprite sprite;
  int data;

public:
  TreeNode(const int val, const sf::Vector2f pos);
  TreeNode& operator=(const int val) {
    std::cout << "OVERLOADED =" << std::endl;
    data = val;
    dataText.setString(std::to_string(val));
    return *this;
  }

private:
  void move(const sf::Vector2f pos);
  sf::Vector2f calcCircleEdge(const float angle);
  void updateLine(TreeNode* prev);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
#pragma once
#include "BST.h"

BST::BST(sf::RenderWindow& win) :
  window(win) {
  try {
    if (!GLOBAL::TREETEXTURE.loadFromFile("./Textures/treenodewhite.png"))
      throw std::runtime_error("tree node texture error");
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void BST::insert(std::unique_ptr<TreeNode>& node, const int data, const sf::Vector2f pos, D direction) {
  if (!node) {
    sf::Vector2f newPos;
    if (direction == D::ROOT)
      newPos = pos;
    else if (direction == D::LEFT)
      newPos = sf::Vector2f(pos.x - 100, pos.y + 100);
    else if (direction == D::RIGHT)
      newPos = sf::Vector2f(pos.x + 100, pos.y + 100);
    else
      throw("tree node insert error");
    node = std::make_unique<TreeNode>(data, newPos);
  }
  else {
    data < node->data
      ? insert(node->left, data, node->sprite.getPosition(), D::LEFT)
      : insert(node->right, data, node->sprite.getPosition(), D::RIGHT);
  }

  return;
}

void BST::display() {
  if (!root)
    return;

  preorderTraversal(root.get(), [](TreeNode* node) {
    std::cout << node->data << std::endl;
  });
}

void BST::search(TreeNode* node, const int data) {
  if (!node)
    return;

  if (node->data == data) {
    node->sprite.setColor(sf::Color::Green);
  }
  else {
    data < node->data
      ? search(node->left.get(), data)
      : search(node->right.get(), data);
  }
  return;
}

void BST::clear(std::unique_ptr<TreeNode>& node) {
  if (!node)
    return;

  clear(node->left);
  clear(node->right);
  node.reset();
}

void BST::draw() {
  if (!root)
    return;

  // dfs or bfs
  preorderTraversal(root.get(), [&](TreeNode* node) {
    window.draw(*node);
  });
}

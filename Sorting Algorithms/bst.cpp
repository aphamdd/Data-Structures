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

void BST::insert(std::unique_ptr<TreeNode>& node, const int data, const sf::Vector2f pos, signed int x) {
  if (!node) {
    sf::Vector2f newPos(pos);
    if (x == -1)
      newPos = sf::Vector2f(pos.x - 100, pos.y + 100);
    else if (x == 1)
      newPos = sf::Vector2f(pos.x + 100, pos.y + 100);
    node = std::make_unique<TreeNode>(data, newPos);
  }
  else {
    data < node->data
      ? insert(node->left, data, node->sprite.getPosition(), -1)
      : insert(node->right, data, node->sprite.getPosition(), 1);
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

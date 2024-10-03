#pragma once
#include "BST.h"

BST::BST(sf::RenderWindow& win, sf::Text& text) :
  window(win),
  treeText(text) {
  try {
    if (!treeTexture.loadFromFile("./Textures/treenodewhite.png"))
      throw std::runtime_error("tree node texture error");
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void BST::add() {
  if (!root) {
    sf::Vector2f pos(100.f, 100.f);
    root = std::make_unique<TreeNode>(pos, treeText, treeTexture);
  }
}

void BST::draw() const {
  if (!root)
    return;

  TreeNode* current = root.get();

  // dfs or bfs
  while (current) {
    window.draw(*current); // invokes overridden drawptr function in LLNode
    current = current->left.get();
  }
  return;
}
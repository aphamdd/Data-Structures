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
    sf::Vector2f pos(400.f, 100.f);
    sf::Vector2f posleft(300.f, 250.f);
    sf::Vector2f posright(500.f, 250.f);
    root = std::make_unique<TreeNode>(pos, treeText, treeTexture);
    root->left = std::make_unique<TreeNode>(posleft, treeText, treeTexture);
    root->right = std::make_unique<TreeNode>(posright, treeText, treeTexture);
  }
}

void BST::draw() {
  if (!root)
    return;

  // dfs or bfs
  preorder([&](TreeNode* node) {
    window.draw(*node);
  });
}

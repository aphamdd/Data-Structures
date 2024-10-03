#pragma once
#include "TreeNode.h"

class BST {
public:
  BST(sf::RenderWindow& win, sf::Text& text);

  void add();
  void remove();

  // recursive dfs|bfs
  void rDFS();
  void rBFS();

  // iterative dfs|bfs
  void iDFS();
  void iBFS();

  void IOS(); // in-order successor handling

  // tree operations
  void findLeaf();
  void findChild();
  void findParent();
  void treeHeight();
  void treeSize();
  void findNode();

  void draw() const;

private:
  std::unique_ptr<TreeNode> root = nullptr;

  sf::RenderWindow& window;
  sf::Text& treeText;
  sf::Texture treeTexture;
};
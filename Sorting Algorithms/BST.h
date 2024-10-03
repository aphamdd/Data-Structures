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

  // dfs, tail recursion (do stuff, then recurse)
  // public API to call template preorder function
  template <typename Func>
  void preorder(Func f) {
    preorderTraversal(root.get(), f);
  }

  void inorder(); // dfs, head recursion (recursion first)
  void postorder(); // bfs

  void IOS(); // in-order successor handling

  // store data in their data-structure to represent current step
  void stackFrame();
  void queueFrame();

  // tree operations
  void findLeaf();
  void findChild();
  void findParent();
  void treeHeight();
  void treeSize();
  void findNode();

  void draw();

private:
  template<typename Func>
  void preorderTraversal(TreeNode* node, Func fn) {
    if (!node)
      return;
    fn(node);
    preorderTraversal(node->left.get(), fn);
    preorderTraversal(node->right.get(), fn);
  }

private:
  std::unique_ptr<TreeNode> root = nullptr;

  sf::RenderWindow& window;
  sf::Text& treeText;
  sf::Texture treeTexture;
};
#pragma once
#include "TreeNode.h"


class BST {
private:
  std::unique_ptr<TreeNode> root = nullptr;
  sf::RenderWindow& window;
  // determine node placement when rendering
  enum class D {
    LEFT,
    ROOT,
    RIGHT
  };

public:
  BST(sf::RenderWindow& win);
  void clear() { clear(root); }

  // wrapper functions for basic tree operations
  void insert(const int data, const sf::Vector2f pos) { insert(root, data, pos, D::ROOT); }
  void search(const int data) { search(root.get(), data); }
  void display();

  // 3 Cases: Leaf, one child, two children
  void remove(const int data) { remove(root, data); }

  void BFSTraversal();



  // BIG BOI FEATURES
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
  bool isBalanced();
  bool isComplete();
  bool isFull() const;
  bool isPerfect();
  void findios();

  void draw();

private:
  void insert(std::unique_ptr<TreeNode>& node, const int data, const sf::Vector2f pos, D direction);
  void search(TreeNode* node, const int data);
  void remove(std::unique_ptr<TreeNode>& node, const int data);
  void clear(std::unique_ptr<TreeNode>& node);

  void propogatePos(TreeNode* node, const sf::Vector2f shift);

  // dfs, tail recursion
  template<typename Func>
  void preorderTraversal(TreeNode* node, Func fn) const {
    if (!node)
      return;
    fn(node);
    preorderTraversal(node->left.get(), fn);
    preorderTraversal(node->right.get(), fn);
  }

  // dfs, head recursion
  template<typename Func>
  void inorderTraversal(TreeNode* node, Func fn) const {
    if (!node)
      return;
    inorderTraversal(node->left.get(), fn);
    fn(node);
    inorderTraversal(node->right.get(), fn);
  }

  // dfs, typically for deleting trees or postfix notation of expressions
  template<typename Func>
  void postorderTraversal(TreeNode* node, Func fn) const {
    if (!node)
      return;
    postorderTraversal(node->left.get(), fn);
    postorderTraversal(node->right.get(), fn);
    fn(node);
  }
};
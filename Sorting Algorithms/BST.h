#pragma once
#include "TreeNode.h"

class BST {
private:
  std::unique_ptr<TreeNode> root = nullptr;
  sf::RenderWindow& window;

  struct Traversal {
    friend class BST;
  public:
    TreeNode* active = nullptr;
  private:
    TreeNode* prev = nullptr;
  };

public:
  Traversal raw;

public:
  BST(sf::RenderWindow& win);
  void clear() { clear(root); }

  // wrapper functions for basic tree operations
  void insert(const int data, const sf::Vector2f pos) { insert(root, data, pos, TreeNode::D::ROOT); }
  void findValue(const int data) { findValue(root.get(), data); }
  void remove(const int data) { remove(root, data); }
  void display();

  void BFSTraversal();

  // interactive node methods
  bool search(const sf::Vector2i pos);
  bool move(const sf::Vector2i pos);

  // BIG BOI FEATURES
  // store data in their data-structure to represent current step
  void stackFrame();
  void queueFrame();
  void fitView();

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
  void insert(std::unique_ptr<TreeNode>& node, const int data, const sf::Vector2f pos, TreeNode::D direction);
  void findValue(TreeNode* node, const int data);
  void remove(std::unique_ptr<TreeNode>& node, const int data);
  void clear(std::unique_ptr<TreeNode>& node);

  void propogatePos(TreeNode* node, const sf::Vector2f shift);

  bool search(TreeNode* node, const sf::Vector2f pos);

  // Template experiment
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
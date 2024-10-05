#pragma once
#include "TreeNode.h"

class BST {
public:
  BST(sf::RenderWindow& win);

  // wrapper insert function
  void insert(const int data, const sf::Vector2f pos) { insert(root, data, pos, 0); }
  void remove();
  void search();
  void clear() { clear(root); }
  void display();

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
  bool isBalanced();
  bool isComplete();
  bool isFull() const;
  bool isPerfect();

  void draw();

private:
  void insert(std::unique_ptr<TreeNode>& node, const int data, const sf::Vector2f pos, signed int x);
  void clear(std::unique_ptr<TreeNode>& node);

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

private:
  std::unique_ptr<TreeNode> root = nullptr;

  sf::RenderWindow& window;
};
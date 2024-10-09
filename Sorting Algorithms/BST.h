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
  void clear() { clear(root); raw.prev = nullptr; raw.active = nullptr; }

  // wrapper functions for basic tree operations
  void insert(const int data, const sf::Vector2f pos) { insert(root, data, pos, TreeNode::D::ROOT); }
  void findValue(const int data) { findValue(root.get(), data); }
  void remove() { if (raw.active) remove(root, raw.active->data); }
  void display();

  // interactive node methods
  bool search(const sf::Vector2i pos);
  bool move(const sf::Vector2i pos);

  // tree operations
  // TODO: highlight timer? highlight for a couple seconds and then revert
  void findLeaves() { findLeaves(root.get()); } // highlight all leaves
  void findChildren() { findChildren(raw.active); } // highlight children of selected node
  void findParent() { findParent(raw.active); } // highlight parent of selected node
  void findIOS(); // highlight the IOS of selected node
  int treeHeight() { if (!raw.active) return treeHeight(root.get()-1); return treeHeight(raw.active)-1;  } // return height of tree
  int treeSize() { return treeSize(root.get()); } // return number of nodes in tree
  bool isBalanced(); // check if tree is balanced
  bool isComplete(); // check if tree is complete
  bool isFull() const; // check if tree is full
  bool isPerfect(); // check if all conditions are met

  void bfs();
  // tree traversal animations
  // active cursor
  void BFSTraversal(); // animate bfs traversal of tree
  // animate preorder traversal of tree
  // animate inorder traversal of tree
  // animate postorder traversal of tree

  // BIG BOI FEATURE
  // store data in their data-structure to represent current step
  void stackFrame();
  void queueFrame();
  void fitView();

  void draw();

private:
  void insert(std::unique_ptr<TreeNode>& node, const int data, const sf::Vector2f pos, TreeNode::D direction);
  void findValue(TreeNode* node, const int data);
  void remove(std::unique_ptr<TreeNode>& node, const int data);
  void clear(std::unique_ptr<TreeNode>& node);
  void propogatePos(TreeNode* node, const sf::Vector2f shift);
  bool search(TreeNode* node, const sf::Vector2f pos);
  void findLeaves(TreeNode* node);
  void findChildren(TreeNode* node);
  void findParent(TreeNode* node);
  int treeHeight(TreeNode* node); 
  int treeSize(TreeNode* node); 

  
  


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
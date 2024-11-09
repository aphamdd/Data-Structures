#pragma once
#include "TreeNode.h"

class BST {
private:
  std::unique_ptr<TreeNode> root = nullptr;
  sf::RenderWindow& window;
  sf::CircleShape cursor; 

  std::vector<sf::FloatRect> nBounds; // vector of all node bounds

  struct Traversal {
    friend class BST;
  public:
    TreeNode* active = nullptr;
  private:
    TreeNode* prev = nullptr;
  };

  // This is all really confusing, refactor this into better structures and names
  enum class TreeState {
    ENTRY,
    HIGHLIGHT,
    WAIT,
    COMPARE,
    INSERT,
    SEARCH
  };

  // only for dfs stack
  struct NodeState {
    bool l = false;
    bool r = false;
    TreeNode* statePtr = nullptr;
  } visit;

  struct AnimationState {
    friend class BST;
  public:
    bool inUse = false;
    // dfs animation
    std::vector<NodeState> treeStack; // stack of node visited states

  private:
    // blinking nodes
    int blinks = 0;
    std::vector<TreeNode*> pointers = {}; // list of nodes to be blinking

    // bfs animation
    std::vector<TreeNode*> queue;
    std::vector<TreeNode*> levelQueue;
    int i = 0;
    int levelWidth = 0;

    // insertion animation
    sf::Vector2f goal;
    std::unique_ptr<TreeNode> newNode = nullptr;

    // general
    TreeState state = TreeState::ENTRY;
    TreeState prevState = TreeState::ENTRY;
    TreeNode* temp = nullptr;
    TreeNode* animatePtr = nullptr;
  };
  /// ///////////////////////////////////////////////////////////

public:
  Traversal raw;
  AnimationState animate;
  sf::Clock delayClock;

  std::string currStep = "None";

public:
  BST(sf::RenderWindow& win);
  void clear() { clear(root); raw.prev = nullptr; raw.active = nullptr; }

  // getter
  int getActiveData() { if (raw.active) return raw.active->data; else return 0; }
  int getPrevData() { if (raw.prev) return raw.prev->data; else return 0; }

  // wrapper functions for basic tree operations
  void insert(const int data, const sf::Vector2f pos) { insert(root, data, pos, TreeNode::D::ROOT); }
  void findValue(const int data) { findValue(root.get(), data); }
  void remove() { if (raw.active) remove(root, raw.active->data); updatePrevPtr(raw.active); }
  void display();

  // tree operations
  // TODO: highlight timer? highlight for a couple seconds and then revert
  void findLeaves() { findLeaves(root.get()); } // highlight all leaves
  void findChildren() { findChildren(raw.active); } // highlight children of selected node
  bool findParent() { return findParent(raw.active); } // highlight parent of selected node
  void findIOS(); // highlight the IOS of selected node
  int treeHeight() { return treeHeight(root.get()); } // return height of tree
  int treeSize() { return treeSize(root.get()); } // return number of nodes in tree
  bool isBalanced() const { int height = 0; return isBalanced(root.get(), &height); }
  bool isComplete(const int size) const { return isComplete(root.get(), 0, size); }
  bool isFull() const { return isFull(root.get()); }
  bool isPerfect(const int height) const { return isPerfect(root.get(), height, 0); }

  // interactive node methods
  bool search(const sf::Vector2i pos);
  bool move(const sf::Vector2i pos);

  // tree traversal animations
  void updateCursor(TreeNode* target); // cursor follows ptr
  void nodeBlink();
  bool dfsAnimate(); // animate dfs traversal of tree
  bool bfsAnimate(); // animate dfs traversal of tree
  bool traverseToAnimate(const int value, const int flag); // same as search animate but better
  void bfs();
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
  bool findParent(TreeNode* node);
  int treeHeight(TreeNode* node); 
  int treeSize(TreeNode* node); 

  bool updatePrevPtr(TreeNode* node);

  bool isBalanced(TreeNode* node, int* height) const; // check if tree is balanced
  bool isComplete(TreeNode* node, const int index, const int numNodes) const; // check if tree is complete
  bool isFull(TreeNode* node) const; // check if tree is full
  bool isPerfect(TreeNode* node, const int depth, int level) const; 

  bool findAllNodeBounds(TreeNode* ptr); // excludes passed node bound
  void shiftSubtrees(TreeNode* node, const sf::Vector2f shift);
  TreeNode* findOverlapNode(TreeNode* node, const TreeNode* ogNode, const sf::FloatRect overlap);
  TreeNode* lca(TreeNode* node, const TreeNode* const a, const TreeNode* const b); // lowest common ancestor

  bool insertNodeAnimation(TreeNode* node, const sf::Vector2f pos);
  void resetAnimateState() { animate = {}; visit = {}; currStep = "None"; }


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
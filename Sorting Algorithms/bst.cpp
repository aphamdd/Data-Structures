#pragma once
#include "BST.h"

BST::BST(sf::RenderWindow& win) :
  window(win),
  cursor(0.f, 3) {
  cursor.setFillColor(sf::Color::White);
  cursor.setOutlineThickness(1.5f);
  cursor.setOutlineColor(sf::Color::Black);
  cursor.setRotation(180.f);

  try {
    if (!GLOBAL::TREETEXTURE.loadFromFile("./Textures/treenodewhitetf.png"))
      throw std::runtime_error("tree node texture error");
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void BST::insert(std::unique_ptr<TreeNode>& node, const int data, const sf::Vector2f pos, TreeNode::D direction) {
  if (!node) {
    // TODO: theres definitely a better way to do this but not a priority
    sf::Vector2f newPos;
    if (direction == TreeNode::D::ROOT)
      newPos = pos;
    else if (direction == TreeNode::D::LEFT)
      newPos = sf::Vector2f(pos.x - 100, pos.y + 100);
    else if (direction == TreeNode::D::RIGHT)
      newPos = sf::Vector2f(pos.x + 100, pos.y + 100);
    else
      throw("tree node insert error");
    node = std::make_unique<TreeNode>(data, newPos);
    if (raw.prev) {
      // a way to save on memory is to just store node bounds on the same level
      // as the node being inserted. assumes nodes are in their original positions
      // shifts left and right subtree if nodes collide on insertion
      if (findAllNodeBounds(node.get())) {
        for (const auto& anysprite : nBounds) {
          if (node->sprite.getGlobalBounds().intersects(anysprite)) {
            if (root) {
              // TODO: i have to pass in the node that's the parent of the nodes
              // colliding. passing root means it could be the ancestor of the collided
              // nodes. shifting the entire tree instead of the affected subtrees
              // but this gets recursively annoying. if the subtree's subtree gets shifted
              // it could collide with the subtree on the other side again, needing another
              // shift once more.

              // TreeNode* ptr = findCommonParent();
              raw.prev = root.get();
              shiftSubtrees(root->left.get(), sf::Vector2f(-100, 0));
              raw.prev = root.get();
              shiftSubtrees(root->right.get(), sf::Vector2f(100, 0));
            }
          }
        }
      }

      node->updateLine(raw.prev);
    }
    raw.prev = nullptr;
  }
  else {
    raw.prev = node.get();
    data < node->data
      ? insert(node->left, data, node->sprite.getPosition(), TreeNode::D::LEFT)
      : insert(node->right, data, node->sprite.getPosition(), TreeNode::D::RIGHT);
  }

  return;
}

void BST::shiftSubtrees(TreeNode* node, const sf::Vector2f shift) {
  if (!node)
    return;

  node->sprite.move(shift);
  node->dataText.move(shift);
  node->updateLine(raw.prev);

  raw.prev = node;
  shiftSubtrees(node->left.get(), shift);
  raw.prev = node;
  shiftSubtrees(node->right.get(), shift);
}

void BST::remove(std::unique_ptr<TreeNode>& node, const int data) {
  if (!node)
    return;

  if (node->data == data && node.get() == raw.active) {
    raw.active->sprite.setColor(sf::Color::White);
    if (raw.active == node.get()) {
      raw.prev = nullptr;
      raw.active = nullptr;
    }
    // leaf
    if (!node->left && !node->right) {
      updatePrevPtr(node.get()); // have prev ptr ready
      node.reset();
      if (raw.prev)
        raw.prev->updateLine(nullptr);
    }
    // 1 child
    else if (!node->left || !node->right) {
      sf::Vector2f shift(100, -100);
      if (node->left) {
        node = std::move(node->left);
        propogatePos(node.get(), shift);
      }
      else if (node->right) {
        node = std::move(node->right);
        shift.x *= -1;
        propogatePos(node.get(), shift);
      }
      updatePrevPtr(node.get());
      node->updateLine(raw.prev);
    }
    // 2 children
    else {
      // if there's a left subtree, remove farthest left, keep its right subtree 
      TreeNode* current = node->right.get();
      if (current->left.get()) {
        while (current->left.get()) {
          raw.prev = current;
          current = current->left.get();
        }
        // TODO: operator overloading = for data and dataText
        node->data = current->data;
        node->dataText.setString(std::to_string(current->data));
        raw.prev->left = std::move(current->right);
        if (raw.prev->left)
          propogatePos(raw.prev->left.get(), sf::Vector2f(-100, -100));
        raw.prev->updateLine(nullptr);
      }
      // if there's no left subtree
      else {
        // TODO: operator overloading = for data and dataText
        node->data = node->right->data;
        node->dataText.setString(std::to_string(node->right->data));
        updatePrevPtr(node.get()); // have prev ptr ready
        node->right = std::move(node->right->right);
        propogatePos(node->right.get(), sf::Vector2f(-100, -100));
        node->updateLine(raw.prev);
      }
    }
  }
  else {
    // preorder traversal
    data < node->data
      ? remove(node->left, data)
      : remove(node->right, data);
  }

  return;
}

void BST::propogatePos(TreeNode* node, const sf::Vector2f shift) {
  if (!node)
    return;
  node->sprite.move(shift);
  node->dataText.move(shift);
  node->updateLine(raw.prev);

  raw.prev = node;
  propogatePos(node->left.get(), shift);
  raw.prev = node;
  propogatePos(node->right.get(), shift);
}

/*
// sorta works? doesn't work for sibling nodes
// another method is to just shift all the nodes by a fixed amount
void BST::propogatePos(TreeNode* node, const sf::Vector2f prevNode, const sf::Vector2f prevText) {
  if (!node)
    return;
  sf::Vector2f nodePos = node->sprite.getPosition();
  sf::Vector2f textPos = node->dataText.getPosition();
  node->sprite.setPosition(prevNode);
  node->dataText.setPosition(prevText);
  propogatePos(node->left.get(), nodePos, textPos);
  propogatePos(node->right.get(), nodePos, textPos);
}
*/

void BST::display() {
  if (!root)
    return;

  preorderTraversal(root.get(), [](TreeNode* node) {
    std::cout << node->data << std::endl;
  });
}

void BST::findValue(TreeNode* node, const int data) {
  if (!node)
    return;

  if (node->data == data) {
    node->sprite.setColor(sf::Color::Green);
  }
  else {
    data < node->data
      ? findValue(node->left.get(), data)
      : findValue(node->right.get(), data);
  }
  return;
}

bool BST::search(const sf::Vector2i mpos) {
  // TODO: more efficient is to just store node positions in a vector 
  // and search through that list. update the vector upon changes
  if (!root)
    return false;

  sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
  // check if we clicked on the same active node
  if (raw.active) {
    sf::FloatRect activeBounds = raw.active->sprite.getGlobalBounds();
    if (activeBounds.contains(convertMPos.x, convertMPos.y)) {
      raw.active->sprite.setColor(sf::Color::Green);
      updatePrevPtr(raw.active);
      raw.active->updateLine(raw.prev); // update lines to follow sprite
      return true;
    }
  }
  // Decision: is it better to do DFS (stack) or BFS (heap) searches for this?
  // Problem is I'll be calling this function CONSTANTLY which could cause
  // a stack overflow. However at the same time, the stack should only get
  // filled up by n amount of nodes.
  return search(root.get(), convertMPos);
}

bool BST::search(TreeNode* node, sf::Vector2f pos) {
  if (!node)
    return false;

  sf::FloatRect currBounds = node->sprite.getGlobalBounds();
  if (currBounds.contains(pos.x, pos.y)) {
    if (raw.active)
      raw.active->sprite.setColor(sf::Color::White);

    raw.active = node;
    updatePrevPtr(raw.active);
    raw.active->sprite.setColor(sf::Color::Green);
    raw.active->updateLine(raw.prev); // update lines to follow sprite
    return true;
  }
  if (search(node->left.get(), pos))
    return true;
  return search(node->right.get(), pos);
}

bool BST::move(const sf::Vector2i mpos) {
  if (raw.active) {
    sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
    sf::Vector2f lastPos = raw.active->sprite.getPosition();

    raw.active->move(convertMPos); // moves sprite
    raw.active->updateLine(raw.prev); // update lines to follow sprite
    return true;
  }
  return false;
}

void BST::findLeaves(TreeNode* node) {
  if (!node)
    return;

  if (!node->left && !node->right)
    node->sprite.setColor(sf::Color::Magenta);
  findLeaves(node->left.get());
  findLeaves(node->right.get());
}

void BST::findChildren(TreeNode* node) {
  if (!node)
    return;
  if (node->left)
    node->left->sprite.setColor(sf::Color::Blue);
  if (node->right)
    node->right->sprite.setColor(sf::Color::Blue);
}

bool BST::findParent(TreeNode* node) {
  if (!node)
    return false;

  if (!updatePrevPtr(node))
    return false;

  raw.prev->sprite.setColor(sf::Color::Red);
  return true;
}

void BST::findIOS() {
  return;
}

int BST::treeHeight(TreeNode* node) {
  if (!node)
    return 0;
  return std::max(treeHeight(node->left.get()), treeHeight(node->right.get())) + 1;
}

int BST::treeSize(TreeNode* node) {
  if (!node)
    return 0;
  return treeSize(node->left.get()) + treeSize(node->right.get()) + 1;
}

bool BST::isBalanced(TreeNode* node, int* height) const {
  if (!node) {
    *height = 0;
    return 1;
  }
  // current node left and right subtree height
  int left = 0, right = 0; 

  // l and r tells us if the left and right subtrees are balanced
  int l = isBalanced(node->left.get(), &left);
  int r = isBalanced(node->right.get(), &right);

  // height propogates left or right subtree height up the tree
  *height = (left > right ? left : right) + 1;

  // if the difference of left and right >= 2, then we're not balanced.
  if (std::abs(left - right) >= 2)
    return 0;

  return l && r; // true if left and right subtrees are balanced
}

bool BST::isComplete(TreeNode* node, const int index, const int numNodes) const {
  if (!node)
    return true;

  if (index >= numNodes)
    return false;

  // If the index of any element in the array is i, the element in the 
  // index 2i + 1 will become the left child and element in 2i + 2 index 
  // will become the right child. Also, the parent of any element at 
  // index i is given by the lower bound of(i - 1) / 2.
  return isComplete(node->left.get(), 2 * index + 1, numNodes) && 
         isComplete(node->right.get(), 2 * index + 2, numNodes);
}

bool BST::isFull(TreeNode* node) const {
  if (!node)
    return true;
  if (!node->left && !node->right)
    return true;
  if (node->left && node->right)
    return isFull(node->left.get()) && isFull(node->right.get());

  return false;
}

bool BST::isPerfect(TreeNode* node, const int depth, int level) const {
  // if tree is empty
  if (!node)
    return true;
  // check leaves
  if (!node->left && !node->right)
    return (depth == level + 1);
  // if there's only 1 child
  if (!node->left || !node->right)
    return false;

  return isPerfect(node->left.get(), depth, level + 1) && 
         isPerfect(node->right.get(), depth, level + 1);
}

void BST::bfs() {
  if (!root)
    return;

  // Notes
  // push_back: when you want to move an existing object into a vector
  // emplace_back: when you want to create a new object into a vector, using the objects constructor arguments

  std::vector<TreeNode*> queue;
  queue.emplace_back(root.get());
  while (!queue.empty()) {
    int levelWidth = queue.size();
    std::vector<TreeNode*> levelQueue;
    for (int i = 0; i < levelWidth; ++i) {
      TreeNode* node = queue.at(0);
      std::cout << node->data << " ";
      if (node->left)
        queue.emplace_back(node->left.get());
      if (node->right)
        queue.emplace_back(node->right.get());
      queue.erase(queue.begin());
    }
  }
  return;
}

bool BST::updatePrevPtr(TreeNode* node) {
  if (!node) {
    raw.prev = nullptr;
    return false;
  }

  raw.prev = root.get();
  while (raw.prev) {
    if ((raw.prev->left.get() == node) || (raw.prev->right.get() == node))
      return true;

    if (raw.prev->data <= node->data)
      raw.prev = raw.prev->right.get();
    else
      raw.prev = raw.prev->left.get();
  }
  raw.prev = nullptr;
  return false;
}

void BST::clear(std::unique_ptr<TreeNode>& node) {
  if (!node)
    return;

  clear(node->left);
  clear(node->right);
  node.reset();
}

void BST::updateCursor(TreeNode* target) {
  sf::Clock clock;
  if (!target) {
    cursor.setRadius(0.f);
    return;
  }
  
  cursor.setRadius(10.f);
  cursor.setFillColor(target->sprite.getColor());

  sf::Vector2f currPos = cursor.getPosition();
  sf::Vector2f goal = target->sprite.getPosition();
  goal.y -= (target->size.y * 0.5) + 20;
  float dx = goal.x - currPos.x;
  float dy = goal.y - currPos.y;
  float distance = float(sqrt(pow(dx, 2) + pow(dy, 2))); // euclidean distance formula
  if (distance >= 3) { 
    // calculate whats necessary
    float deltaTime = clock.restart().asSeconds();
    float k = 2000 * deltaTime; // dampening constant
    sf::Vector2f direction(dx / distance, dy / distance); // normalize dx,dy direction
    float velx = (k * distance * direction.x);
    float vely = (k * distance * direction.y);
    sf::Vector2f offset(velx, vely);
    cursor.move(offset);
  } 
  else {
    // TODO: idle animation
    cursor.setPosition(goal);
  }
}

bool BST::findAllNodeBounds(TreeNode* ptr) {
  nBounds.clear();
  if (!root)
    return false;

  std::vector<TreeNode*> queue;
  queue.emplace_back(root.get());
  while (!queue.empty()) {
    int levelWidth = queue.size();
    std::vector<TreeNode*> levelQueue;
    for (int i = 0; i < levelWidth; ++i) {
      TreeNode* node = queue.at(0);
      if (node != ptr)
        nBounds.emplace_back(node->sprite.getGlobalBounds());
      if (node->left)
        queue.emplace_back(node->left.get());
      if (node->right)
        queue.emplace_back(node->right.get());
      queue.erase(queue.begin());
    }
  }
  return true;
}

void BST::draw() {
  if (!root)
    return;

  // dfs or bfs
  preorderTraversal(root.get(), [&](TreeNode* node) {
    window.draw(*node);
  });
  window.draw(cursor);
}

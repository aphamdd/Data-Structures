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
    if (!GLOBAL::TREETEXTURE.loadFromFile("./Textures/shield_4.png"))
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
    updatePrevPtr(node.get());
    if (raw.prev) {
      // a way to save on memory is to just store node bounds on the same level
      // as the node being inserted. assumes nodes are in their original positions
      // shifts left and right subtree if nodes collide on insertion
      if (findAllNodeBounds(node.get())) {
        for (const auto& anysprite : nBounds) {
          if (node->sprite.getGlobalBounds().intersects(anysprite)) {
            TreeNode* parent = lca(root.get(), node.get(), findOverlapNode(root.get(), node.get(), anysprite));
            raw.prev = parent;
            shiftSubtrees(parent->left.get(), sf::Vector2f(-100, 0));
            raw.prev = parent;
            shiftSubtrees(parent->right.get(), sf::Vector2f(100, 0));
          }
        }
      }

      node->updateLine(raw.prev);
    }
    raw.prev = nullptr;
  }
  else {
    //raw.prev = node.get(); // while this works, it doesn't guarantee 
    data < node->data
      ? insert(node->left, data, node->sprite.getPosition(), TreeNode::D::LEFT)
      : insert(node->right, data, node->sprite.getPosition(), TreeNode::D::RIGHT);
  }

  return;
}

TreeNode* BST::findOverlapNode(TreeNode* node, const TreeNode* ogNode, const sf::FloatRect overlap) {
  if (!node)
    return nullptr;

  if (node != ogNode && node->sprite.getGlobalBounds().intersects(overlap))
    return node;

  TreeNode* left = findOverlapNode(node->left.get(), ogNode, overlap);
  TreeNode* right = findOverlapNode(node->right.get(), ogNode, overlap);

  return left ? left : right;
}

TreeNode* BST::lca(TreeNode* node, const TreeNode* const a, const TreeNode* const b) {
  if (!node)
    return nullptr;
  if (node == a || node == b)
    return node;

  TreeNode* left = lca(node->left.get(), a, b);
  TreeNode* right = lca(node->right.get(), a, b);

  if (left && right)
    return node;
  else if (!left && !right)
    return nullptr;

  return left ? left : right;
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

void BST::display() {
  if (!root)
    return;

  preorderTraversal(root.get(), [](TreeNode* node) {
    std::cout << node->data << std::endl;
  });
}

void BST::nodeBlink() {
  if (animate.blinks < 6 && animate.inUse && !animate.pointers.empty()) {
    if (delayClock.getElapsedTime().asSeconds() >= 0.5) {
      if (animate.blinks % 2 == 0) {
        for (TreeNode* const node : animate.pointers) {
          node->sprite.setColor(sf::Color::Yellow);
        }
      }
      else {
        for (TreeNode* const node : animate.pointers) {
          node->sprite.setColor(sf::Color::White);
        }
      }
      ++animate.blinks;
      delayClock.restart();
    }
  }
  else
    resetAnimateState();

  return;
}

bool BST::findValue(TreeNode* node, const int data) {
  if (!node)
    return false;

  if (node->data == data) {
    animate.pointers.emplace_back(node);
    return true;
  }
  if (data < node->data)
    return findValue(node->left.get(), data);
  else
    return findValue(node->right.get(), data);
  /*
  else {
    data < node->data
      ? return findValue(node->left.get(), data)
      : return findValue(node->right.get(), data);
  }
  */
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
    animate.pointers.emplace_back(node);
  findLeaves(node->left.get());
  findLeaves(node->right.get());
}

void BST::findChildren(TreeNode* node) {
  if (!node)
    return;
  if (node->left)
    animate.pointers.emplace_back(node->left.get());
  if (node->right)
    animate.pointers.emplace_back(node->right.get());
}

bool BST::findParent(TreeNode* node) {
  if (!node)
    return false;

  if (!updatePrevPtr(node))
    return false;

  animate.pointers.emplace_back(raw.prev);
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

  sf::Vector2f goal = target->sprite.getPosition();
  goal.x += 10;
  goal.y -= (target->size.y * 0.5) + 20;

  // allows cursor to point at a fake node
  if (!visit.statePtr) {
    if (currStep == "Visit Left") {
      goal.x -= 100;
      goal.y += 100;
    }
    else if (currStep == "Visit Right") {
      goal.x += 100;
      goal.y += 100;
    }
  }
  
  cursor.setFillColor(target->sprite.getColor());
  cursor.setRadius(10.f);
  sf::Vector2f currPos = cursor.getPosition();
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

// did i just write an iterative dfs
bool BST::dfsAnimate() {
  if (!root) {
    resetAnimateState();
    return true;
  }

  if (visit.statePtr)
    updateCursor(visit.statePtr);
  else {
    if (!animate.treeStack.empty()) {
      TreeNode* temp = animate.treeStack.back().statePtr;
      updateCursor(temp);
    }
  }
  switch (animate.state) {
    case TreeState::ENTRY: {
      visit.statePtr = root.get();

      animate.prevState = animate.state;
      animate.state = TreeState::HIGHLIGHT;
      delayClock.restart();
      currStep = "DFS Traversal";
    } break;
    case TreeState::HIGHLIGHT: {
      // base case: if null or visited left & right
      if (!visit.statePtr || (visit.l && visit.r)) {
        if (!visit.statePtr)
          currStep = "Empty: Pop Back";
        if (!animate.treeStack.empty()) {
          visit = animate.treeStack.back();
          animate.treeStack.pop_back();
        }
        else { // if stack is empty, we've searched the entire tree
          resetAnimateState();
          return true;
        }
      }
      else {
        if (!visit.l)
          currStep = "Left Unvisited";
      }


      visit.statePtr->sprite.setColor(sf::Color::Red);
      animate.prevState = animate.state;
      animate.state = TreeState::WAIT;
      delayClock.restart();
    } break;
    case TreeState::WAIT: {
      if (delayClock.getElapsedTime().asSeconds() >= GLOBAL::DELAY) {
        if (visit.l && !visit.r && currStep != "Right Unvisited") {
          currStep = "Right Unvisited";
        }
        else if (animate.prevState == TreeState::HIGHLIGHT) {
          visit.statePtr->sprite.setColor(sf::Color::White);
          animate.prevState = animate.state;
          animate.state = TreeState::COMPARE;
        }
        else if (animate.prevState == TreeState::COMPARE) {
          animate.prevState = animate.state;
          animate.state = TreeState::HIGHLIGHT;
        }
        delayClock.restart();
      }
    } break;
    case TreeState::COMPARE: {
      if (!visit.l) {
        visit.l = true;
        animate.treeStack.push_back(visit);

        visit.l = false;
        visit.r = false;
        visit.statePtr = visit.statePtr->left.get();
        currStep = "Visit Left";
      }
      else if (!visit.r) {
        visit.r = true;
        animate.treeStack.push_back(visit);

        visit.l = false;
        visit.r = false;
        visit.statePtr = visit.statePtr->right.get();
        currStep = "Visit Right";
      }
      if (visit.l && visit.r)
        currStep = "L&R Visited: Pop Back";

      animate.prevState = animate.state;
      animate.state = TreeState::WAIT;
      delayClock.restart();
    } break;
  }
  return false;
}

bool BST::bfsAnimate() {
  if (!root) {
    resetAnimateState();
    return true;
  }

  updateCursor(animate.temp);
  switch (animate.state) {
    case TreeState::ENTRY: {
      animate.queue.emplace_back(root.get());
      animate.prevState = animate.state;
      animate.state = TreeState::HIGHLIGHT;
      delayClock.restart();
      currStep = "Entry";
    } break;
    case TreeState::HIGHLIGHT: {
      if (!animate.queue.empty()) {
        animate.i = 0;
        animate.temp = nullptr;
        animate.levelWidth = animate.queue.size();
        animate.prevState = animate.state;
        animate.state = TreeState::COMPARE;
      }
      else {
        resetAnimateState();
        return true;
      }
      delayClock.restart();
    } break;
    case TreeState::WAIT: {
      if (delayClock.getElapsedTime().asSeconds() >= GLOBAL::DELAY) {
        animate.temp->sprite.setColor(sf::Color::White);
        animate.prevState = animate.state;
        animate.state = TreeState::COMPARE;
        delayClock.restart();
      }
    } break;
    case TreeState::COMPARE: {
      if (animate.i < animate.levelWidth) {
        animate.temp = animate.queue.at(0);
        animate.temp->sprite.setColor(sf::Color::Red);
        if (animate.temp->left) {
          animate.queue.emplace_back(animate.temp->left.get());
        }
        if (animate.temp->right) {
          animate.queue.emplace_back(animate.temp->right.get());
        }
        animate.queue.erase(animate.queue.begin());
        ++animate.i;

        animate.prevState = animate.state;
        animate.state = TreeState::WAIT;
      }
      else {
        animate.prevState = animate.state;
        animate.state = TreeState::HIGHLIGHT;
      }
      delayClock.restart();
    } break;
  }

  return false;
}

// TODO: make this a template so I can use this both for cursor and inserting nodes?
bool BST::insertNodeAnimation(TreeNode* target, const sf::Vector2f goal) {
  if (!target)
    return true;

  sf::Clock clock;
  sf::Vector2f currPos = target->sprite.getPosition();
  float dx = goal.x - currPos.x;
  float dy = goal.y - currPos.y;
  float distance = float(sqrt(pow(dx, 2) + pow(dy, 2))); // euclidean distance formula
  if (distance >= 3) { 
    // calculate whats necessary
    float deltaTime = clock.restart().asSeconds();
    float k = 10000 * deltaTime; // dampening constant
    sf::Vector2f direction(dx / distance, dy / distance); // normalize dx,dy direction
    float velx = (k * distance * direction.x);
    float vely = (k * distance * direction.y);
    sf::Vector2f offset(velx, vely);

    target->sprite.move(offset);
    target->dataText.move(offset);
  } 
  else {
    target->sprite.setPosition(goal);
    target->updateLine(animate.animatePtr); // update lines to follow sprite
    return true;
  }
  return false;
}

// flag 0 : search and highlight
// flag 1 : insert new node
// flag 2 : delete node
bool BST::traverseToAnimate(const int value, const int flag) {
  if (!root) {
    if (flag == 1) {
      insert(root, value, sf::Vector2f(900, 100), TreeNode::D::ROOT);
    }
    resetAnimateState();
    return true;
  }

  updateCursor(animate.animatePtr);
  switch (animate.state) {
    case TreeState::ENTRY: {
      if (flag == 1) {
        animate.newNode = std::make_unique<TreeNode>(value, sf::Vector2f(100, 100));
      }
      animate.animatePtr = root.get();
      animate.state = TreeState::HIGHLIGHT;
      delayClock.restart();
    } break;
    case TreeState::HIGHLIGHT: {
      if (!animate.animatePtr) {
        resetAnimateState();
        return true;
      }

      animate.animatePtr->sprite.setColor(sf::Color::Red);
      animate.state = TreeState::WAIT;
      delayClock.restart();
    } break;
    case TreeState::WAIT: {
      if (delayClock.getElapsedTime().asSeconds() >= GLOBAL::DELAY) {
        // for finding a node and highlighting found node
        if (flag == 0) {
          animate.state = TreeState::SEARCH;
        }
        // inserting a new node
        else if (flag == 1) {
          animate.state = TreeState::COMPARE;
        }
        else {
          std::cout << "there isn't a state for this flag" << std::endl;
          return true;
        }
        delayClock.restart();
      }
    } break;
    case TreeState::SEARCH: {
      if (animate.animatePtr->data == value) {
        animate.animatePtr->sprite.setColor(sf::Color::Cyan);
        resetAnimateState();
        return true;
      }
      animate.animatePtr->sprite.setColor(sf::Color::White);
      value < animate.animatePtr->data ?
        animate.animatePtr = animate.animatePtr->left.get() :
        animate.animatePtr = animate.animatePtr->right.get();

      animate.state = TreeState::HIGHLIGHT;
      delayClock.restart();
    } break;
    case TreeState::COMPARE: {
      animate.animatePtr->sprite.setColor(sf::Color::White);
      if (value < animate.animatePtr->data) {
        // set a goal position for the temp node
        // link up the pointers before moving
        // move the temp node to its position
        if (!animate.animatePtr->left) {
          animate.goal = animate.animatePtr->sprite.getPosition();
          animate.goal = sf::Vector2f(animate.goal.x - 100, animate.goal.y + 100);
          animate.temp = animate.newNode.get();
          animate.animatePtr->left = std::move(animate.newNode);
          animate.state = TreeState::INSERT;
        }
        else {
          animate.state = TreeState::HIGHLIGHT;
          animate.animatePtr = animate.animatePtr->left.get();
        }
      }
      else {
        if (!animate.animatePtr->right) {
          animate.goal = animate.animatePtr->sprite.getPosition();
          animate.goal = sf::Vector2f(animate.goal.x + 100, animate.goal.y + 100);
          animate.temp = animate.newNode.get();
          animate.animatePtr->right = std::move(animate.newNode);
          animate.state = TreeState::INSERT;
        }
        else {
          animate.state = TreeState::HIGHLIGHT;
          animate.animatePtr = animate.animatePtr->right.get();
        }
      }

      delayClock.restart();
    } break;
    case TreeState::INSERT: {
      if (insertNodeAnimation(animate.temp, animate.goal)) {
        resetAnimateState();
        return true;
      }
    } break;
  }
  return false;
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

void BST::draw() {
  if (!root)
    return;

  // dfs or bfs
  preorderTraversal(root.get(), [&](TreeNode* node) {
    window.draw(*node);
  });
  window.draw(cursor);
  if (animate.newNode)
    window.draw(*animate.newNode);
}

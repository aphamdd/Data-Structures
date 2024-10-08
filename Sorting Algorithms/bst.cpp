#pragma once
#include "BST.h"

BST::BST(sf::RenderWindow& win) :
  window(win) {
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
    if (raw.prev)
      node->updateLine(raw.prev, direction);
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

void BST::remove(std::unique_ptr<TreeNode>& node, const int data) {
  if (!node)
    return;

  if (node->data == data) {
    if (raw.active == node.get()) {
      raw.prev = nullptr;
      raw.active = nullptr;
    }
    // leaf
    if (!node->left && !node->right) {
      node.reset();
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
    }
    // 2 children
    else {
      // if there's a left subtree
      TreeNode* current = node->right.get();
      if (current->left.get()) {
        while (current->left->left.get()) {
          current = current->left.get();
        }
        // TODO: operator overloading = for data and dataText
        node->data = current->left->data;
        node->dataText.setString(std::to_string(current->left->data));
        current->left.reset();
        std::cout << "Right subtree with left subtree" << std::endl;
      }
      // if there's no left subtree
      else {
        std::cout << "Right subtree with no left subtree" << std::endl;
        // TODO: operator overloading = for data and dataText
        node->data = node->right->data;
        node->dataText.setString(std::to_string(node->right->data));
        node->right = std::move(node->right->right);
        propogatePos(node->right.get(), sf::Vector2f(-100, -100));
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
  propogatePos(node->left.get(), shift);
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
      if (raw.active->sprite.getColor() == sf::Color::White)
        raw.active->sprite.setColor(sf::Color::Green);
      else if (raw.active->sprite.getColor() == sf::Color::Green)
        raw.active->sprite.setColor(sf::Color::Blue);
      else if (raw.active->sprite.getColor() == sf::Color::Blue)
        raw.active->sprite.setColor(sf::Color::Red);
      else if (raw.active->sprite.getColor() == sf::Color::Red)
        raw.active->sprite.setColor(sf::Color::White);
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
    raw.active = node;
    if (raw.active->sprite.getColor() == sf::Color::White)
      raw.active->sprite.setColor(sf::Color::Green);
    else if (raw.active->sprite.getColor() == sf::Color::Green)
      raw.active->sprite.setColor(sf::Color::Blue);
    else if (raw.active->sprite.getColor() == sf::Color::Blue)
      raw.active->sprite.setColor(sf::Color::Red);
    else if (raw.active->sprite.getColor() == sf::Color::Red)
      raw.active->sprite.setColor(sf::Color::White);
    //updateLines();
    return true;
  }
  raw.prev = node;
  if (search(node->left.get(), pos))
    return true;

  raw.prev = node;
  return search(node->right.get(), pos);
}

bool BST::move(const sf::Vector2i mpos) {
  if (raw.active) {
    sf::Vector2f convertMPos = window.mapPixelToCoords(mpos, window.getView());
    sf::Vector2f lastPos = raw.active->sprite.getPosition();

    raw.active->move(convertMPos); // moves sprite
    raw.active->updateLine2(raw.prev); // update lines to follow sprite
    return true;
  }
  return false;
}

void BST::clear(std::unique_ptr<TreeNode>& node) {
  if (!node)
    return;

  clear(node->left);
  clear(node->right);
  node.reset();
}

void BST::draw() {
  if (!root)
    return;

  // dfs or bfs
  preorderTraversal(root.get(), [&](TreeNode* node) {
    window.draw(*node);
  });
}

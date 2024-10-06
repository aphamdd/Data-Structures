#pragma once
#include "BST.h"

BST::BST(sf::RenderWindow& win) :
  window(win) {
  try {
    if (!GLOBAL::TREETEXTURE.loadFromFile("./Textures/treenodewhite.png"))
      throw std::runtime_error("tree node texture error");
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void BST::insert(std::unique_ptr<TreeNode>& node, const int data, const sf::Vector2f pos, D direction) {
  if (!node) {
    sf::Vector2f newPos;
    if (direction == D::ROOT)
      newPos = pos;
    else if (direction == D::LEFT)
      newPos = sf::Vector2f(pos.x - 100, pos.y + 100);
    else if (direction == D::RIGHT)
      newPos = sf::Vector2f(pos.x + 100, pos.y + 100);
    else
      throw("tree node insert error");
    node = std::make_unique<TreeNode>(data, newPos);
  }
  else {
    data < node->data
      ? insert(node->left, data, node->sprite.getPosition(), D::LEFT)
      : insert(node->right, data, node->sprite.getPosition(), D::RIGHT);
  }

  return;
}

void BST::remove(std::unique_ptr<TreeNode>& node, const int data) {
  if (!node)
    return;

  if (node->data == data) {
    // leaf
    if (!node->left && !node->right) {
      node.reset();
    }
    // 1 child
    else if (!node->left || !node->right) {
      //sf::Vector2f nodePos = node->sprite.getPosition();
      //sf::Vector2f textPos = node->dataText.getPosition();
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
      //propogatePos(node.get(), nodePos, textPos);
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

void BST::search(TreeNode* node, const int data) {
  if (!node)
    return;

  if (node->data == data) {
    node->sprite.setColor(sf::Color::Green);
  }
  else {
    data < node->data
      ? search(node->left.get(), data)
      : search(node->right.get(), data);
  }
  return;
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

#include "pch.h"
#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>
#include "C:\Users\antho\Documents\Code\Sorting Algorithms\Sorting Algorithms\BST.cpp"
#include "C:\Users\antho\Documents\Code\Sorting Algorithms\Sorting Algorithms\treenode.cpp"
#include "C:\Users\antho\Documents\Code\Sorting Algorithms\Sorting Algorithms\common.h"

/* documentation
TEST(TestCaseName, TestName) {
  ASSERT_EQ(1, 1); // fatal equals. used when the condition must hold, otherwise stop
  EXPECT_EQ(1, 1); // non-fatal equals. used when condition should hold, continue
  EXPECT_NE(2, 1); // non-fatal not equals
  EXPECT_TRUE(true);
  // output upon non-fatal errors
  for (int i = 0; i < x.size(); ++i) {
    EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
  }
} */

class BSTTest : public ::testing::Test {
protected:
  sf::RenderWindow window;
  std::unique_ptr<BST> bst;
  void SetUp() override {
    window.create(sf::VideoMode(800, 600), "Test Window");
    bst = std::make_unique<BST>(window);
  }
  void TearDown() override {
    bst->clear();
    window.close();
  }
};

// TEST doesnt require shared setup/test fixtures. TEST_F shares state and uses fixtures
TEST_F(BSTTest, SearchNodeTest) {
  bst->insert(0, sf::Vector2f(1, 1));
  EXPECT_FALSE(bst->findValue(1));
  EXPECT_TRUE(bst->findValue(0));
}

TEST_F(BSTTest, InsertTest) {
  bst->insert(1, sf::Vector2f(1, 1));
  bst->insert(-1, sf::Vector2f(1, 1));
  bst->insert(3, sf::Vector2f(1, 1));
  bst->insert(2, sf::Vector2f(1, 1));
  bst->insert(4, sf::Vector2f(1, 1));
  EXPECT_TRUE(bst->findValue(1));
  EXPECT_TRUE(bst->findValue(-1));
  EXPECT_TRUE(bst->findValue(3));
  EXPECT_TRUE(bst->findValue(2));
  EXPECT_FALSE(bst->findValue(5));
}

#include "pch.h"
#include <SFML/Graphics.hpp>
#include <gtest/gtest.h>
#include "C:\Users\antho\Documents\Code\Sorting Algorithms\Sorting Algorithms\graph.cpp"
#include "C:\Users\antho\Documents\Code\Sorting Algorithms\Sorting Algorithms\common.cpp"

bool vectorCompare(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2) {
  return (rect1.getPosition() == rect2.getPosition()) &&
    (rect1.getSize() == rect2.getSize()) &&
    (rect1.getFillColor() == rect2.getFillColor()) &&
    (rect1.getOutlineColor() == rect2.getOutlineColor()) &&
    (rect1.getOutlineThickness() == rect2.getOutlineThickness());
}
// documentation
TEST(TestCaseName, TestName) {
  ASSERT_EQ(1, 1); // fatal equals
  EXPECT_EQ(1, 1); // non-fatal equals
  EXPECT_NE(2, 1); // non-fatal not equals
  EXPECT_TRUE(true);
  /* output upon non-fatal errors
  for (int i = 0; i < x.size(); ++i) {
    EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
  }
  */  
}

// SEH exception solved with https://stackoverflow.com/questions/13157671/seh-exception-with-code-0xc0000005-thrown-in-the-test-body
TEST(GraphShuffleTest, ShuffleEmptyVector) {
  Graph graph(0);
  std::vector<sf::RectangleShape> dupe = graph.m_histogram;
  graph.shuffle();
  // std::equal(begin1, end1, begin2, customfunc);
  // 
  // [begin, end) as your iterator
  // then it takes begin2 as the comparison, utilizing the same range as [begin, end)
  // finally, it can pass in a custom function as your comparison, or use the default
  EXPECT_TRUE(std::equal(graph.m_histogram.begin(), graph.m_histogram.end(), dupe.begin(), vectorCompare));
}

TEST(GraphShuffleTest, ShuffleOneVector) {
  Graph graph(1);
  std::vector<sf::RectangleShape> dupe = graph.m_histogram;
  graph.shuffle();
  // this is essentially the same thing as std::equal
  /*for (int i = 0; i < graph.m_histogram.size(); ++i) {
    EXPECT_TRUE(vectorCompare(graph.m_histogram.at(i), dupe.at(i)));
  }
  */
  EXPECT_TRUE(std::equal(graph.m_histogram.begin(), graph.m_histogram.end(), dupe.begin(), vectorCompare));
}

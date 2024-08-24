#include "pch.h"
#include "C:\Users\antho\Documents\Code\Sorting Algorithms\Sorting Algorithms\Graph.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(GraphShuffleTest, ShuffleEmptyVector) {
  Graph graph(0);
  graph.shuffle();
  EXPECT_TRUE(graph.m_histogram.empty());
}
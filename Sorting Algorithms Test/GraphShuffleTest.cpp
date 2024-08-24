#include "pch.h"
#include <gtest/gtest.h>
#include "C:\Users\antho\Documents\Code\Sorting Algorithms\Sorting Algorithms\graph.cpp"
#include "C:\Users\antho\Documents\Code\Sorting Algorithms\Sorting Algorithms\common.cpp"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(GraphShuffleTest, ShuffleEmptyVector) {
  Graph graph(0);
  graph.shuffle();
  EXPECT_TRUE(graph.m_histogram.empty());
}
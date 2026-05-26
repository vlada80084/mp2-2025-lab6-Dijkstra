#include <gtest/gtest.h>
#include "Array.h"
#include "Queue.h"
#include "Graph.h"
#include "Heap3.h"
#include "Tree.h"
#include "BHeap.h"
#include "Dijkstra.h"
#include <cstdlib>

TEST(Array, can_create_empty_array) {
    ASSERT_NO_THROW(Array<int> a);
}

TEST(Array, can_add_and_read_elements) {
    Array<int> a;
    a.add(10);
    a.add(20);
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
}

TEST(Array, throws_on_bad_index) {
    Array<int> a;
    a.add(1);
    EXPECT_THROW(a[5], std::out_of_range);
}

TEST(Queue, can_create_empty_queue) {
    ASSERT_NO_THROW(Queue<int> q);
}

TEST(Queue, keeps_order) {
    Queue<int> q;
    q.add(1);
    q.add(2);
    q.add(3);
    EXPECT_EQ(q.take(), 1);
    EXPECT_EQ(q.take(), 2);
    EXPECT_EQ(q.take(), 3);
}

TEST(Queue, throws_on_empty_take) {
    Queue<int> q;
    EXPECT_THROW(q.take(), std::runtime_error);
}

TEST(Graph, can_create_with_positive_size) {
    ASSERT_NO_THROW(Graph g(5));
}

TEST(Graph, throws_on_bad_size) {
    EXPECT_THROW(Graph g(0), std::invalid_argument);
}

TEST(Graph, can_add_road) {
    Graph g(3);
    g.addRoad(0, 1, 5);
    EXPECT_EQ(g.getM(), 1);
    EXPECT_TRUE(g.hasRoad(0, 1));
}

TEST(Graph, throws_on_loop) {
    Graph g(3);
    EXPECT_THROW(g.addRoad(1, 1, 5), std::invalid_argument);
}

TEST(Graph, detects_connected_and_disconnected) {
    Graph g1(3);
    g1.addRoad(0, 1, 1);
    g1.addRoad(1, 2, 1);
    EXPECT_TRUE(g1.isOk());

    Graph g2(4);
    g2.addRoad(0, 1, 1);
    g2.addRoad(2, 3, 1);
    EXPECT_FALSE(g2.isOk());
}

TEST(Graph, makeGraph_makes_connected_graph) {
    std::srand(42);
    Graph g = Graph::makeGraph(10, 15, 1, 10);
    EXPECT_TRUE(g.isOk());
    EXPECT_EQ(g.getM(), 15);
}

TEST(Graph, makeGraph_throws_on_bad_roads_count) {
    EXPECT_THROW(Graph::makeGraph(5, 3, 1, 5), std::invalid_argument);
    EXPECT_THROW(Graph::makeGraph(5, 11, 1, 5), std::invalid_argument);
}

TEST(Heap3, add_and_get_min) {
    Heap3 h;
    h.add(Item(1, 10));
    h.add(Item(2, 5));
    h.add(Item(3, 8));
    EXPECT_EQ(h.getMin().dist, 5);
}

TEST(Heap3, take_returns_sorted_order) {
    Heap3 h;
    int vals[] = { 7, 2, 9, 1, 5 };
    for (int i = 0; i < 5; i++) {
        h.add(Item(i, vals[i]));
    }
    int sorted[] = { 1, 2, 5, 7, 9 };
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(h.takeMin().dist, sorted[i]);
    }
}

TEST(Heap3, throws_on_empty_take) {
    Heap3 h;
    EXPECT_THROW(h.takeMin(), std::runtime_error);
}

TEST(Tree, add_and_get_min) {
    Tree t;
    t.add(Item(0, 10));
    t.add(Item(1, 3));
    t.add(Item(2, 7));
    EXPECT_EQ(t.getMin().dist, 3);
}

TEST(Tree, take_returns_sorted) {
    Tree t;
    int vals[] = { 5, 2, 8, 1, 9 };
    for (int i = 0; i < 5; i++) {
        t.add(Item(i, vals[i]));
    }
    int sorted[] = { 1, 2, 5, 8, 9 };
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(t.takeMin().dist, sorted[i]);
    }
}

TEST(Tree, throws_on_empty) {
    Tree t;
    EXPECT_THROW(t.getMin(), std::runtime_error);
}

TEST(BHeap, add_and_get_min) {
    BHeap h;
    h.add(Item(0, 5));
    h.add(Item(1, 2));
    h.add(Item(2, 8));
    EXPECT_EQ(h.getMin().dist, 2);
}

TEST(BHeap, take_returns_sorted) {
    BHeap h;
    int vals[] = { 9, 3, 7, 1, 4 };
    for (int i = 0; i < 5; i++) {
        h.add(Item(i, vals[i]));
    }
    int sorted[] = { 1, 3, 4, 7, 9 };
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(h.takeMin().dist, sorted[i]);
    }
}

TEST(BHeap, throws_on_empty) {
    BHeap h;
    EXPECT_THROW(h.takeMin(), std::runtime_error);
}

TEST(Dijkstra, simple_graph_works) {
    Graph g(5);
    g.addRoad(0, 1, 4);
    g.addRoad(0, 2, 1);
    g.addRoad(2, 1, 2);
    g.addRoad(1, 3, 1);
    g.addRoad(3, 4, 3);
    Answer a = dijkstra3(g, 0);
    EXPECT_EQ(a.dist[1], 3);
    EXPECT_EQ(a.dist[4], 7);
}

TEST(Dijkstra, both_heaps_give_same_result) {
    std::srand(11);
    Graph g = Graph::makeGraph(20, 50, 1, 100);
    Answer a1 = dijkstra3(g, 0);
    Answer a2 = dijkstraB(g, 0);
    for (int i = 0; i < a1.dist.size(); i++) {
        EXPECT_EQ(a1.dist[i], a2.dist[i]);
    }
}

TEST(Dijkstra, path_recovery_works) {
    Graph g(4);
    g.addRoad(0, 1, 1);
    g.addRoad(1, 2, 1);
    g.addRoad(2, 3, 1);
    g.addRoad(0, 3, 10);
    Answer a = dijkstra3(g, 0);
    EXPECT_EQ(a.dist[3], 3);
    Array<int> path = a.getPath(3);
    EXPECT_EQ(path.size(), 4);
    EXPECT_EQ(path[0], 0);
    EXPECT_EQ(path[3], 3);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#ifndef TREE_H
#define TREE_H

#include "Heap3.h"
#include "Array.h"

struct Node {//узел дерева
    Item value;//что в узле
    int height;//высота поддерева
    Node* left;//левый ребенок
    Node* right;//правый ребенок

    Node(const Item& v) {
        value = v;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

class Tree {
private:
    Node* root;//корень
    int count;//кол-во элементов

    int getHeight(Node* p) const;//высота узла
    int getBalance(Node* p) const;//насколько сбалансированное
    void fixHeight(Node* p);//пересчитываем высоту
    Node* turnRight(Node* p);
    Node* turnLeft(Node* p);
    Node* balance(Node* p);//выравниваем узел
    Node* addNode(Node* p, const Item& v);
    Node* findMinNode(Node* p) const;
    Node* removeMinNode(Node* p);
    void clearAll(Node* p);//очистить поддерево
    void walk(Node* p, Array<Item>& out) const;

public:
    Tree();
    ~Tree();
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    void add(const Item& v);
    Item getMin() const;
    Item takeMin();
    int size() const;
    bool isEmpty() const;
    void addAll(Tree& other);//перенос элементов
    void toArray(Array<Item>& out) const;//запись в массив
};

#endif

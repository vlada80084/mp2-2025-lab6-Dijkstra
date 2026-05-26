#ifndef B_HEAP_H
#define B_HEAP_H

#include "Tree.h"
#include "Array.h"

struct Part {//одно дерево внутри биномиальной кучи
    int order;//порядок (0 = 1 элемент, 1 = 2 элемента, 2 = 4, ...)
    Tree* tree;

    Part() {
        order = 0;
        tree = nullptr;
    }
    Part(int o, Tree* t) {
        order = o;
        tree = t;
    }
};

class BHeap {
private:
    Array<Part> parts;//маленькие деревья

    Part mergeTwo(Part& a, Part& b);//соединяем 2 дерева
    void cleanUp();//убираем деревья одинакового порядка
    void freeAll();

public:
    BHeap();
    ~BHeap();
    BHeap(const BHeap&) = delete;//запрет на копирование и присваивание
    BHeap& operator=(const BHeap&) = delete;

    void add(const Item& v);//добавление элемента
    Item getMin() const;//просмотр минимума
    Item takeMin();//взятие минимума
    int size() const;
    bool isEmpty() const;
};

#endif

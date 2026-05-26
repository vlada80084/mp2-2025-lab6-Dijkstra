#ifndef HEAP3_H
#define HEAP3_H

#include "Array.h"

struct Item {
    int point;//номер точки
    long long dist;//дистанция точки

    Item() {//по умолчанию
        point = -1;
        dist = 0;
    }
    Item(int p, long long d) {//по значению параметров
        point = p;
        dist = d;
    }
};

class Heap3 {
private:
    Array<Item> data;

    void goUp(int i);//поднимаем элемент, пока он меньше родителя
    void goDown(int i);//опускаем элемент, пока он больше детей

public:
    Heap3();

    void add(const Item& x);
    Item getMin() const;
    Item takeMin();
    bool isEmpty() const;
    int size() const;
};

#endif

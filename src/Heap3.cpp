#include "Heap3.h"
#include <stdexcept>

Heap3::Heap3() {}//пустой конструктор: Array внутри сам создается пустым

void Heap3::goUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 3;//у элемента i родитель имеет номер (i-1)/3
        if (data[i].dist < data[parent].dist) {
            Item tmp = data[i];//меняем местами
            data[i] = data[parent];
            data[parent] = tmp;
            i = parent;
        }
        else { break; }
    }
}

void Heap3::goDown(int i) {
    int n = data.size();
    while (true) {
        int firstChild = i * 3 + 1;//у элемента i дети: 3*i+1, 3*i+2, 3*i+3
        if (firstChild >= n) { break; }//детей нет
        int minChild = firstChild;//ищем самого маленького ребенка
        if (firstChild + 1 < n) {
            if (data[firstChild + 1].dist < data[minChild].dist) {
                minChild = firstChild + 1;
            }
        }
        if (firstChild + 2 < n) {
            if (data[firstChild + 2].dist < data[minChild].dist) {
                minChild = firstChild + 2;
            }
        }
        if (data[minChild].dist < data[i].dist) {
            Item tmp = data[i];
            data[i] = data[minChild];
            data[minChild] = tmp;
            i = minChild;
        }
        else { break; }
    }
}

void Heap3::add(const Item& x) {//кладем в конец и поднимаем
    data.add(x);
    goUp(data.size() - 1);
}

Item Heap3::getMin() const {//минимум - в корне
    if (data.isEmpty()) {
        throw std::runtime_error("heap is empty");
    }
    return data[0];
}

Item Heap3::takeMin() {
    //запоминаем корень, в корень кладем последний, удаляем последний, опускаем
    if (data.isEmpty()) {
        throw std::runtime_error("heap is empty");
    }
    Item result = data[0];
    int n = data.size();
    if (n == 1) {
        data.removeLast();
    }
    else {
        data[0] = data[n - 1];
        data.removeLast();
        goDown(0);
    }
    return result;
}

bool Heap3::isEmpty() const {
    return data.isEmpty();
}

int Heap3::size() const {
    return data.size();
}

#include "BHeap.h"
#include <stdexcept>

BHeap::BHeap() {}

BHeap::~BHeap() {
    freeAll();
}

void BHeap::freeAll() {//освобождаем все деревья
    for (int i = 0; i < parts.size(); i++) {
        if (parts[i].tree != nullptr) {
            delete parts[i].tree;
            parts[i].tree = nullptr;
        }
    }
    parts.clear();
}

Part BHeap::mergeTwo(Part& a, Part& b) {
    //сливаем два дерева одинакового порядка:
    //все элементы из b переезжают в a, дерево b удаляем
    a.tree->addAll(*b.tree);
    delete b.tree;
    b.tree = nullptr;
    return Part(a.order + 1, a.tree);
}

void BHeap::cleanUp() {
    //в куче не должно быть двух деревьев одного порядка
    //если есть - сливаем их в одно дерево большего порядка
    if (parts.size() <= 1) { return; }

    //сортируем деревья по порядку (сортировка вставкой)
    for (int i = 1; i < parts.size(); i++) {
        Part cur = parts[i];
        int j = i - 1;
        while (j >= 0 && parts[j].order > cur.order) {
            parts[j + 1] = parts[j];
            j = j - 1;
        }
        parts[j + 1] = cur;
    }

    //идем по списку и сливаем подряд идущие деревья одного порядка
    Array<Part> newParts;
    int i = 0;
    while (i < parts.size()) {
        Part cur = parts[i];
        while (i + 1 < parts.size() && parts[i + 1].order == cur.order) {
            Part merged = mergeTwo(cur, parts[i + 1]);
            cur = merged;
            i = i + 1;
        }
        newParts.add(cur);
        i = i + 1;
    }
    parts = newParts;

    //если после слияний снова появились одинаковые порядки - повторить
    bool needAgain = false;
    for (int k = 0; k + 1 < parts.size(); k++) {
        if (parts[k].order == parts[k + 1].order) {
            needAgain = true;
            break;
        }
    }
    if (needAgain) {
        cleanUp();
    }
}

void BHeap::add(const Item& v) {
    //делаем новое дерево с одним элементом (порядок 0), кладем в кучу, прибираемся
    Tree* t = new Tree();
    t->add(v);
    parts.add(Part(0, t));
    cleanUp();
}

Item BHeap::getMin() const {
    //перебираем минимумы всех деревьев и выбираем самый маленький
    if (parts.isEmpty()) {
        throw std::runtime_error("heap is empty");
    }
    Item best = parts[0].tree->getMin();
    for (int i = 1; i < parts.size(); i++) {
        Item cur = parts[i].tree->getMin();
        if (cur.dist < best.dist) {
            best = cur;
        }
        else if (cur.dist == best.dist && cur.point < best.point) {
            best = cur;
        }
    }
    return best;
}

Item BHeap::takeMin() {
    if (parts.isEmpty()) {
        throw std::runtime_error("heap is empty");
    }

    //находим индекс дерева с минимальным элементом
    int bestIdx = 0;
    Item best = parts[0].tree->getMin();
    for (int i = 1; i < parts.size(); i++) {
        Item cur = parts[i].tree->getMin();
        if (cur.dist < best.dist) {
            best = cur;
            bestIdx = i;
        }
        else if (cur.dist == best.dist && cur.point < best.point) {
            best = cur;
            bestIdx = i;
        }
    }

    //удаляем минимум из этого дерева
    Item result = parts[bestIdx].tree->takeMin();

    //собираем оставшиеся элементы
    Array<Item> leftover;
    parts[bestIdx].tree->toArray(leftover);

    //удаляем это дерево из кучи
    delete parts[bestIdx].tree;
    parts[bestIdx].tree = nullptr;
    for (int i = bestIdx; i + 1 < parts.size(); i++) {
        parts[i] = parts[i + 1];
    }
    parts.removeLast();

    //добавляем оставшиеся элементы обратно
    for (int i = 0; i < leftover.size(); i++) {
        add(leftover[i]);
    }

    return result;
}

int BHeap::size() const {
    int s = 0;
    for (int i = 0; i < parts.size(); i++) {
        s = s + parts[i].tree->size();
    }
    return s;
}

bool BHeap::isEmpty() const {
    return size() == 0;
}

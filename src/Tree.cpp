#include "Tree.h"
#include <stdexcept>

//сравнение: сначала по dist, при равенстве по point
//(так все элементы становятся различимы, даже с одинаковым dist)
static bool less(const Item& a, const Item& b) {
    if (a.dist != b.dist) { return a.dist < b.dist; }
    return a.point < b.point;
}

Tree::Tree() {
    root = nullptr;
    count = 0;
}

Tree::~Tree() {
    clearAll(root);
}

int Tree::getHeight(Node* p) const {//высота узла (0 для пустого)
    if (p == nullptr) { return 0; }
    return (*p).height;
}

int Tree::getBalance(Node* p) const {
    if (p == nullptr) { return 0; }
    return getHeight((*p).left) - getHeight((*p).right);
}

void Tree::fixHeight(Node* p) {//высота = 1 + макс высоты детей
    int hl = getHeight((*p).left);
    int hr = getHeight((*p).right);
    if (hl > hr) {
        (*p).height = hl + 1;
    }
    else {
        (*p).height = hr + 1;
    }
}

Node* Tree::turnRight(Node* p) {//правый поворот
    Node* q = (*p).left;
    (*p).left = (*q).right;
    (*q).right = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

Node* Tree::turnLeft(Node* p) {//левый поворот
    Node* q = (*p).right;
    (*p).right = (*q).left;
    (*q).left = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

Node* Tree::balance(Node* p) {//выравниваем узел поворотами
    fixHeight(p);
    int bf = getBalance(p);
    if (bf == 2) {//перекос влево
        if (getBalance((*p).left) < 0) {
            (*p).left = turnLeft((*p).left);
        }
        return turnRight(p);
    }
    if (bf == -2) {//перекос вправо
        if (getBalance((*p).right) > 0) {
            (*p).right = turnRight((*p).right);
        }
        return turnLeft(p);
    }
    return p;
}

Node* Tree::addNode(Node* p, const Item& v) {//вставка в поддерево
    if (p == nullptr) {
        return new Node(v);
    }
    if (less(v, (*p).value)) {
        (*p).left = addNode((*p).left, v);
    }
    else {
        (*p).right = addNode((*p).right, v);
    }
    return balance(p);
}

void Tree::add(const Item& v) {
    root = addNode(root, v);
    count = count + 1;
}

Node* Tree::findMinNode(Node* p) const {//минимум - самый левый узел
    if (p == nullptr) { return nullptr; }
    while ((*p).left != nullptr) {
        p = (*p).left;
    }
    return p;
}

Item Tree::getMin() const {
    if (root == nullptr) {
        throw std::runtime_error("tree is empty");
    }
    return (*findMinNode(root)).value;
}

Node* Tree::removeMinNode(Node* p) {//удаляем самый левый, возвращаем новый корень
    if ((*p).left == nullptr) {
        Node* r = (*p).right;
        delete p;
        return r;
    }
    (*p).left = removeMinNode((*p).left);
    return balance(p);
}

Item Tree::takeMin() {
    if (root == nullptr) {
        throw std::runtime_error("tree is empty");
    }
    Item m = (*findMinNode(root)).value;
    root = removeMinNode(root);
    count = count - 1;
    return m;
}

int Tree::size() const {
    return count;
}

bool Tree::isEmpty() const {
    return count == 0;
}

void Tree::clearAll(Node* p) {//освободить поддерево
    if (p == nullptr) { return; }
    clearAll((*p).left);
    clearAll((*p).right);
    delete p;
}

void Tree::walk(Node* p, Array<Item>& out) const {//обход дерева в порядке возрастания
    if (p == nullptr) { return; }
    walk((*p).left, out);
    out.add((*p).value);
    walk((*p).right, out);
}

void Tree::toArray(Array<Item>& out) const {
    walk(root, out);
}

void Tree::addAll(Tree& other) {
    //собираем элементы other в массив, вставляем сюда, очищаем other
    if (&other == this) { return; }
    Array<Item> tmp;
    other.toArray(tmp);
    for (int i = 0; i < tmp.size(); i++) {
        add(tmp[i]);
    }
    other.clearAll(other.root);
    other.root = nullptr;
    other.count = 0;
}

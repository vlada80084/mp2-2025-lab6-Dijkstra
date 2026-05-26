#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <stdexcept>

template <typename T>
class Queue {
private:
    T* data;
    int cap;
    int head;//откуда
    int tail;//куда
    int len;

    void makeBigger() {
        int _cap = cap * 2;
        T* _data = new T[_cap];
        for (int i = 0; i < len; i++) {
            _data[i] = data[(head + i) % cap];
        }
        delete[] data;
        data = _data;
        cap = _cap;
        head = 0;
        tail = len;
    }

public:
    Queue() {//пустая очередь
        cap = 8;
        head = 0;
        tail = 0;
        len = 0;
        data = new T[cap];
    }

    ~Queue() {
        delete[] data;
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void add(const T& x) {//кладем в конец
        if (len == cap) {
            makeBigger();
        }
        data[tail] = x;
        tail = (tail + 1) % cap;
        len = len + 1;
    }

    T take() {//берем из начала
        if (len == 0) {
            throw std::runtime_error("queue is empty");
        }
        T x = data[head];
        head = (head + 1) % cap;
        len = len - 1;
        return x;
    }

    bool isEmpty() const {
        return len == 0;
    }

    int size() const {
        return len;
    }
};

#endif

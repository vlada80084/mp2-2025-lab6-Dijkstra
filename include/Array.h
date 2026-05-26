#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <stdexcept>

template <typename T>
class Array {
private:
    T* data;
    int len;//количество элементов
    int cap;//количество выделенных ячеек

    void makeBigger(int _cap) {//увеличиваем массив, если нужно
        T* _data = new T[_cap];
        for (int i = 0; i < len; i++) {
            _data[i] = data[i];
        }
        delete[] data;
        data = _data;
        cap = _cap;
    }

public:
    Array() {//конструктор по умолчанию
        cap = 4;
        len = 0;
        data = new T[cap];
    }

    Array(int n) {//конструктор по числу элементов
        if (n < 0) { throw std::out_of_range("wrong index"); }
        else {
            cap = n;
            len = n;
        }
        data = new T[cap];
    }

    Array(const Array& other) {//конструктор копирования
        cap = other.cap;
        len = other.len;
        data = new T[cap];
        for (int i = 0; i < len; i++) { data[i] = other.data[i]; }
    }

    Array& operator=(const Array& other) {//присваивание массивов
        if (this == &other) {
            return *this;
        }
        delete[] data;
        cap = other.cap;
        len = other.len;
        data = new T[cap];
        for (int i = 0; i < len; i++) { data[i] = other.data[i]; }
        return *this;
    }

    ~Array() {
        delete[] data;
    }

    void add(const T& x) {//добавляем элемент в конец
        if (len >= cap) {
            makeBigger(cap * 2);
        }
        data[len] = x;
        len = len + 1;
    }

    void removeLast() {//удаляем последний элемент
        if (len > 0) {
            len = len - 1;
        }
    }

    int size() const {
        return len;
    }

    bool isEmpty() const {
        return (len == 0);
    }

    void clear() {
        len = 0;
    }

    T& operator[](int i) {
        if (i < 0 || i >= len) {
            throw std::out_of_range("wrong index");
        }
        return data[i];
    }
    const T& operator[](int i) const {
        if (i < 0 || i >= len) {
            throw std::out_of_range("wrong index");
        }
        return data[i];
    }

    void resize(int n) {//изменение размера
        if (n > cap) {
            makeBigger(n);
        }
        len = n;
    }
};

#endif

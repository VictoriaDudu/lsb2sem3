#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

using namespace std;

template <typename T>
struct Vector {
private:
    T* data;           // Указатель на массив данных
    int len;           // Текущая длина
    int cap;           // Максимальная емкость
    float loadFactor;  // Коэффициент загрузки

public:
    Vector(int initCap = 16, int initLen = 0) {
        cap = initCap;
        len = 0;
        data = new T[cap];
        loadFactor = 0.5;
    }

    int size() const {
        return len;
    }

    void extend() {
        int newCap = cap * 2;
        T* newData = new T[newCap];
        for (int i = 0; i < cap; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

    void pushBack(T value) {
        if (static_cast<float>(len) / cap >= loadFactor) {
            extend();
        }
        data[len] = value;
        len++;
    }

    void insert(int index, T value) {
        if (static_cast<float>(len) / cap >= loadFactor) {
            extend();
        }
        for (int i = len; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        len++;
    }

    void remove(int index) {
        if (index < 0 || index >= len) {
            throw invalid_argument("invalid index");
        }

        for (int i = index; i < len - 1; i++) {
            data[i] = data[i + 1];
        }
        len--;
    }

    void set(int index, T value) {
        if (index < 0 || index >= len) {
            throw invalid_argument("invalid index");
        }
        data[index] = value;
    }

    void reverse() {
        for (int i = 0; i < len / 2; i++) {
            T temp = data[i];
            data[i] = data[len - i - 1];
            data[len - i - 1] = temp;
        }
    }

    void sort() {
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                if (data[i] < data[j]) {
                    T temp = data[i];
                    data[i] = data[j];
                    data[j] = temp;
                }
            }
        }
    }

    T get(int index) const {
        if (index < 0 || index >= len) {
            throw invalid_argument("invalid index");
        }

        return data[index];
    }

    int find(T value) {
        for (int i = 0; i < len; i++) {
            if (data[i] == value) {
                return i;
            }
        }
        return -1;
    }

    int min() {
        if (len == 0) {
            throw runtime_error("Vector is empty");
        }

        int minValue = data[0];
        for (int i = 1; i < len; i++) {
            if (data[i] < minValue) {
                minValue = data[i];
            }
        }
        return minValue;
    }
    T* begin() {
        return data;
    }

    // Итератор end
    T* end() {
        return data + len;
    }
    // Функция contains для поиска элемента в векторе
    bool contains(const T& value) const {
        for (int i = 0; i < len; i++) {
            if (data[i] == value) {
                return true;  // Элемент найден
            }
        }
        return false;  // Элемент не найден
    }
};

ostream& operator<<(ostream& os, const pair<char, char>& p) {
    os << p.first << "," << p.second;
    return os;
}

template <typename T>
ostream& operator<<(ostream& os, const Vector<T>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        os << vec.get(i) << ";";
    }
    os << endl;
    return os;
}

#endif
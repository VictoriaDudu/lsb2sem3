
#pragma once
#ifndef STACK_H
#define STACK_H

#include "slist.hpp"  // Предполагаем, что у вас есть этот файл, реализующий односвязный список.

template <typename T>
struct Stack {
private:
    SList<T> list;

public:
    Stack() {}

    // Возвращает количество элементов в стеке
    int size() const {
        return list.size();
    }

    // Возвращает данные списка
    SList<T> data() const {
        return list;
    }

    // Добавление элемента в стек
    void push(const T& value) {
        list.pushForward(value);
    }

    // Удаление верхнего элемента из стека
    T pop() {
        if (list.head == nullptr) {
            throw runtime_error("Stack is empty");
        }
        T popped = list.head->value;
        list.removeForward();
        return popped;
    }

    // Проверка, пуст ли стек
    bool Empty() const {
        return list.head == nullptr;
    }

    // Получение верхнего элемента стека (не удаляя его)
    T top() const {
        if (list.head == nullptr) {
            throw runtime_error("Stack is empty");
        }
        return list.head->value;
    }
};

template <typename T>
ostream& operator<<(ostream& os, const Stack<T>& stack) {
    os << stack.data() << endl;
    return os;
}

#endif
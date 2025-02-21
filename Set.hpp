
#pragma once
#ifndef SET_H
#define SET_H

#include <string>
#include <iostream>
#include <cstdint>
#include "Vector.hpp"

using namespace std;

template <typename T>
struct Set {
private:
    // Структура для элемента с обобщённым типом
    struct Val {
        T value;
        Val* next;

        Val(T v) : value(v), next(nullptr) {}
    };

    Val** data;  // Указатель на массив указателей на элементы
    int cap;      // Емкость множества
    int len;      // Текущая длина множества
    double loadFactor;  // Коэффициент загрузки

public:
    // Конструктор множества
    Set() {
        cap = 16;
        len = 0;
        loadFactor = 0.50;
        data = new Val * [cap];

        for (int i = 0; i < cap; i++) {
            data[i] = nullptr;
        }
    }

    // Получение данных
    Val** getData() const {
        return data;
    }

    int getLen() const {
        return len;
    }

    int getCap() const {
        return cap;
    }

    // Добавление элемента в множество
    void put(T value) {
        if (static_cast<double>(len) / cap >= loadFactor) {
            rehash(cap * 2);  // Перехеширование при необходимости
        }

        uint32_t index = hash(value) % cap;

        if (data[index] == nullptr) {  // Если бакет пустой
            data[index] = new Val(value);  // Создаём новый элемент
        }
        else {  // Если бакет не пустой -> добавляем в него
            Val* current = data[index];
            if (current->value == value) {
                return;  // Если элемент уже существует, ничего не делаем
            }

            while (current->next != nullptr) {
                if (current->value == value) {
                    return;  // Если элемент уже существует, ничего не делаем
                }
                current = current->next;
            }

            current->next = new Val(value);  // Добавляем новый элемент в список
        }

        len++;
    }

    // Получение элемента по значению
    T get(T value) const {
        uint32_t index = hash(value) % cap;
        Val* current = data[index];

        while (current != nullptr) {
            if (current->value == value) {
                return current->value;
            }
            current = current->next;
        }

        throw runtime_error("Value not found");
    }

    // Проверка наличия элемента в множестве
    bool contains(T value) const {
        try {
            get(value);
            return true;
        }
        catch (runtime_error&) {
            return false;
        }
    }

    // Удаление элемента из множества
    void remove(T value) {
        uint32_t index = hash(value) % cap;
        Val* prev = nullptr;
        Val* current = data[index];

        while (current != nullptr) {
            if (current->value == value) {
                if (prev == nullptr) {
                    data[index] = current->next;
                }
                else {
                    prev->next = current->next;
                }
                delete current;
                len--;
                return;
            }
            prev = current;
            current = current->next;
        }

        throw runtime_error("Value not found");
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Не удалось открыть файл для чтения" << endl;
            return; // Return if file cannot be opened
        }

        T value;
        while (file >> value) { // Assuming values are space-separated
            put(value);
        }

        file.close();
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            cout << "Не удалось открыть файл для записи" << endl;
            return; // Return if file cannot be opened
        }

        for (int i = 0; i < cap; i++) {
            Val* current = data[i];
            while (current) {
                file << current->value << " "; // Output values space-separated
                current = current->next;
            }
        }

        file.close();
    }

    // Хеш-функция
    uint32_t hash(T value) const {
        uint32_t hash = 5381;
        hash = ((hash << 5) + hash) + std::hash<T>{}(value);  // Применяем стандартную хеш-функцию для типа T
        return hash;
    }

    // Перехеширование
    void rehash(int newCap) {
        Val** newSet = new Val * [newCap];  // Новый массив для бакетов увеличенной емкости

        for (int i = 0; i < newCap; i++) {
            newSet[i] = nullptr;
        }

        for (int i = 0; i < cap; i++) {
            Val* current = data[i];
            while (current != nullptr) {
                Val* next = current->next;
                uint32_t newIndex = hash(current->value) % newCap;

                if (newSet[newIndex] == nullptr) {
                    current->next = nullptr;
                    newSet[newIndex] = current;
                }
                else {
                    Val* currentNewSet = newSet[newIndex];
                    while (currentNewSet->next != nullptr) {
                        currentNewSet = currentNewSet->next;
                    }
                    current->next = nullptr;
                    currentNewSet->next = current;
                }
                current = next;
            }
        }

        delete[] data;
        data = newSet;
        cap = newCap;
    }

    // Возвращает все элементы множества в виде вектора
    Vector<T> values() const {
        Vector<T> result;

        for (int i = 0; i < cap; i++) {
            Val* node = data[i];

            while (node != nullptr) {
                result.pushBack(node->value);
                node = node->next;
            }
        }

        return result;
    }

    // Соединяет все элементы в одну строку
    string join() const {
        string result = "";

        for (int i = 0; i < cap; i++) {
            Val* current = data[i];
            while (current != nullptr) {
                result += to_string(current->value) + ",";  // Преобразуем значения в строку
                current = current->next;
            }
        }
        result = result.substr(0, result.size() - 1);  // Убираем последнюю запятую
        return result;
    }

    // Перегрузка оператора вывода
    friend ostream& operator<<(ostream& os, const Set<T>& set) {
        auto data = set.getData();
        for (int i = 0; i < set.getCap(); i++) {
            if (data[i] != nullptr) {
                auto current = data[i];
                while (current != nullptr) {
                    os << current->value << "  ";
                    current = current->next;
                }
            }
        }
        return os;
    }
};

#endif

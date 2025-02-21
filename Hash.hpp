#pragma once
#ifndef HASHTAB_H_INCLUDED
#define HASHTAB_H_INCLUDED
#include <string>
#include <iostream>
#include <stdexcept>
#include "Vector.hpp"
#define LOADFACTOR 0.75
using namespace std;

template <typename T>
struct Map {
    struct Item {
        string key;
        T value;
        Item* next;

        Item(string k, T v) : key(k), value(v), next(nullptr) {}
    };

    Item** data;
    int cap;
    int len;

    Map() {
        cap = 16;
        len = 0;


        data = new Item * [cap];

        for (int i = 0; i < cap; i++) {
            data[i] = nullptr;
        }
    }

    Item** getData() const {
        return data;
    }

    int getLen() const {
        return len;
    }

    int getCap() const {
        return cap;
    }

    Vector<string> keys() const {
        Vector<string> result;

        for (int i = 0; i < cap; i++) {
            Item* node = data[i];

            while (node != nullptr) {
                result.pushBack(node->key);
                node = node->next;
            }
        }

        return result;
    }

    uint32_t hash(string key) const {
        uint32_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash;
    }
    T& operator[](const string& key) {
        uint32_t index = hash(key) % cap;

        Item* current = data[index];
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;  // ���� ���� ������, ���������� ������ �� ��� ��������
            }
            current = current->next;
        }

        // ���� ���� �� ������, ��������� ����� �������
        if (static_cast<double>(len) / cap >= LOADFACTOR) {
            rehash(cap * 2);  // ���� �������� ������� ������� �������, ����������� � ������
        }

        // ������ ����� ������� � ��������� ���������
        Item* newItem = new Item(key, T());  // T() � ��� ����� ������������ ��� ���� T (�� ���������)

        // ��������� ��� � ������ ������
        newItem->next = data[index];
        data[index] = newItem;

        len++;

        return newItem->value;  // ���������� ������ �� �������� ������ ��������
    }
    void put(string key, T value) {
        if (static_cast<double>(len) / cap >= LOADFACTOR) {
            rehash(cap * 2);
        }

        uint32_t index = hash(key) % cap;

        if (data[index] == nullptr) { // ������ ���
            data[index] = new Item(key, value);
        }
        else { // ����� ���� -> ��������� � ����
            Item* current = data[index];
            if (current->key == key) {
                current->value = value;
                return;
            }

            while (current->next != nullptr) {
                if (current->key == key) {
                    current->value = value;
                    return;
                }
                current = current->next;
            }

            current->next = new Item(key, value);
        }

        len++;
    }

    T get(string key) const {
        uint32_t index = hash(key) % cap;

        Item* current = data[index];
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        throw runtime_error("Key not found");
    }

    bool contains(string key) {
        try {
            get(key);
        }
        catch (runtime_error& e) {
            return false;
        }
        return true;
    }

    void remove(string key) {
        uint32_t index = hash(key) % cap;

        Item* prev = nullptr;
        Item* current = data[index];

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    data[index] = current->next;
                    delete current;
                    len--;
                    return;
                }
                else {
                    prev = current->next;
                    delete current;
                    len--;
                    return;
                }
            }
            prev = current;
            current = current->next;
        }

        throw runtime_error("Key not found");
    }

    void rehash(int newcap) {
        Item** newMap = new Item * [newcap];

        for (int i = 0; i < newcap; i++) {
            newMap[i] = nullptr;
        }

        for (int i = 0; i < cap; i++) {
            Item* current = data[i];
            while (current != nullptr) {
                Item* next = current->next;
                uint32_t newIndex = hash(current->key) % newcap;

                if (newMap[newIndex] == nullptr) { // ������ ���
                    current->next = nullptr;
                    newMap[newIndex] = current;
                }
                else { // ����� ���� -> ��������� � ����
                    Item* currentNewMap = newMap[newIndex];

                    while (currentNewMap->next != nullptr) {
                        currentNewMap = currentNewMap->next;
                    }
                    current->next = nullptr;
                    currentNewMap->next = current;
                }
                current = next;
            }
        }

        delete[] data;
        data = newMap;
        cap = newcap;
    }

    string join() {
        string result = "";

        for (int i = 0; i < cap; i++) {
            Item* current = data[i];
            while (current != nullptr) {
                result += current->key + "," + current->value + ";";
                current = current->next;
            }
        }
        result = result.substr(0, result.size() - 1);
        return result;
    }
};

template <typename T>
ostream& operator<<(ostream& os, const Map<T>& map) {
    auto data = map.getData();
    for (int i = 0; i < map.getCap(); i++) {
        if (data[i] != nullptr) {
            auto current = data[i];
            os << i << ": ";
            while (current != nullptr) {
                os << current->key << " - " << current->value << ", ";
                current = current->next;
            }
            os << endl;
        }
    }
    return os;
}


#endif

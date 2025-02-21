#pragma once
#ifndef BLOOMFILTER_H_INCLUDED
#define BLOOMFILTER_H_INCLUDED

#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include "Hash.hpp" // Подключение вашего хеш-кода (Map)

template<typename T>
class BloomFilter {
private:
    std::vector<bool> bitArray;
    size_t numBits;
    size_t numHashFunctions;

public:
    BloomFilter(size_t size, size_t hashFunctions) : numBits(size), numHashFunctions(hashFunctions) {
        bitArray.resize(numBits);
    }

    // Хэш-функция, возвращающая индекс в битовом массиве
    size_t hash1(const std::string& key) const {
        return std::hash<std::string>{}(key) % numBits;
    }

    size_t hash2(const std::string& key) const {
        std::hash<std::string> hasher;
        return (hasher(key) / numBits) % numBits;
    }

    // Добавление элемента в фильтр Блума
    void add(const std::string& key) {
        for (size_t i = 0; i < numHashFunctions; ++i) {
            size_t hashValue = (hash1(key) + i * hash2(key)) % numBits;
            bitArray[hashValue] = true;
        }
    }

    // Проверка, существует ли элемент в фильтре
    bool contains(const std::string& key) const {
        for (size_t i = 0; i < numHashFunctions; ++i) {
            size_t hashValue = (hash1(key) + i * hash2(key)) % numBits;
            if (!bitArray[hashValue]) {
                return false; // Если хотя бы один бит не установлен, элемента точно нет
            }
        }
        return true; // Все обычно установленные биты
    }

    // Пример вывода
    void print() const {
        for (size_t i = 0; i < numBits; ++i) {
            std::cout << bitArray[i] << " ";
        }
        std::cout << std::endl;
    }
};

#endif

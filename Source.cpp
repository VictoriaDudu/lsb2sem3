#include "Set.hpp"
#include <iostream>
#include <string>
#include "Vector.hpp"
#include "stack.hpp"
#include "BST.hpp"
#include "Hash.hpp"
#include "Bloom.hpp"
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
using namespace std;


void task1() {
    // Ввод данных с клавиатуры: задачи
    vector<string> tasks_input;
    cout << "Введите задачи через запятую (например, A,B,C): ";
    string tasks_str;
    getline(cin, tasks_str); // Читаем всю строку ввода

    string task;
    for (size_t i = 0; i < tasks_str.length(); ++i) {
        if (tasks_str[i] == ',') {
            task.erase(remove_if(task.begin(), task.end(), ::isspace), task.end());
            tasks_input.push_back(task);
            task = "";
        }
        else {
            task += tasks_str[i];
        }
    }
    task.erase(remove_if(task.begin(), task.end(), ::isspace), task.end());
    tasks_input.push_back(task);

    // Ввод данных с клавиатуры: зависимости
    vector<pair<string, string>> dependencies_input;
    cout << "Введите зависимости в формате (A,B) через запятую (например, (A,B),(B,C)): ";
    string dependencies_str;
    getline(cin, dependencies_str);

    string dependency;
    string first, second;
    bool parsing_first = true;
    for (size_t i = 0; i < dependencies_str.length(); ++i) {
        if (dependencies_str[i] == '(') {
            parsing_first = true;
        }
        else if (dependencies_str[i] == ',') {
            parsing_first = false;
            first.erase(remove_if(first.begin(), first.end(), ::isspace), first.end());
        }
        else if (dependencies_str[i] == ')') {
            second.erase(remove_if(second.begin(), second.end(), ::isspace), second.end());
            dependencies_input.push_back(make_pair(first, second));
            first = "";
            second = "";
        }
        else if (dependencies_str[i] != ' ') {
            if (parsing_first) {
                first += dependencies_str[i];
            }
            else {
                second += dependencies_str[i];
            }
        }
    }

    // Создаем SList задач
    SList<string> tasks;
    for (const auto& task : tasks_input) {
        tasks.pushBack(task);
    }

    // Создаем map зависимостей
    unordered_map<string, unordered_set<string>> dependencies;
    for (const auto& dep : dependencies_input) {
        dependencies[dep.first].insert(dep.second);
    }

    // Для хранения степени входа для каждой задачи
    unordered_map<string, int> inDegree;

    // Инициализация inDegree для всех задач
    SNode<string>* current = tasks.head;
    while (current != nullptr) {
        inDegree[current->value] = 0; // Изначально 0 для всех задач
        current = current->next;
    }

    // Подсчитываем степень входа для задач
    for (const auto& dep : dependencies) {
        for (const auto& task : dep.second) {
            inDegree[task]++;
        }
    }

    // Инициализация стека и добавление задач с нулевой степенью входа
    Stack<string> stack;
    for (const auto& task : inDegree) {
        if (task.second == 0) {
            stack.push(task.first);
        }
    }

    // Количество задач, которые мы можем выполнить
    int count = 0;

    // Выполняем топологическую сортировку
    while (!stack.Empty()) {
        string task = stack.pop();
        count++;

        // Уменьшаем степень входа всех зависимых задач
        if (dependencies.count(task) > 0) { // Добавлена проверка
            for (const auto& dep : dependencies[task]) {
                inDegree[dep]--;
                if (inDegree[dep] == 0) {
                    stack.push(dep);
                }
            }
        }
    }

    // Проверяем, сможем ли мы выполнить все задачи
    if (count == inDegree.size()) {
        cout << "Возможно" << std::endl;
    }
    else {
        cout << "Невозможно" << std::endl;
    }
}

void extractPairs(const string& genome, Set<string>& pairsSet) {
    for (size_t i = 0; i < genome.length() - 1; ++i) {
        string pair = string(1, genome[i]) + genome[i + 1];
        pairsSet.put(pair);
    }
}


void task2() {
    Set<string> mySet;
    const string filename = "fail.data";

    // Load initial data from the file
    mySet.loadFromFile(filename);

    string command;
    string value;
    cout << "Доступные команды: \n";
    cout << "add <value> - добавить элемент \n";
    cout << "delete <value> - удалить элемент \n";
    cout << "contains <value> - проверить наличие элемента \n";
    cout << "print - вывести все элементы \n";
    cout << "exit - завершить программу \n";

    while (true) {
        cout << "> ";
        cin >> command;

        if (command == "add") {
            cin >> value;
            mySet.put(value);
            mySet.saveToFile(filename); // Save to file after addition
        }
        else if (command == "delete") {
            cin >> value;
            mySet.remove(value);
            mySet.saveToFile(filename); // Save to file after deletion
        }
        else if (command == "contains") {
            cin >> value;
            cout << (mySet.contains(value) ? "Да" : "Нет") << std::endl;
        }
        else if (command == "print") {
            cout << mySet << "\n";
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "Неизвестная команда. Пожалуйста, попробуйте снова.\n";
        }
    }
}
void task3() {
    // Ввод двух геномов
    string genome1, genome2;
    cout << "Введите первый геном: ";
    cin >> genome1;
    cout << "Введите второй геном: ";
    cin >> genome2;

    // Создаем множество для хранения пар второго генома
    Set<string> pairsSet2;
    extractPairs(genome2, pairsSet2);

    // Считаем степень близости
    int closeness = 0;

    // Проверяем, сколько пар из первого генома присутствуют во втором геноме
    for (size_t i = 0; i < genome1.length() - 1; ++i) {
        string pair = string(1, genome1[i]) + genome1[i + 1];
        if (pairsSet2.contains(pair)) {
            closeness++;
        }
    }

    // Выводим результат
    cout << "Степень близости: " << closeness << endl;
    
}

// Функция для подсчета ударений в слове
int countStresses(const string& word) {
    int count = 0;
    for (char c : word) {
        if (isupper(c)) {
            count++;
        }
    }
    return count;
}

// Функция для проверки корректности слова
bool isCorrectWord(const string& word, const Vector<string>& dictionary) {
    int stressCount = countStresses(word);

    // Если слово есть в словаре
    if (dictionary.contains(word)) {
        return stressCount > 0;  // Слово правильное, если есть хотя бы одно ударение
    }
    else {
        return stressCount == 1;  // Если слово отсутствует, оно правильное только с одним ударением
    }
}

void task4() {
    int N;
    cin >> N;
    cin.ignore(); // Очистка буфера

    Vector<string> dictionary;  // Словарь слов с правильными ударениями
    for (int i = 0; i < N; i++) {
        string word;
        getline(cin, word);
        dictionary.pushBack(word);  // Добавляем слово в словарь
    }

    string text;
    getline(cin, text);  // Читаем строку текста

    istringstream iss(text);
    string word;
    int errorCount = 0;

    // Проверяем каждое слово в тексте
    while (iss >> word) {
        if (!isCorrectWord(word, dictionary)) {
            errorCount++;  // Увеличиваем счетчик ошибок, если слово неверное
        }
    }

    cout << errorCount << endl; // Выводим количество ошибок
}
void findLeaves(Node* node) {
    if (node != nullptr) {
        //рекурсивно обходим (сначала левое поддерево)
        findLeaves(node->left);
        //если у узла нет потомков, это лист
        if (node->left == nullptr && node->right == nullptr) {
            cout << node->data << " "; // Выводим данные листа
        }
        //рекурсивно обходим правое поддерево
        findLeaves(node->right);
    }
}

void task5() {
BST tree;

    cout << "Введите количество узлов в дереве: ";
    int n;
    cin >> n;

    cout << "Введите узлы дерева: " << endl;
    for (int i = 0; i < n; ++i) {
        int value;
        cin >> value;
        tree.insert(value);
    }

    if (tree.isBST()) {
        cout << "Дерево является бинарным деревом поиска." << endl;
    } else {
        cout << "Дерево НЕ является бинарным деревом поиска." << endl;
    }

    cout << "BST structure (formatted):" << endl;
    tree.printTree();
    cout << endl;
}

int countDescendants(const string& name, Map<int>& descendantsCount, Map<string>& tree) {
    // Если количество потомков для этого имени уже вычислено, возвращаем кэшированное значение
    if (descendantsCount.contains(name)) {
        return descendantsCount.get(name);
    }

    int count = 0;

    // Если в дереве есть потомки для этого имени, считаем их количество
    if (tree.contains(name)) {
        string children = tree.get(name); // Получаем строку с детьми
        stringstream ss(children); // Разбиваем строку на детей
        string child;

        // Рекурсивно считаем количество потомков для каждого ребенка
        while (ss >> child) {
            count += 1 + countDescendants(child, descendantsCount, tree); // 1 за ребенка + его потомков
        }
    }

    // Сохраняем количество потомков для этого имени в кэш
    descendantsCount.put(name, count);
    return count;
}

void task6() {
    BloomFilter<int> bloomFilter(1000, 5); // Размер битового массива 1000, 5 хэш-функций

    // Добавление элементов
    bloomFilter.add("apple");
    bloomFilter.add("banana");
    bloomFilter.add("orange");

    // Проверка наличия элементов
    cout << "Contains apple: " << bloomFilter.contains("apple") << std::endl;   // true
    cout << "Contains banana: " << bloomFilter.contains("banana") << std::endl; // true
    cout << "Contains grape: " << bloomFilter.contains("grape") << std::endl;   // false

    // Вывод битового массива
    cout << "Bloom Filter Bit Array: ";
    bloomFilter.print();
}
int main() {
    setlocale(LC_ALL, "Russian");
    cout << "-------------------Task 1------------------" << endl;
    task1();
    cout << "-------------------Task 2------------------" << endl;
    task2();
    cout << "-------------------Task 3------------------" << endl;
	task3();
    cout << "-------------------Task 4------------------" << endl;
    task4();
    cout << "-------------------Task 5------------------" << endl;
    task5();
    cout <<endl<< "-------------------Task 6------------------" << endl;
    task6();

}
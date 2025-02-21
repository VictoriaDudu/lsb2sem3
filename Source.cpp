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
    // ���� ������ � ����������: ������
    vector<string> tasks_input;
    cout << "������� ������ ����� ������� (��������, A,B,C): ";
    string tasks_str;
    getline(cin, tasks_str); // ������ ��� ������ �����

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

    // ���� ������ � ����������: �����������
    vector<pair<string, string>> dependencies_input;
    cout << "������� ����������� � ������� (A,B) ����� ������� (��������, (A,B),(B,C)): ";
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

    // ������� SList �����
    SList<string> tasks;
    for (const auto& task : tasks_input) {
        tasks.pushBack(task);
    }

    // ������� map ������������
    unordered_map<string, unordered_set<string>> dependencies;
    for (const auto& dep : dependencies_input) {
        dependencies[dep.first].insert(dep.second);
    }

    // ��� �������� ������� ����� ��� ������ ������
    unordered_map<string, int> inDegree;

    // ������������� inDegree ��� ���� �����
    SNode<string>* current = tasks.head;
    while (current != nullptr) {
        inDegree[current->value] = 0; // ���������� 0 ��� ���� �����
        current = current->next;
    }

    // ������������ ������� ����� ��� �����
    for (const auto& dep : dependencies) {
        for (const auto& task : dep.second) {
            inDegree[task]++;
        }
    }

    // ������������� ����� � ���������� ����� � ������� �������� �����
    Stack<string> stack;
    for (const auto& task : inDegree) {
        if (task.second == 0) {
            stack.push(task.first);
        }
    }

    // ���������� �����, ������� �� ����� ���������
    int count = 0;

    // ��������� �������������� ����������
    while (!stack.Empty()) {
        string task = stack.pop();
        count++;

        // ��������� ������� ����� ���� ��������� �����
        if (dependencies.count(task) > 0) { // ��������� ��������
            for (const auto& dep : dependencies[task]) {
                inDegree[dep]--;
                if (inDegree[dep] == 0) {
                    stack.push(dep);
                }
            }
        }
    }

    // ���������, ������ �� �� ��������� ��� ������
    if (count == inDegree.size()) {
        cout << "��������" << std::endl;
    }
    else {
        cout << "����������" << std::endl;
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
    cout << "��������� �������: \n";
    cout << "add <value> - �������� ������� \n";
    cout << "delete <value> - ������� ������� \n";
    cout << "contains <value> - ��������� ������� �������� \n";
    cout << "print - ������� ��� �������� \n";
    cout << "exit - ��������� ��������� \n";

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
            cout << (mySet.contains(value) ? "��" : "���") << std::endl;
        }
        else if (command == "print") {
            cout << mySet << "\n";
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "����������� �������. ����������, ���������� �����.\n";
        }
    }
}
void task3() {
    // ���� ���� �������
    string genome1, genome2;
    cout << "������� ������ �����: ";
    cin >> genome1;
    cout << "������� ������ �����: ";
    cin >> genome2;

    // ������� ��������� ��� �������� ��� ������� ������
    Set<string> pairsSet2;
    extractPairs(genome2, pairsSet2);

    // ������� ������� ��������
    int closeness = 0;

    // ���������, ������� ��� �� ������� ������ ������������ �� ������ ������
    for (size_t i = 0; i < genome1.length() - 1; ++i) {
        string pair = string(1, genome1[i]) + genome1[i + 1];
        if (pairsSet2.contains(pair)) {
            closeness++;
        }
    }

    // ������� ���������
    cout << "������� ��������: " << closeness << endl;
    
}

// ������� ��� �������� �������� � �����
int countStresses(const string& word) {
    int count = 0;
    for (char c : word) {
        if (isupper(c)) {
            count++;
        }
    }
    return count;
}

// ������� ��� �������� ������������ �����
bool isCorrectWord(const string& word, const Vector<string>& dictionary) {
    int stressCount = countStresses(word);

    // ���� ����� ���� � �������
    if (dictionary.contains(word)) {
        return stressCount > 0;  // ����� ����������, ���� ���� ���� �� ���� ��������
    }
    else {
        return stressCount == 1;  // ���� ����� �����������, ��� ���������� ������ � ����� ���������
    }
}

void task4() {
    int N;
    cin >> N;
    cin.ignore(); // ������� ������

    Vector<string> dictionary;  // ������� ���� � ����������� ����������
    for (int i = 0; i < N; i++) {
        string word;
        getline(cin, word);
        dictionary.pushBack(word);  // ��������� ����� � �������
    }

    string text;
    getline(cin, text);  // ������ ������ ������

    istringstream iss(text);
    string word;
    int errorCount = 0;

    // ��������� ������ ����� � ������
    while (iss >> word) {
        if (!isCorrectWord(word, dictionary)) {
            errorCount++;  // ����������� ������� ������, ���� ����� ��������
        }
    }

    cout << errorCount << endl; // ������� ���������� ������
}
void findLeaves(Node* node) {
    if (node != nullptr) {
        //���������� ������� (������� ����� ���������)
        findLeaves(node->left);
        //���� � ���� ��� ��������, ��� ����
        if (node->left == nullptr && node->right == nullptr) {
            cout << node->data << " "; // ������� ������ �����
        }
        //���������� ������� ������ ���������
        findLeaves(node->right);
    }
}

void task5() {
BST tree;

    cout << "������� ���������� ����� � ������: ";
    int n;
    cin >> n;

    cout << "������� ���� ������: " << endl;
    for (int i = 0; i < n; ++i) {
        int value;
        cin >> value;
        tree.insert(value);
    }

    if (tree.isBST()) {
        cout << "������ �������� �������� ������� ������." << endl;
    } else {
        cout << "������ �� �������� �������� ������� ������." << endl;
    }

    cout << "BST structure (formatted):" << endl;
    tree.printTree();
    cout << endl;
}

int countDescendants(const string& name, Map<int>& descendantsCount, Map<string>& tree) {
    // ���� ���������� �������� ��� ����� ����� ��� ���������, ���������� ������������ ��������
    if (descendantsCount.contains(name)) {
        return descendantsCount.get(name);
    }

    int count = 0;

    // ���� � ������ ���� ������� ��� ����� �����, ������� �� ����������
    if (tree.contains(name)) {
        string children = tree.get(name); // �������� ������ � ������
        stringstream ss(children); // ��������� ������ �� �����
        string child;

        // ���������� ������� ���������� �������� ��� ������� �������
        while (ss >> child) {
            count += 1 + countDescendants(child, descendantsCount, tree); // 1 �� ������� + ��� ��������
        }
    }

    // ��������� ���������� �������� ��� ����� ����� � ���
    descendantsCount.put(name, count);
    return count;
}

void task6() {
    BloomFilter<int> bloomFilter(1000, 5); // ������ �������� ������� 1000, 5 ���-�������

    // ���������� ���������
    bloomFilter.add("apple");
    bloomFilter.add("banana");
    bloomFilter.add("orange");

    // �������� ������� ���������
    cout << "Contains apple: " << bloomFilter.contains("apple") << std::endl;   // true
    cout << "Contains banana: " << bloomFilter.contains("banana") << std::endl; // true
    cout << "Contains grape: " << bloomFilter.contains("grape") << std::endl;   // false

    // ����� �������� �������
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
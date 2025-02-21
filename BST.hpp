#include <iostream>
#include <queue>
using namespace std;

struct Node {
    int data;    
    Node* left; 
    Node* right; 

    Node(int val) : data(val), left(nullptr), right(nullptr) {} // Конструктор узла
};

// Класс для представления бинарного дерева поиска
struct BST {
    Node* root;//корень

    Node* createNode(int data) {
        return new Node(data);
    }

    //метод для вставки узла в дерево
    Node* insertNode(Node* node, int data) {
        if (node == nullptr) { //если дерево пустое, возвращаем новый узел
            return createNode(data);
        }

        //иначе, рекурсивно вставляем данные в нужное место
        if (data < node->data) {
            node->left = insertNode(node->left, data);
        }
        else if (data > node->data) {
            node->right = insertNode(node->right, data);
        }

        //возвращаем указатель на текущий узел
        return node;
    }

    // Метод для проверки, является ли дерево BST
    bool isBSTUtil(Node* node, int minValue, int maxValue) const {
        if (node == nullptr) {
            return true; // Пустое поддерево - это BST
        }

        // Проверяем текущий узел, чтобы убедиться, что он находится в допустимом интервале
        if (node->data <= minValue || node->data >= maxValue) {
            return false;
        }

        // Проверяем, является ли левое и правое поддерево BST
        return isBSTUtil(node->left, minValue, node->data) &&
            isBSTUtil(node->right, node->data, maxValue);
    }

    bool isBST() const {
        return isBSTUtil(root, INT_MIN, INT_MAX);
    }



    //метод для поиска узла
    Node* searchNode(Node* node, int key) const {
        if (node == nullptr || node->data == key) {
            return node;
        }

        //если ключ больше, идем в правое поддерево
        if (node->data < key) {
            return searchNode(node->right, key);
        }

        // Если ключ меньше, идем в левое поддерево
        return searchNode(node->left, key);
    }

    // Метод для нахождения минимального узла в правом поддереве
    Node* minValueNode(Node* node) const {
        Node* current = node;
        while (current && current->left != nullptr) { 
            current = current->left;
        }
        return current;
    }

    // Метод для удаления узла из дерева
    Node* deleteNode(Node* root, int data) {
        if (root == nullptr) {
            return root;
        }

        // Если данные меньше текущего узла, ищем в левом поддереве
        if (data < root->data) {
            root->left = deleteNode(root->left, data);
        }
        // Если данные больше текущего узла, ищем в правом поддереве
        else if (data > root->data) {
            root->right = deleteNode(root->right, data);
        }
        else { // Если данные совпадают с текущим узлом
            // Узел с одним или без детей
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            // Узел с двумя детьми: находим преемника (минимум в правом поддереве)
            Node* temp = minValueNode(root->right);

            // Копируем данные преемника в текущий узел
            root->data = temp->data;

            // Удаляем преемника
            root->right = deleteNode(root->right, temp->data);
        }
        return root;
    }

    // Конструктор
    BST() : root(nullptr) {}

    // Метод для вставки узла в дерево
    void insert(int data) {
        root = insertNode(root, data);
    }

    // Метод для поиска узла
    bool search(int key) const {
        Node* foundNode = searchNode(root, key);
        return foundNode != nullptr;
    }

    // Метод для удаления узла
    void deleteValue(int data) {
        root = deleteNode(root, data);
    }

    friend ostream& operator<<(ostream& os, const BST& bst) {
        if (bst.root == nullptr) {
            os << "Empty BST" << endl;
            return os;
        }

        queue<Node*> q;
        q.push(bst.root);

        while (!q.empty()) {
            int levelSize = q.size();
            for (int i = 0; i < levelSize; ++i) {
                Node* current = q.front();
                q.pop();

                if (current == nullptr) {
                    os << "N" << " "; // Use "N" to represent null nodes
                }
                else {
                    os << current->data << " ";
                    q.push(current->left);
                    q.push(current->right);
                }
            }
            os << endl;
        }
        return os;
    }

    // Another printing method, better for large trees:
    void printTree(Node* root, int space = 0, int COUNT = 5) const {
        // Base case
        if (root == nullptr)
            return;

        // Increase distance between levels
        space += COUNT;

        // Process right child first
        printTree(root->right, space);

        // Print current node after space
        // count
        cout << endl;
        for (int i = COUNT; i < space; i++)
            cout << " ";
        cout << root->data << "\n";

        // Process left child
        printTree(root->left, space);
    }

    void printTree() const {
        printTree(root);
    }
};


#include <iostream>
#include <queue>
using namespace std;

struct Node {
    int data;    
    Node* left; 
    Node* right; 

    Node(int val) : data(val), left(nullptr), right(nullptr) {} // ����������� ����
};

// ����� ��� ������������� ��������� ������ ������
struct BST {
    Node* root;//������

    Node* createNode(int data) {
        return new Node(data);
    }

    //����� ��� ������� ���� � ������
    Node* insertNode(Node* node, int data) {
        if (node == nullptr) { //���� ������ ������, ���������� ����� ����
            return createNode(data);
        }

        //�����, ���������� ��������� ������ � ������ �����
        if (data < node->data) {
            node->left = insertNode(node->left, data);
        }
        else if (data > node->data) {
            node->right = insertNode(node->right, data);
        }

        //���������� ��������� �� ������� ����
        return node;
    }

    // ����� ��� ��������, �������� �� ������ BST
    bool isBSTUtil(Node* node, int minValue, int maxValue) const {
        if (node == nullptr) {
            return true; // ������ ��������� - ��� BST
        }

        // ��������� ������� ����, ����� ���������, ��� �� ��������� � ���������� ���������
        if (node->data <= minValue || node->data >= maxValue) {
            return false;
        }

        // ���������, �������� �� ����� � ������ ��������� BST
        return isBSTUtil(node->left, minValue, node->data) &&
            isBSTUtil(node->right, node->data, maxValue);
    }

    bool isBST() const {
        return isBSTUtil(root, INT_MIN, INT_MAX);
    }



    //����� ��� ������ ����
    Node* searchNode(Node* node, int key) const {
        if (node == nullptr || node->data == key) {
            return node;
        }

        //���� ���� ������, ���� � ������ ���������
        if (node->data < key) {
            return searchNode(node->right, key);
        }

        // ���� ���� ������, ���� � ����� ���������
        return searchNode(node->left, key);
    }

    // ����� ��� ���������� ������������ ���� � ������ ���������
    Node* minValueNode(Node* node) const {
        Node* current = node;
        while (current && current->left != nullptr) { 
            current = current->left;
        }
        return current;
    }

    // ����� ��� �������� ���� �� ������
    Node* deleteNode(Node* root, int data) {
        if (root == nullptr) {
            return root;
        }

        // ���� ������ ������ �������� ����, ���� � ����� ���������
        if (data < root->data) {
            root->left = deleteNode(root->left, data);
        }
        // ���� ������ ������ �������� ����, ���� � ������ ���������
        else if (data > root->data) {
            root->right = deleteNode(root->right, data);
        }
        else { // ���� ������ ��������� � ������� �����
            // ���� � ����� ��� ��� �����
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

            // ���� � ����� ������: ������� ��������� (������� � ������ ���������)
            Node* temp = minValueNode(root->right);

            // �������� ������ ��������� � ������� ����
            root->data = temp->data;

            // ������� ���������
            root->right = deleteNode(root->right, temp->data);
        }
        return root;
    }

    // �����������
    BST() : root(nullptr) {}

    // ����� ��� ������� ���� � ������
    void insert(int data) {
        root = insertNode(root, data);
    }

    // ����� ��� ������ ����
    bool search(int key) const {
        Node* foundNode = searchNode(root, key);
        return foundNode != nullptr;
    }

    // ����� ��� �������� ����
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


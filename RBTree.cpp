#include <iostream>
using namespace std;

enum Color { RED, BLACK };

template <typename T>
class RedBlackTree {
private:
    struct Node {
        T data;
        Color color;
        Node* parent;
        Node* left;
        Node* right;

        Node(T value) : data(value), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
    };

    Node* root;
    Node* NIL;

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != NIL)
            y->left->parent = x;

        y->parent = x->parent;
        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != NIL)
            y->right->parent = x;

        y->parent = x->parent;
        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node*& root, Node* u, Node* v) {
        if (u->parent == NIL)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    Node* minValueNode(Node* node) {
        while (node->left != NIL)
            node = node->left;
        return node;
    }

    void fixDelete(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void deleteTree(Node* node) {
        if (node != NIL) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    void printHelper(Node* node, string prefix = "", bool isLeft = false) {
        if (node != NIL) {
            if (node->right != NIL)
                printHelper(node->right, prefix + (isLeft ? "│   " : "    "), false);

            cout << prefix;
            cout << (isLeft ? "└──" : "┌──");
            cout << node->data << "(" << (node->color == RED ? "R" : "B") << ")" << endl;

            if (node->left != NIL)
                printHelper(node->left, prefix + (isLeft ? "    " : "│   "), true);
        }
    }

public:
    RedBlackTree() {
        NIL = new Node(T());
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = nullptr;
        root = NIL;
    }

    ~RedBlackTree() {
        deleteTree(root);
        delete NIL;
    }

    void insert(T key) {
        Node* node = new Node(key);
        node->left = node->right = node->parent = NIL;

        Node* y = NIL;
        Node* x = root;

        while (x != NIL) {
            y = x;
            if (node->data < x->data)
                x = x->left;
            else
                x = x->right;
        }

        node->parent = y;
        if (y == NIL)
            root = node;
        else if (node->data < y->data)
            y->left = node;
        else
            y->right = node;

        node->color = RED;
        fixInsert(node);
    }

    void remove(T key) {
        Node* z = root;
        Node* x;
        Node* y;

        while (z != NIL) {
            if (z->data == key)
                break;
            else if (key < z->data)
                z = z->left;
            else
                z = z->right;
        }

        if (z == NIL) {
            cout << "Key not found in the tree" << endl;
            return;
        }

        y = z;
        Color yOriginalColor = y->color;

        if (z->left == NIL) {
            x = z->right;
            transplant(root, z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(root, z, z->left);
        } else {
            y = minValueNode(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z)
                x->parent = y;
            else {
                transplant(root, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(root, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        if (yOriginalColor == BLACK)
            fixDelete(x);
    }

    void printTree() {
        if (root == NIL) cout << "Tree is empty.\n";
        else printHelper(root);
    }
};


int main()
{
    RedBlackTree<int> rbtree;
    int choice;
    int value;
    int count;

    cout << "Введите количество элементов для добавления: ";
    cin >> count;

    cout << "Введите " << count << " элементов (через пробел или каждый с новой строки):\n";
    for (int i = 0; i < count; i++) {
        cin >> value;
        rbtree.insert(value);
        cout << "Добавлен элемент: " << value << endl;
    }

    cout << "\nИтоговое дерево после добавления элементов:\n";
    rbtree.printTree();

    // меню для работы с деревом
    while (true) {
        cout << "\nМеню операций:\n";
        cout << "1. Добавить элемент\n";
        cout << "2. Удалить элемент\n";
        cout << "3. Вывести дерево\n";
        cout << "4. Выход\n";
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Введите значение для добавления: ";
                cin >> value;
                rbtree.insert(value);
                cout << "Элемент " << value << " добавлен в дерево.\n";
                break;
            }
            case 2: {
                cout << "Введите значение для удаления: ";
                cin >> value;
                rbtree.remove(value);
                cout << "Элемент " << value << " удален из дерева.\n";
                break;
            }
            case 3: {
                cout << "Текущее состояние дерева:\n";
                rbtree.printTree();
                break;
            }
            case 4: {
                cout << "Выход из программы.\n";
                return 0;
            }
            default: {
                cout << "Неверный выбор. Попробуйте снова.\n";
                break;
            }
        }
    }
}
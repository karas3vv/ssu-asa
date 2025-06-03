#include <iostream>
#include <queue>
#include <math.h>
using namespace std;

struct Node { // структура для узла дерева
    int key; // значение ключа узла
    Node *left; // указатель на левого потомка
    Node *right; // указатель на правого потомка
    int height; // высота узла

    Node(int k) { key = k; left = nullptr; right = nullptr; height = 1;} // конструктор
};

int height(Node *N) { // функция для получения высоты узла
    if (N == nullptr)
        return 0; // если узел пустой, то высота 0
    return N->height; // иначе возвращаем высоту
}

Node *rightRotate(Node *y) { // функция правого вращения
    Node *x = y->left; // x — левый потомок y
    Node *T2 = x->right; // временное хранилище правого поддерева x

    x->right = y; // вращение: x становится корнем, y — правым ребенком
    y->left = T2; // поддерево T2 становится левым ребенком y

    y->height = 1 + max(height(y->left), height(y->right)); // обновляем высоту y
    x->height = 1 + max(height(x->left), height(x->right)); // обновляем высоту x

    return x; // возвращаем новый корень поддерева
}

Node *leftRotate(Node *x) { // функция левого вращения
    Node *y = x->right; // y — правый потомок x
    Node *T2 = y->left; // временное хранилище левого поддерева y

    y->left = x; // вращение: y становится корнем, x — левым ребенком
    x->right = T2; // поддерево T2 становится правым ребенком x

    x->height = 1 + max(height(x->left), height(x->right)); // обновляем высоту x
    y->height = 1 + max(height(y->left), height(y->right)); // обновляем высоту y

    return y; // возвращаем новый корень поддерева
}

int getBalance(Node *N) { // получаем баланс узла
    if (N == nullptr)
        return 0; // если узел пустой, то баланс 0
    return height(N->left) - height(N->right); // разность высот левого и правого поддерева
}

Node* insert(Node* node, int key) { // функция вставки узла
    if (node == nullptr)
        return new Node(key); // создаем новый узел, если дерево пусто

    if (key < node->key) // если ключ меньше текущего
        node->left = insert(node->left, key); // вставляем в левое поддерево
    else if (key > node->key) // если ключ больше
        node->right = insert(node->right, key); // вставляем в правое поддерево
    else 
        return node; // дубликаты не вставляем

    node->height = 1 + max(height(node->left), height(node->right)); // обновляем высоту узла

    int balance = getBalance(node); // получаем баланс узла

    // случаи дисбаланса:
    if (balance > 1 && key < node->left->key) // левый левый случай
        return rightRotate(node);

    if (balance < -1 && key > node->right->key) // правый правый случай
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) { // левый правый случай
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) { // правый левый случай
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // возвращаем (возможно обновленный) узел
}

Node * minValueNode(Node* node) { // функция поиска минимального узла в поддереве
    Node* current = node;
    while (current->left != nullptr) // идем влево до упора
        current = current->left;

    return current; // возвращаем самый левый узел
}

Node* deleteNode(Node* root, int key) { // функция удаления узла
    if (root == nullptr)
        return root; // если дерево пусто, ничего не делаем

    if (key < root->key) // ищем ключ в левом поддереве
        root->left = deleteNode(root->left, key);
    else if (key > root->key) // ищем ключ в правом поддереве
        root->right = deleteNode(root->right, key);
    else { // нашли узел
        if ((root->left == nullptr) || (root->right == nullptr)) { // случай 0 или 1 ребенок
            Node *temp = root->left ? root->left : root->right; // выбираем единственного ребенка

            if (temp == nullptr) { // нет детей
                temp = root;
                root = nullptr;
            } else // один ребенок
                *root = *temp; // копируем содержимое

            free(temp); // удаляем временный узел
        } else {
            Node* temp = minValueNode(root->right); // ищем минимальный в правом поддереве
            root->key = temp->key; // копируем его значение
            root->right = deleteNode(root->right, temp->key); // удаляем этот узел
        }
    }

    if (root == nullptr)
        return root; // если дерево стало пустым

    root->height = 1 + max(height(root->left), height(root->right)); // обновляем высоту
    int balance = getBalance(root); // получаем баланс

    // восстанавливаем баланс
    if (balance > 1 && getBalance(root->left) >= 0) // левый левый
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) { // левый правый
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0) // правый правый
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) { // правый левый
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root; // возвращаем обновленный корень
}

void preOrder(Node *root) { // прямой (префиксный) обход
    if (root != nullptr) {
        cout << root->key << " "; // выводим значение узла
        preOrder(root->left); // обходим левое поддерево
        preOrder(root->right); // обходим правое поддерево
    }
}

void print(Node *tr, int k) { // функция печати дерева в виде уровней
    if (!tr)
        cout << "Дерево пустое\n"; // если дерево пустое
    else {
        queue<Node *> cur, next; // очереди для текущего и следующего уровня
        Node *r = tr;
        cur.push(r); // начинаем с корня
        int j = 0;
        while (cur.size()) {
            if (j == 0) {
                for (int i = 0; i < (int)pow(2.0, k) - 1; i++)
                    cout << " "; // отступ в начале строки
            }
            Node *buf = cur.front();
            cur.pop();
            j++;
            if (buf) {
                cout << buf->key; // выводим значение узла
                next.push(buf->left); // добавляем детей в очередь
                next.push(buf->right);
                for (int i = 0; i < (int)pow(2.0, k + 1) - 1; i++)
                    cout << " "; // отступ между узлами
            } else {
                for (int i = 0; i < (int)pow(2.0, k + 1) - 1; i++)
                    cout << " "; // отступ для пустого места
                cout << " ";
            }
            if (cur.empty()) {
                cout << endl; // переход на следующую строку
                swap(cur, next); // переходим к следующему уровню
                j = 0;
                k--; // уменьшаем уровень
            }
        }
    }
}

int main() {
    int n, x, d;
    cout << "Введите количество элементов в дереве: ";
    cin >> n;

    Node *root = nullptr;

    cout << "Введите элементы в дерева: " << endl;;
    for(int i = 0; i < n; i++){
        cin >> x;
        root = insert(root, x);
    }

	int k = int(log((float)n) / log((float)2.0));
    print (root, k);

    cout << "Введите ключ для удаления узла: ";
    cin >> d;
    root = deleteNode(root, d);

    cout << "После удаления" << endl; 
    print (root, k);

    return 0;
}
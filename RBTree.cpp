#include <iostream>
using namespace std;

// перечисление для цветов узлов в красно-черном дереве
enum Color { RED, BLACK };

// шаблон класса для красно-черного дерева
template <typename T> class RedBlackTree {
private:
    // структура для узла
    struct Node {
        T data;         // данные узла
        Color color;    // цвет узла (RED или BLACK)
        Node* parent;   // указатель на родителя
        Node* left;     // указатель на левого потомка
        Node* right;    // указатель на правого потомка

        // конструктор для инициализации узла с данными и цветом
        Node(T value)
            : data(value)
            , color(RED)    // новые узлы всегда красные
            , parent(nullptr)
            , left(nullptr)
            , right(nullptr)
        {
        }
    };

    Node* root; // корень красно-черного дерева

    // вспомогательная функция: левый поворот
    void rotateLeft(Node*& node)
    {
        Node* child = node->right;   // правый потомок становится новым корнем
        node->right = child->left;   // левое поддерево потомка становится правым поддеревом узла
        if (node->right != nullptr)
            node->right->parent = node;  // обновляем родителя для перенесенного поддерева
        child->parent = node->parent;   // переносим родителя узла в потомка
        if (node->parent == nullptr)
            root = child;           // если узел был корнем, теперь корень - потомок
        else if (node == node->parent->left)
            node->parent->left = child; // обновляем ссылку родителя на потомка
        else
            node->parent->right = child;
        child->left = node;        // делаем узел левым потомком
        node->parent = child;      // обновляем родителя узла
    }

    // вспомогательная функция: правый поворот (аналогично левому)
    void rotateRight(Node*& node)
    {
        Node* child = node->left;
        node->left = child->right;
        if (node->left != nullptr)
            node->left->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->right = node;
        node->parent = child;
    }

    // вспомогательная функция: исправление нарушений при вставке
    void fixInsert(Node*& node)
    {
        Node* parent = nullptr;
        Node* grandparent = nullptr;
        while (node != root && node->color == RED
               && node->parent->color == RED) {
            parent = node->parent;
            grandparent = parent->parent;
            if (parent == grandparent->left) {  // если родитель - левый потомок
                Node* uncle = grandparent->right;
                if (uncle != nullptr && uncle->color == RED) {  // случай 1: дядя красный
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->right) {  // случай 2: узел - правый потомок
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    // случай 3: узел - левый потомок
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else {  // симметричный случай, если родитель - правый потомок
                Node* uncle = grandparent->left;
                if (uncle != nullptr && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = BLACK;  // корень всегда черный
    }

    // вспомогательная функция: исправление нарушений при удалении
    void fixDelete(Node*& node)
    {
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {  // если узел - левый потомок
                Node* sibling = node->parent->right;
                if (sibling->color == RED) {  // случай 1: брат красный
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }
                if ((sibling->left == nullptr || sibling->left->color == BLACK)
                    && (sibling->right == nullptr || sibling->right->color == BLACK)) {  // случай 2: оба ребенка брата черные
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->right == nullptr || sibling->right->color == BLACK) {  // случай 3: правый ребенок брата черный
                        if (sibling->left != nullptr)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }
                    // случай 4: правый ребенок брата красный
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                    rotateLeft(node->parent);
                    node = root;
                }
            }
            else {  // симметричный случай, если узел - правый потомок
                Node* sibling = node->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }
                if ((sibling->left == nullptr || sibling->left->color == BLACK)
                    && (sibling->right == nullptr || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->left == nullptr || sibling->left->color == BLACK) {
                        if (sibling->right != nullptr)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;  // делаем узел черным
    }

    // вспомогательная функция: поиск узла с минимальным значением
    Node* minValueNode(Node*& node)
    {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;  // идем до крайнего левого узла
        return current;
    }

    // вспомогательная функция: замена узлов в дереве
    void transplant(Node*& root, Node*& u, Node*& v)
    {
        if (u->parent == nullptr)
            root = v;  // если u - корень
        else if (u == u->parent->left)
            u->parent->left = v;  // если u - левый потомок
        else
            u->parent->right = v; // если u - правый потомок
        if (v != nullptr)
            v->parent = u->parent;  // обновляем родителя для v
    }

    // вспомогательная функция: вывод дерева
    void printHelper(Node* node, string prefix = "", bool isLeft = false)
{
    if (node != nullptr) {
        if (node->right)
            printHelper(node->right, prefix + (isLeft ? "│   " : "    "), false);

        cout << prefix;
        cout << (isLeft ? "└──" : "┌──");
        cout << node->data << "(" << (node->color == RED ? "R" : "B") << ")" << endl;

        if (node->left)
            printHelper(node->left, prefix + (isLeft ? "    " : "│   "), true);
    }
}


    // вспомогательная функция: удаление всех узлов дерева
    void deleteTree(Node* node)
    {
        if (node != nullptr) {
            deleteTree(node->left);   // удаляем левое поддерево
            deleteTree(node->right);  // удаляем правое поддерево
            delete node;              // удаляем текущий узел
        }
    }

public:
    // конструктор: инициализация дерева
    RedBlackTree()
        : root(nullptr)
    {
    }

    // деструктор: удаление дерева
    ~RedBlackTree() { deleteTree(root); }

    // публичная функция: вставка значения в дерево
    void insert(T key)
    {
        Node* node = new Node(key);  // создаем новый узел
        Node* parent = nullptr;
        Node* current = root;
        while (current != nullptr) {  // поиск места для вставки
            parent = current;
            if (node->data < current->data)
                current = current->left;
            else
                current = current->right;
        }
        node->parent = parent;  // устанавливаем родителя
        if (parent == nullptr)
            root = node;  // дерево было пустым
        else if (node->data < parent->data)
            parent->left = node;  // вставляем как левого потомка
        else
            parent->right = node;  // вставляем как правого потомка
        fixInsert(node);  // исправляем возможные нарушения
    }

    // публичная функция: удаление значения из дерева
    void remove(T key)
    {
        Node* node = root;
        Node* z = nullptr;
        Node* x = nullptr;
        Node* y = nullptr;
        while (node != nullptr) {  // поиск удаляемого узла
            if (node->data == key) {
                z = node;
            }

            if (node->data <= key) {
                node = node->right;
            }
            else {
                node = node->left;
            }
        }

        if (z == nullptr) {  // если узел не найден
            cout << "Key not found in the tree" << endl;
            return;
        }

        y = z;
        Color yOriginalColor = y->color;
        if (z->left == nullptr) {  // если нет левого потомка
            x = z->right;
            transplant(root, z, z->right);
        }
        else if (z->right == nullptr) {  // если нет правого потомка
            x = z->left;
            transplant(root, z, z->left);
        }
        else {  // если есть оба потомка
            y = minValueNode(z->right);  // находим минимальный в правом поддереве
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y;
            }
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
        delete z;  // удаляем узел
        if (yOriginalColor == BLACK) {  // если удалили черный узел
            fixDelete(x);  // исправляем нарушения
        }
    }

    // публичная функция: вывод дерева
    void printTree()
{
    if (root == nullptr)
        cout << "Tree is empty." << endl;
    else {
        printHelper(root, "", false);
    }
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
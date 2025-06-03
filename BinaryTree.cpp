#include <iostream>       
#include <string>         
using namespace std;      

// структура для реализации узла дерева
struct tree
{
	int inf;              // значение узла
	tree* left;           // указатель на левого потомка
	tree* right;          // указатель на правого потомка
	tree* parent;         // указатель на родительский узел
};

// функция создания нового узла дерева
tree* node(int x)
{
	tree* node = new tree;     // выделение памяти под новый узел
	node->inf = x;             // присвоение значения
	node->left = node->right = NULL;   // инициализация потомков как пустые
	node->parent = NULL;      // родитель изначально отсутствует

	return node;              // возврат указателя на созданный узел
}

// функция вставки узла в дерево
void insert(tree *&tr, int x)
{
	tree* n = node(x);        // создание нового узла
	if (!tr) tr = n;          // если дерево пустое, новый узел становится корнем
	else
	{
		tree *y = tr;         // временный указатель для прохода по дереву
		while(y)
		{
			if (n->inf > y->inf)              // если значение больше — идем вправо
				if (y->right)                 // если правый потомок есть — двигаемся дальше
					y = y->right;
				else                          // если правого потомка нет — вставляем здесь
				{
					n->parent = y;
					y->right = n;
					break;
				}
			else if (n->inf < y->inf)         // если значение меньше — идем влево
				if (y->left)                  // если левый потомок есть — двигаемся дальше
					y = y->left;
				else                          // если левого потомка нет — вставляем здесь
				{
					n->parent = y;
					y->left = n;
					break;
				}
		}
	}
}

// симметричный (in-order) обход дерева: левый — корень — правый
void symmetric(tree *tr)
{
	if(tr)
	{
		symmetric(tr->left);      // обход левого поддерева
		cout << tr->inf << " ";   // печать текущего узла
		symmetric(tr->right);     // обход правого поддерева
	}
}

// прямой (pre-order) обход: корень — левый — правый
void forward(tree *tr)
{
	if(tr)
	{
		cout << tr->inf << " ";   // печать текущего узла
		forward(tr->left);        // обход левого поддерева
		forward(tr->right);       // обход правого поддерева
	}
}

// обратный (post-order) обход: левый — правый — корень
void backward(tree *tr)
{
	if(tr)
	{
		backward(tr->left);       // обход левого поддерева
		backward(tr->right);      // обход правого поддерева
		cout << tr->inf << " ";   // печать текущего узла
	}
}

// рекурсивный поиск узла по значению
tree *find(tree *tr, int x)
{
	if (!tr || x == tr->inf)     // если дерево пусто или нашли нужное значение
		return tr;

	if (x < tr->inf)             // ищем в левом поддереве
		return find(tr->left, x);
	else                         // ищем в правом поддереве
		return find(tr->right, x);
}

// поиск узла с минимальным значением (самый левый)
tree *Min(tree *tr)
{
	tree* curr = tr;
	while (curr && curr->left != nullptr) // пока есть левый потомок — идем влево
		curr = curr->left;

	return curr;  // возвращаем минимальный узел
}

// поиск узла с максимальным значением (самый правый)
tree *Max(tree *tr)
{
	tree* curr = tr;
	while (curr && curr->right != nullptr) // пока есть правый потомок — идем вправо
		curr = curr->right;

	return curr;  // возвращаем максимальный узел
}

// поиск следующего (по значению) узла
tree *Next(tree* tr, int x)
{
	tree* n = find(tr, x);          // находим узел
	if (n->right)                   // если есть правое поддерево
		return Min(n->right);       // следующий — минимум в правом поддереве

	tree* y = n->parent;
	while (y && n == y->right)      // поднимаемся вверх пока узел — правый потомок
	{
		n = y;
		y = y->parent;
	}

	return y;                       // возвращаем родителя, который больше x
}

// поиск предыдущего узла (по значению)
tree *Prev(tree* tr, int x)
{
	tree *n = find(tr, x);          // находим узел
	if (n->left)                    // если есть левое поддерево
		return Max(n->left);        // предыдущий — максимум в левом поддереве

	tree* y = n->parent;
	while (y && n == y->left)       // поднимаемся вверх пока узел — левый потомок
	{
		n = y;
		y =y->parent;
	}

	return y;                       // возвращаем родителя, который меньше x
}

// удаление узла по значению
tree* Delete(tree* tr, int x)
{
	if (tr == nullptr) return tr;   // если дерево пустое — ничего не делаем

	if (x < tr->inf)                // если x меньше — идем влево
		tr->left = Delete(tr->left, x);

	else if (x > tr->inf)           // если x больше — идем вправо
		tr->right = Delete(tr->right, x);

	else                            // найден узел для удаления
	{
		if (tr->left == nullptr)    // нет левого поддерева
		{
			tree* temp = tr->right;
			delete tr;
			return temp;
		}
		else if (tr->right == nullptr) // нет правого поддерева
		{
			tree* temp = tr->left;
			delete tr;
			return temp;
		}

		tree* temp = Min(tr->right);     // находим минимальный узел в правом поддереве
		tr->inf = temp->inf;             // копируем его значение
		tr->right = Delete(tr->right, temp->inf); // удаляем дубликат
	}

	return tr;                     // возвращаем корень
}

// основная функция
int main()
{
	int n, x;
	cout << "Количество узлов: n = "; cin >> n;    // ввод количества узлов
	tree *tr = NULL;             // указатель на корень дерева

	// ввод значений и построение дерева
	for (int i = 0; i < n; ++i)
	{
		cout << i << ": ";
		cin >> x;
		if (find(tr, x))         // проверка на уникальность
		{
			cout << "Недопустимое значение" << endl;
			--i;                 // повтор текущей итерации
		}
		else
			insert(tr, x);      // вставка значения в дерево
	}

	cout << "/////////////////////////////////////////////////////////////////" << endl;

	// вывод обходов
	cout << "Симметричный обход: "; symmetric(tr); cout << endl;
	cout << "Прямой обход: "; forward(tr); cout << endl;
	cout << "Обратный обход: "; backward(tr); cout << endl;

	cout << endl;

	cout << "min = " << Min(tr)->inf << endl;  // минимальный элемент
	cout << "max = " << Max(tr)->inf << endl;  // максимальный элемент

	// удаление узла
	cout << "Введите узел, который желаете удалить: "; cin >> x;
	tree* node_to_delete = find(tr, x);

	if(!node_to_delete)
		cout << "Такого узла нет" << endl;
	else
	{
		tr = Delete(tr, x);  // удаление узла

		cout << endl << "Результат: " << endl;
		cout << "Симметричный обход: "; symmetric(tr); cout << endl;
		cout << "Прямой обход: "; forward(tr); cout << endl;
		cout << "Обратный обход: "; backward(tr); cout << endl;
	}

	// вставка нового узла
	cout << "Введите узел, который желаете вставить: "; cin >> x;
	if (!find(tr, x))
		insert(tr, x);
	cout << endl << "Результат: " << endl;

	// обходы после вставки
	cout << "Симметричный обход: "; symmetric(tr); cout << endl;
	cout << "Прямой обход: "; forward(tr); cout << endl;
	cout << "Обратный обход: "; backward(tr); cout << endl;

	return 0;
}
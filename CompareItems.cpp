#include <iostream>
#include <chrono> // для измерения времени выполнения

using namespace std;

// быстрая сортировка (quicksort)
void quicksort(int arr[], int l, int h){
    int lowInd = l; // начальный индекс
    int highInd = h; // конечный индекс

    // выбираем средний элемент как опорный
    int midInd = arr[(lowInd + highInd)/2];

    // разделение массива по принципу хоара
    while (lowInd <= highInd){
        while (arr[lowInd] < midInd){ // ищем элемент слева, который >= опорного
            lowInd++;
        }
        while (arr[highInd] > midInd){ // ищем элемент справа, который <= опорного
            highInd--;
        }
        if (lowInd <= highInd){ // если указатели не пересеклись
            swap(arr[lowInd], arr[highInd]); // меняем местами элементы
            lowInd++; // сдвигаем левый указатель
            highInd--; // сдвигаем правый указатель
        }
    }

    // рекурсивно сортируем подмассивы
    if (highInd > l){
        quicksort(arr, l, highInd); // сортируем левую часть
    }
    if (lowInd < h){
        quicksort(arr, lowInd, h); // сортируем правую часть
    }
}

// слияние двух отсортированных подмассивов
void mergeProcess(int arr[], int l, int m, int h)
{
    int size_1 = m - l + 1; // размер первого подмассива
    int size_2 = h - m; // размер второго подмассива

    // временные массивы
    int *first = new int[size_1], *second = new int[size_2];

    // копируем данные в подмассивы
    for (int i = 0; i < size_1; ++i)
        first[i] = arr[l + i]; // копируем первую часть в first
    for (int i = 0; i < size_2; ++i)
        second[i] = arr[m + 1 + i]; // копируем вторую часть в second

    // сливаем обратно в arr[l..h]
    int i = 0, j = 0, k = l; // индексы для first, second и arr
    while (i < size_1 && j < size_2)
    {
        if (first[i] <= second[j]) // выбираем наименьший элемент
        {
            arr[k] = first[i]; // вставляем элемент из first
            i++; // двигаем индекс первого массива
        }
        else
        {
            arr[k] = second[j]; // вставляем элемент из second
            j++; // двигаем индекс второго массива
        }
        k++; // двигаем основной индекс
    }

    // копируем оставшиеся элементы
    while (i < size_1)
    {
        arr[k] = first[i]; // вставляем оставшиеся из first
        i++;
        k++;
    }
    while (j < size_2)
    {
        arr[k] = second[j]; // вставляем оставшиеся из second
        j++;
        k++;
    }

    // освобождаем память
    delete[] first; // удаляем временный массив first
    delete[] second; // удаляем временный массив second
}

// рекурсивная сортировка слиянием (merge sort)
void mergeSort(int arr[], int l, int h)
{
    if (l < h)
    {
        int m = l + (h - l) / 2;

        // сортируем левую и правую части
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, h);

        // сливаем отсортированные части
        mergeProcess(arr, l, m, h);
    }
}

// построение кучи (heapify) для пирамидальной сортировки
void heapify(int arr[], int size, int i)
{
    int maxx = i;           // индекс наибольшего элемента
    int l = 2*i + 1;        // левый потомок
    int r = 2*i + 2;        // правый потомок

    // ищем наибольший среди родителя и потомков
    if (l < size && arr[l] > arr[maxx])
        maxx = l;
    if (r < size && arr[r] > arr[maxx])
        maxx = r;

    // если наибольший не родитель — меняем местами и продолжаем
    if (maxx != i)
    {
        swap(arr[i], arr[maxx]);
        heapify(arr, size, maxx);
    }
}

// пирамидальная сортировка (heap sort)
void heapSort(int arr[], int size)
{
    // построение max-heap
    for (int i = size/2 - 1; i >= 0; --i)
        heapify(arr, size, i);

    // один за другим извлекаем элементы из кучи
    for (int i = size - 1; i >= 0; --i)
    {
        swap(arr[0], arr[i]);      // перемещаем текущий максимум в конец
        heapify(arr, i, 0);        // восстанавливаем свойство кучи
    }
}

int main(){
    int method;
    // выбор алгоритма сортировки
    cout << "Выберите один из вариантов:\n1. | Быстрая сортировка.\n2. | Сортировка слиянием\n3. | Пирамидальная сортировка\n> ";
    cin >> method;

    int n, size;
    // выбор размера массива
    cout << "1. | Размерность: 50.000\n2. | Размерность: 100.000\n3. | Размерность: 500.000\n4. | Размерность: 1.000.000\n> ";
    cin >> n;

    // присваиваем реальный размер массива
    switch (n)
    {
    case 1:
        size = 50000;
        break;
    case 2:
        size = 100000;
        break;
    case 3:
        size = 500000;
        break;
    case 4:
        size = 1000000;
        break;
    default:
        cout << "Выберите конкретный вариант!\n";
        return 1;
    }

    // выделение памяти под массив
    int *arr = new int[size];

    // заполнение массива случайными числами от 0 до 99
    for (int i = 0; i < size; i++){
        arr[i] = rand() % 100;
    }

    // засекаем время перед сортировкой
    auto start = chrono::high_resolution_clock::now();

    // выполняем выбранную сортировку
    switch (method)
    {
    case 1:
        quicksort(arr, 0, size - 1);
        break;
    case 2:
        mergeSort(arr, 0, size - 1);
        break;
    case 3:
        heapSort(arr, size);
        break;
    default:
        cout << "Выберите конкретный вариант!\n";
        delete[] arr;
        return 1;

    }

    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }

    // засекаем время после сортировки
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> dur = end - start;

    // вывод времени сортировки
    cout << "Время исполнения: " << dur.count() << " секунд\n";

    // освобождаем память
    delete[] arr;

    return 0;
}

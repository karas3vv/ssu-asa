#include <iostream>
#include <vector>
#include <algorithm>
#include <climits> // для INT_MIN, INT_MAX

using namespace std;

int main() {
    int n;
    cout << "Введите N: (задание 1): ";
    cin >> n; 

    vector<int> arr(n); 
    int max_value = INT_MIN; // инициализируем минимально возможным значением
    int min_value = INT_MAX; // инициализируем максимально возможным значением

    // вводим элементы и находим min и max значения
    for (int i = 0; i < n; i++) {
        cout << i + 1 << ": ";
        cin >> arr[i];
        max_value = max(max_value, arr[i]); // обновляем максимум
        min_value = min(min_value, arr[i]); // обновляем минимум
    }

    // создаем массив для подсчета, размером от min до max значения
    vector<int> count_arr(max_value - min_value + 1, 0);

    // подсчитываем количество каждого элемента
    for (int i : arr) {
        count_arr[i - min_value]++; // увеличиваем счетчик для текущего числа
    }

    // восстанавливаем отсортированный массив из счетчиков
    int j = 0; // индекс для заполнения исходного массива
    for (int i = 0; i < count_arr.size(); i++) {
        while (count_arr[i] > 0) {
            arr[j++] = i + min_value; // записываем число обратно в массив
            count_arr[i]--; // уменьшаем счетчик
        }
    }

    cout << "Отсортированный массив методом подсчёта:\n";
    for (int i : arr) {
        cout << i << ' ';
    }

    return 0;
}
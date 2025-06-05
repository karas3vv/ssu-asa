#include <iostream>
#include <vector>
#include <string>

using namespace std;

// функция для создания таблицы "плохого символа"
vector<int> buildBadCharTable(const string& pattern) {
    const int ALPHABET_SIZE = 256;  // размер алфавита (ASCII)
    vector<int> badCharTable(ALPHABET_SIZE, -1);  // создаём таблицу и заполняем -1

    for (int i = 0; i < pattern.size(); i++) {  // проходим по всем символам шаблона
        badCharTable[(unsigned char)pattern[i]] = i;  // запоминаем последнее вхождение символа
    }

    return badCharTable;
}

// алгоритм Бойера — Мура (только эвристика плохого символа)
vector<int> boyerMooreSearch(const string& text, const string& pattern) {
    vector<int> result;  // вектор для хранения позиций совпадений
    int n = text.size();  // длина текста
    int m = pattern.size();  // длина шаблона

    if (m == 0 || n < m) return result;  // проверка на пустой шаблон или короткий текст

    vector<int> badChar = buildBadCharTable(pattern);  // строим таблицу плохих символов

    int shift = 0;  // начальное смещение

    while (shift <= (n - m)) {  // пока шаблон помещается в оставшийся текст
        int j = m - 1;  // начинаем сравнение с конца шаблона

        // сравнение с конца шаблона
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;  // двигаемся к началу шаблона при совпадении
        }

        // если шаблон найден
        if (j < 0) {
            result.push_back(shift);  // сохраняем позицию совпадения
            shift += (shift + m < n) ? m - badChar[(unsigned char)text[shift + m]] : 1;  // вычисляем смещение
        } else {
            // смещение по эвристике плохого символа
            shift += max(1, j - badChar[(unsigned char)text[shift + j]]);
        }
    }

    return result; 
}

int main() {
    string text, pattern;

    cout << "Введите текст для поиска: ";
    getline(cin, text);

    cout << "Введите шаблон для поиска: ";
    getline(cin, pattern); 

    vector<int> matches = boyerMooreSearch(text, pattern);  // выполняем поиск

    if (matches.empty()) {  // если совпадений нет
        cout << "Шаблон не найден в тексте." << endl;
    } else {  // если совпадения есть
        cout << "Найденные позиции: ";  
        for (int pos : matches) {  
            cout << pos << " ";  
        }
        cout << endl;
    }

    return 0; 
}
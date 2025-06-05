#include <iostream>
#include <vector>
#include <string>

using namespace std;

// функция для вычисления префикс-функции строки
vector<int> prefix_function(const string &s)
{
    int n = s.size();  // получаем длину строки
    vector<int> p(n, 0);  // создаем массив для хранения значений префикс-функции, заполняем нулями

    for (int i = 1; i < n; i++)  // начинаем с 1, так как для первого символа префикс-функция всегда 0
    {
        int j = p[i - 1];  // берем значение префикс-функции для предыдущего символа

        while (j > 0 && s[i] != s[j])  // пока j > 0 и символы не совпадают
            j = p[j - 1];  // уменьшаем j, используя уже вычисленные значения префикс-функции

        if (s[i] == s[j])  // если символы совпали
            p[i] = j + 1;  // увеличиваем значение префикс-функции
    }

    return p;  // возвращаем массив значений префикс-функции
}

// функция для поиска подстроки в тексте с использованием алгоритма Кнута-Морриса-Пратта
vector<int> kmp_search(const string &text, const string &pattern)
{
    string concatenated = pattern + "@" + text;  // объединяем подстроку, разделитель и текст
    vector<int> p = prefix_function(concatenated);  // вычисляем префикс-функцию для объединенной строки
    vector<int> matches;  // создаем массив для хранения индексов совпадений

    for (int i = pattern.size() + 1; i < p.size(); i++)
    { 
        if (p[i] == pattern.size())  // если значение префикс-функции равно длине подстроки
        {                                              
            matches.push_back(i - 2 * pattern.size());  // добавляем индекс начала подстроки в текст
        }
    }

    return matches;  // возвращаем массив индексов совпадений
}

int main()
{
    string text, pattern;

    cout << "введите исходную строку: ";
    getline(cin, text);
    cout << "введите искомую подстроку: ";
    getline(cin, pattern);

    vector<int> matches = kmp_search(text, pattern);  // ищем все вхождения подстроки
    
    if (matches.size() == 0)
        cout << "не удалось найти совпадения";
    else 
    {
        cout << "результат (индекс(-ы), с которого начинается подстрока): ";
        for (int idx : matches)
        {
            cout << idx << " ";
        }
        cout << endl;
    }

    return 0;
}
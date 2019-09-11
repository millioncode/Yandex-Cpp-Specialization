#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

void LeftStrip(string_view& sv) {
    // удаляем лишние пробелы слева
    while (!sv.empty() && isspace(sv[0])) {
        sv.remove_prefix(1);
    }
}
string_view ReadToken(string_view& sv) {
    LeftStrip(sv);
    // ищем пробел
    // т.е. позицию конца слова - мы ведь сначала удалили пробелы
    auto pos = sv.find(' ');
    // создаем слово
    auto word = sv.substr(0, pos);
    // удаляем слово и
    sv.remove_prefix(pos != sv.npos? pos:sv.size() );
    return word;
}

vector<string_view> SplitIntoWords(string_view line) {
    vector<string_view> value;
    /* читаем строку(содержимое файла
     * за одну итерацию читаем слово
     * добавляем слово в вектор
     * */
    for( string_view word = ReadToken(line); !word.empty() ; word = ReadToken(line)) {
        value.push_back(word);
    }
    return value;
}

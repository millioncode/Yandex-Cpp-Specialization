#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // Напишите реализацию функции,
    // не копируя сортируемые элементы
    if (range_end - range_begin < 2 ) return;

    vector <typename RandomIt::value_type> values (make_move_iterator(range_begin),
                         make_move_iterator(range_end));
    size_t size = values.size();
    auto one = range_begin;
    auto two = range_begin + size/3;
    auto three = range_end - size/3;
    auto four = range_end;

    MergeSort(one, two);
    MergeSort(two, three);
    MergeSort(three, four);

    vector <typename RandomIt::value_type> temp;
    merge(make_move_iterator(one),
          make_move_iterator(two),
          make_move_iterator(two),
          make_move_iterator(three),
          back_inserter(temp));

    merge(make_move_iterator(temp.begin()),
          make_move_iterator(temp.end()),
          make_move_iterator(three),
          make_move_iterator(four),
          range_begin );
}

void TestIntVector() {
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    for(auto& a : numbers) {
        cout << a << " ";
    }
    cout << endl;
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}

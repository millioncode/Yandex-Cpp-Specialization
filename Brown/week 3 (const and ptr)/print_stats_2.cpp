#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>

#include <ostream>
#include <istream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <string_view>
#include <iterator>
#include <functional>
using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

private:
  Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
  return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
  string name;
  int age, income;
  bool is_male;
};

bool operator==(const Person& left, const Person& right) {
        return left==right;
}

struct Hasher {
    std::hash<int> ihash;
    std::hash<std::string> shash;
    std::hash<bool> bhash;
    const std::size_t k = 113;
    std::size_t operator()(const Person& p) const {
        return (shash(p.name)*k*k*k + ihash(p.age)*k*k + ihash(p.income)*k + bhash(p.is_male) );
    }
};

int see_ages(std::vector<Person>& v, int age) {
    IteratorRange range{
        begin(v),
        partition(begin(v), end(v), [age](const Person& p) {
            return p.age >= age;
        })
    };
    return distance(range.begin(), range.end());
}
int max_incomes(const std::vector<Person>& v, int M)  {
    std::vector<int> incomes(M,0);
    int i=0;
    for(const auto it : Head(v, M)) {
        incomes[i++] = it.income;
    }
    auto it = min_element(incomes.begin(), incomes.end());
    for(i=M; i<v.size(); ++i) {
        if (v[i].income > *it) {
            *it = v[i].income;
            it = min_element(incomes.begin(), incomes.end());
        }
    }
    int result = 0;
    for(const auto& a:Head(incomes, M)) {
        result += a;
    }
    return result;
}
vector<Person> ReadPeople(istream& input) {
  int count ;
  input >> count;
  vector<Person> result(count);

  for (Person& p : result) {
    char gender;
    cin >> p.name >> p.age >> p.income >> gender;
    p.is_male = gender == 'M';
  }
  return result;
}

int main() {
    vector<Person> people = ReadPeople(cin);
  for (string command; cin >> command; ) {
    if (command == "AGE") {
      int adult_age;
      cin >> adult_age;
      cout << "There are " << see_ages(people, adult_age)
            << " adult people for maturity age " << adult_age << '\n';
    } else if (command == "WEALTHY") {
      int count;
      cin >> count;
      cout << "Top-" << count << " people have total income " << max_incomes(people, count) << '\n';
    } else if (command == "POPULAR_NAME") {
      char gender;
      cin >> gender;
        IteratorRange range{
                begin(people),
                partition(begin(people), end(people), [gender](const Person& p) {
                    return p.is_male == (gender == 'M');
                })
        };
        if (range.begin() == range.end()) {
            cout << "No people of gender " << gender << '\n';
        } else {
            sort(range.begin(), range.end(), [](const Person& lhs, const Person& rhs) {
                return lhs.name < rhs.name;
            });
            const string* most_popular_name = &range.begin()->name;
            int count = 1;
            for (auto i = range.begin(); i != range.end(); ) {
                auto same_name_end = find_if_not(i, range.end(), [i](const Person& p) {
                    return p.name == i->name;
                });
                auto cur_name_count = std::distance(i, same_name_end);
                if (cur_name_count > count) {
                    count = cur_name_count;
                    most_popular_name = &i->name;
                }
                i = same_name_end;
            }
            cout << "Most popular name among people of gender " << gender << " is "
                 << *most_popular_name << '\n';
        }
    }
  }
}
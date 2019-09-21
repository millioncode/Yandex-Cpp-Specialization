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

class Statics {
public:
    Statics(const std::vector<Person>& persons) {
        for(const auto& p: persons) {
            auto [ptr, succes] = __persons.insert(p);
            if (succes) {
                ages_persons.insert( ptr->age );
                incomes.insert(ptr->income);
                if (ptr->is_male) {
                    M_names[ptr->name]++;
                }
                else {
                    W_names[ptr->name]++;
                }
            }
        }

    }
    int see_ages(int age) const {
        int result = 0;
        auto it = lower_bound(ages_persons.begin(), ages_persons.end(), age);
        return std::distance(it, ages_persons.end());
    }
    int max_incomes(int M) const {
        int result = 0;
        for(auto it=incomes.begin(); it!=incomes.end() && M; ++it, M--) {
            result += *it;
        }
        return result;
    }
    std::string top_names(bool man) const {
        int m = 0;
        std::string name;
        if (man) {
            for (const auto&[k, value]: M_names) {
                if (value > m) {
                    name = k;
                    m = value;
                }
            }
        }
        else {
            for (const auto&[k, value]: W_names) {
                if (value > m) {
                    name = k;
                    m = value;
                }
            }
        }
        return name;
    }
private:
    std::unordered_set<Person, Hasher> __persons;
    std::multiset<int> ages_persons;
    std::multiset<int, std::greater<int>> incomes;
    struct name_count {
        std::string_view name;
        int count;
    };
    std::map<const std::string_view, int> M_names;
    std::map<const std::string_view, int> W_names;
};
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
    Statics stat(ReadPeople(cin));
  for (string command; cin >> command; ) {
    if (command == "AGE") {
      int adult_age;
      cin >> adult_age;

        cout << "There are " << stat.see_ages(adult_age)
             << " adult people for maturity age " << adult_age << '\n';
    } else if (command == "WEALTHY") {
      int count;
      cin >> count;

      cout << "Top-" << count << " people have total income " << stat.max_incomes(count) << '\n';
    } else if (command == "POPULAR_NAME") {
      char gender;
      cin >> gender;
        std::string most_popular_name = stat.top_names(gender=='M');
        if (!most_popular_name.empty()) {
            cout << "Most popular name among people of gender " << gender << " is "
                 << most_popular_name << '\n';
        }
        else {
            cout << "No people of gender " << gender << '\n';
        }
    }
  }
}

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
        return (shash(p.name)*k + ihash(p.age)*k + ihash(p.income)*k + bhash(p.is_male) );
    }
};

class Statics {
public:
    Statics(const std::vector<Person>& persons) {
        std::map<const std::string, int> M_names;
        std::map<const std::string, int> W_names;
        for(const auto& p: persons) {
            auto [ptr, succes] = __persons.insert(p);
            if (succes) {
                ages_persons.insert( ptr->age );
                incomes.insert(ptr->income);
                if (ptr->is_male) M_names[ptr->name]++;
                else W_names[ptr->name]++;
            }
        }
        int m = 0;
        for(const auto& [k, value]: M_names) {
            if (value>m) {
                man_name = k;
                m = value;
            }
        }
        m = 0;
        for(const auto& [k, value]: M_names) {
            if (value>m) {
                man_name = k;
                m = value;
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
        return (man)? man_name : woman_name;
    }
private:
    std::unordered_set<Person, Hasher> __persons;
    std::multiset<int> ages_persons;
    std::multiset<int, std::greater<int>> incomes;
    std::string man_name;
    std::string woman_name;
};
vector<Person> ReadPeople(istream& input) {
    // читаем кол-во людей
  int count=11;
  //input >> count;
    std::vector<std::string> s = {//"11\n",
                                  "Ivan 25 1000 M\n",
                                  "Olga 30 623 W\n",
                                  "Sergey 24 825 M\n",
                                  "Maria 42 1254 W\n",
                                  "Mikhail 15 215 M\n",
                                  "Oleg 18 230 M\n",
                                  "Denis 53 8965 M\n",
                                  "Maxim 37 9050 M\n",
                                  "Ivan 47 19050 M\n",
                                  "Ivan 17 50 M\n",
                                  "Olga 23 550 W\n",
                                  };
    std::stringstream os;
    for(const auto& a:s){
        os << a;
    }
  vector<Person> result(count);
    // читаем данные по людям
  for (Person& p : result) {
    char gender;
    os >> p.name >> p.age >> p.income >> gender;
    p.is_male = gender == 'M';
  }

  return result;
}

int main() {
   /* std::stringstream ss ;
    std::vector<std::string> s ={
                "AGE 18\n",
                "AGE 25\n",
                "WEALTHY 5\n",
                "POPULAR_NAME M"};
    for(const auto& a:s){
        ss << a;
    }*/

  vector<Person> people = ReadPeople(cin);
    Statics stat(people);
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

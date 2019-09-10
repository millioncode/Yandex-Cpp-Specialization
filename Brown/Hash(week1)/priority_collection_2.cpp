#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
      using Id = int /* тип, используемый для идентификаторов */;
      // Добавить объект с нулевым приоритетом
      // с помощью перемещения и вернуть его идентификатор
      Id Add(T object) {
            const Id new_id = __objects.size();
            __objects.push_back({std::move(object)});
            sort_objects.insert(std::make_pair(0, new_id));
            return new_id;
      }
      // Добавить все элементы диапазона [range_begin, range_end)
      // с помощью перемещения, записав выданные им идентификаторы
      // в диапазон [ids_begin, ...)
      template <typename ObjInputIt, typename IdOutputIt>
      void Add(ObjInputIt range_begin, ObjInputIt range_end,
               IdOutputIt ids_begin) {
            while (range_begin != range_end) {
                  *ids_begin++ = Add(std::move(*range_begin++));
            }
      }

      // Определить, принадлежит ли идентификатор какому-либо
      // хранящемуся в контейнере объекту
      bool IsValid(Id id) const {
            return (id>=0 && id<__objects.size() && __objects[id].rate>=0) ? true : false;
      }

      // Получить объект по идентификатору
      const T& Get(Id id) const {
            return __objects[id].object;

      }

      // Увеличить приоритет объекта на 1
      void Promote(Id id) {
            auto & _obj =  __objects[id];
            const int old_rate = _obj.rate;
            const int new_rate = ++_obj.rate;
            sort_objects.erase({old_rate, id});
            sort_objects.insert({new_rate, id});
      }
      // Получить объект с максимальным приоритетом и его приоритет
      pair<const T&, int> GetMax() const {
            const auto& obj = __objects[std::prev(sort_objects.end())->second];
            return {obj.object, obj.rate};
      }

      // Аналогично GetMax, но удаляет элемент из контейнера
      pair<T, int> PopMax() {
            const auto it = prev(sort_objects.end());
            auto& obj = __objects[it->second];
            sort_objects.erase(it);
            const int rate = obj.rate;
            obj.rate = -1;
            return {std::move(obj.object), rate};
      }

private:
      // Приватные поля и методы
      struct Data{
            T object;
            int rate = 0;
      };
      std::vector<Data> __objects;
      std::set<std::pair<int, Id>> sort_objects;
};


class StringNonCopyable : public string {
public:
        using string::string;  // Позволяет использовать конструкторы строки
        StringNonCopyable(const StringNonCopyable&) = delete;
        StringNonCopyable(StringNonCopyable&&) = default;
        StringNonCopyable& operator=(const StringNonCopyable&) = delete;
        StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
        PriorityCollection<StringNonCopyable> strings;
        const auto white_id = strings.Add("white");
        const auto yellow_id = strings.Add("yellow");
        const auto red_id = strings.Add("red");

        strings.Promote(yellow_id);
        for (int i = 0; i < 2; ++i) {
              strings.Promote(red_id);
        }
       strings.Promote(yellow_id);
      {
                const auto item = strings.PopMax();
                ASSERT_EQUAL(item.first, "red");
                ASSERT_EQUAL(item.second, 2);
        }
        {
                const auto item = strings.PopMax();
                ASSERT_EQUAL(item.first, "yellow");
                ASSERT_EQUAL(item.second, 2);
        }
        {
                const auto item = strings.PopMax();
                ASSERT_EQUAL(item.first, "white");
                ASSERT_EQUAL(item.second, 0);
        }
}

int main() {
        TestRunner tr;
        RUN_TEST(tr, TestNoCopy);
        return 0;
}

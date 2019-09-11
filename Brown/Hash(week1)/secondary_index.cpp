#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
        string id;
        string title;
        string user;
        int timestamp;
        int karma;
        /*std::size_t operator==(const Record& r) const {
              return this->id==r.id
                          && this->title==r.title
                          && this->user==r.user
                          && this->timestamp==r.timestamp
                          && this->karma==r.karma;
        }*/
};
struct Hasher {
      std::hash<int> ihash;
      std::hash<std::string> shash;
      const std::size_t k = 113;
      std::size_t operator()(const Record& r) const {
            return (shash(r.id)*k + shash(r.title)*k + shash(r.user)*k + ihash(r.timestamp)*k + ihash(r.karma)*k );
      }
};

struct Data {
      Record value; // непосредственно само значение
      std::multimap<int, const Record*>::iterator it_karma; // по карме будем знать тело
      std::multimap<std::string, const Record*>::iterator it_user;
      std::multimap<int, const Record*>::iterator it_time;
};
// Реализуйте этот класс
class Database {
public:
      bool Put(const Record& record) {
            auto [it, succes] = general.insert( {record.id, Data{record, {} }} );
            if (!succes) {
                  return false;
            }
            else {
                  auto& data = it->second;
                  const Record* ptr_record = &data.value;
                  data.it_karma = karmas.insert( {record.karma, ptr_record} );
                  data.it_user = users.insert( {record.user, ptr_record});
                  data.it_time = times.insert( {record.timestamp, ptr_record} );
                  return true;
            }
      }
      const Record* GetById(const string& id) const {
            auto it = general.find(id);
            return (it==general.end()) ? nullptr : &it->second.value;
      }
      bool Erase(const string& id) {
            auto it = general.find(id);
            if (it==general.end()) return false;
            const auto& data = it->second;
            karmas.erase(data.it_karma);
            users.erase(data.it_user);
            times.erase(data.it_time);
            general.erase(it);
            return true;
      }

      template <typename Callback>
      void RangeByTimestamp(int low, int high, Callback callback) const {
            if (low>high) {
                  return ;
            }
            auto it_start = times.lower_bound(low);
            auto it_end = times.upper_bound(high);
            for(auto it=it_start; it!=it_end; ++it) {
                  if (!callback(*it->second)) {
                        break;
                  }
            }
      }

      template <typename Callback>
      void RangeByKarma(int low, int high, Callback callback) const {
            if (low>high) {
                  return ;
            }
            auto it_start = karmas.lower_bound(low);
            auto it_end = karmas.upper_bound(high);
            for(auto it=it_start; it!=it_end; ++it) {
                  if (!callback(*it->second)) {
                        break;
                  }
            }
      }

      template <typename Callback>
      void AllByUser(const string& user, Callback callback) const {
              auto [it, end] = users.equal_range(user);
                      while (it != end && callback(*it->second)) {
                          ++it;
                      }
        }
private:
      template <typename T>
      using Index = std::multimap<T,  const Record*>;

      std::unordered_map<std::string, Data> general;

      Index<int> karmas;
      Index<std::string> users;
      Index<int> times;
};

void TestRangeBoundaries() {
        const int good_karma = 1000;
        const int bad_karma = -10;

        Database db;
        db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
        db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

        int count = 0;
        db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
                ++count;
                return true;
        });

        ASSERT_EQUAL(2, count);
}

void TestSameUser() {
        Database db;
        db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
        db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

        int count = 0;
        db.AllByUser("master", [&count](const Record&) {
                ++count;
                return true;
        });

        ASSERT_EQUAL(2, count);
}
void TestReplacement() {
        const string final_body = "Feeling sad";

        Database db;
        db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
        db.Erase("id");
        db.Put({"id", final_body, "not-master", 1536107260, -10});

        auto record = db.GetById("id");
        ASSERT(record != nullptr);
        ASSERT_EQUAL(final_body, record->title);
}

int main() {
        TestRunner tr;
        RUN_TEST(tr, TestRangeBoundaries);
        RUN_TEST(tr, TestSameUser);
        RUN_TEST(tr, TestReplacement);
        return 0;
}

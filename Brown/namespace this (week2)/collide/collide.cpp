#include "geo2d.h"
#include "game_object.h"

#include "test_runner.h"

#include <vector>
#include <memory>

using namespace std;

// Определите классы Unit, Building, Tower и Fence так, чтобы они наследовались от
// GameObject и реализовывали его интерфейс.
bool operator==(const geo2d::Point& a, const geo2d::Point& b) {
  return (a.x == b.x 
              && a.y == b.y) ;
}
bool operator==(const geo2d::Rectangle& a, const geo2d::Rectangle & b) {
  return (
      a.BottomLeft() == b.BottomLeft()
      && a.TopRight() == b.TopRight()
              ) ;
}

class Unit: public GameObject {
public:
  explicit Unit(geo2d::Point position): pos_unit(position) {}

  virtual bool Collide(const GameObject& that) const override {
    return that.CollideWith(*this);
  }
  
private:
  geo2d::Point pos_unit;
  virtual bool CollideWith(const Unit& that) const override {
      return (this->pos_unit == that.pos_unit) ;
  }
  virtual bool CollideWith(const Building& that) const override{
    return true;
  }
};

class Building: public GameObject {
public:
  explicit Building(geo2d::Rectangle geometry): pos_building(geometry) {}
  virtual bool Collide(const GameObject& that) const override {
    return that.CollideWith(*this);
  }
  virtual bool CollideWith(const Building& that) const override{
    return (this->pos_building==that.pos_building);
  }
  virtual bool CollideWith(const Unit& that) const override {
    return true;
  }
private:
  geo2d::Rectangle pos_building;
  /*int left_x, right_x;
  int bottom_y, top_y;*/
};
/*
class Tower {
public:
  explicit Tower(geo2d::Circle geometry);
};

class Fence {
public:
  explicit Fence(geo2d::Segment geometry);
};
*/
// Реализуйте функцию Collide из файла GameObject.h

bool Collide(const GameObject& first, const GameObject& second) {
  return first.Collide(second);
}

void TestAddingNewObjectOnMap() {
  // Юнит-тест моделирует ситуацию, когда на игровой карте уже есть какие-то объекты,
  // и мы хотим добавить на неё новый, например, построить новое здание или башню.
  // Мы можем его добавить, только если он не пересекается ни с одним из существующих.
  using namespace geo2d;
  geo2d::Point p1{1,1};
  geo2d::Point p2{3,3};

  Unit a(p1);
  Unit b(p2);
  if (Collide(a,b)) std::cout << "collide " << std::endl;
  else std::cout << "no collide " << std::endl;

  
  geo2d::Rectangle r(p1, p2);
  Building ab(Rectangle{Point{1,1}, Point{3,3}});
  Building bb(Rectangle{Point{1,1}, Point{3,3}});
  if (Collide(ab,bb)) std::cout << "collide " << std::endl;
  else std::cout << "no collide " << std::endl;
  geo2d::Circle c{p1, 2};
/*
  const vector<shared_ptr<GameObject>> game_map = {
    make_shared<Unit>(Point{3, 3}),
    make_shared<Unit>(Point{5, 5}),
    make_shared<Unit>(Point{3, 7}),
    make_shared<Fence>(Segment{{7, 3}, {9, 8}}),
    make_shared<Tower>(Circle{Point{9, 4}, 1}),
    make_shared<Tower>(Circle{Point{10, 7}, 1}),
    make_shared<Building>(Rectangle{{11, 4}, {14, 6}})
  };

  for (size_t i = 0; i < game_map.size(); ++i) {
    Assert(
      Collide(*game_map[i], *game_map[i]),
      "An object doesn't collide with itself: " + to_string(i)
    );

    for (size_t j = 0; j < i; ++j) {
      Assert(
        !Collide(*game_map[i], *game_map[j]),
        "Unexpected collision found " + to_string(i) + ' ' + to_string(j)
      );
    }
  }

  auto new_warehouse = make_shared<Building>(Rectangle{{4, 3}, {9, 6}});
  ASSERT(!Collide(*new_warehouse, *game_map[0]));
  ASSERT( Collide(*new_warehouse, *game_map[1]));
  ASSERT(!Collide(*new_warehouse, *game_map[2]));
  ASSERT( Collide(*new_warehouse, *game_map[3]));
  ASSERT( Collide(*new_warehouse, *game_map[4]));
  ASSERT(!Collide(*new_warehouse, *game_map[5]));
  ASSERT(!Collide(*new_warehouse, *game_map[6]));

  auto new_defense_tower = make_shared<Tower>(Circle{{8, 2}, 2});
  ASSERT(!Collide(*new_defense_tower, *game_map[0]));
  ASSERT(!Collide(*new_defense_tower, *game_map[1]));
  ASSERT(!Collide(*new_defense_tower, *game_map[2]));
  ASSERT( Collide(*new_defense_tower, *game_map[3]));
  ASSERT( Collide(*new_defense_tower, *game_map[4]));
  ASSERT(!Collide(*new_defense_tower, *game_map[5]));
  ASSERT(!Collide(*new_defense_tower, *game_map[6]));*/
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestAddingNewObjectOnMap);
  return 0;
}

#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>

using namespace std;

struct Person
{
  string name;
  int cash; // cashier will swear gamblers(cash == negative)
  size_t order_count;
  bool prefer_spicy;
  unordered_set<string> favorites; // help recommend food
  
  Person(string n, int c, size_t o, bool p, unordered_set<string>f)
      : name(n), cash(c), order_count(o), prefer_spicy(p), favorites(move(f)) // faborites succeed f's memory
									{}
};

struct Food
{
  std::string dish_name;
  bool flavor;// 1 is spicy, 0 is sweet
  int cooking_style; // 1 is grilled
		     // 2 is fried
		     // 3 is steamed

  int portion_size; // 1 is half-portion
		    // 2 is regular
		    // 3 is large
  
  Food(std::string d, bool f, int c, int p)
	: dish_name(d), flavor(f), cooking_style(c), portion_size(p) {}
};



#include<iostream>
#include<vector>
#include<string>

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

int main()
{
  std::vector<Food> order_list;

  order_list.emplace_back("pizza", true, 1, 2);
  order_list.emplace_back("a", true, 1, 2);
  order_list.emplace_back("b", true, 1, 2);
  order_list.emplace_back("c", true, 1, 2);

  for(const auto& food : order_list)
  {
    if(food.dish_name == "b")
    {
      std::cout << food.portion_size << '\n';
    }
  }
}


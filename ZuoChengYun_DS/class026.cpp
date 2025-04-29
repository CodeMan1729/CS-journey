#include<iostream>
#include<unordered_set>
#include<unordered_map>
#include<string>// alphabet order
#include<functional>
#include<algorithm>
#include<set>

using namespace std;

struct student
{
  int age;
  string name;

  student(int a, string n) : age(a), name(n) {}
 
  bool operator== (const student& other) const // avoid collision
  {
    return (age == other.age) && (name == other.name);
  }
};

struct employee
{
  int company;
  int age;

  employee(int c, int a) : company(c), age(a) {}
};

struct employee_comparator
{
  bool operator() (const employee& e1, const employee& e2) const // priotity of age
  {
    if(e1.age != e2.age)
    {
      return e1.age < e2.age;
    }
    else
    {
      if(e1.company != e2.company)
      {
	return e1.company < e2.company;
      }
    }

    return false;
  } 
};

namespace std
{
  template<>
  struct hash<student>
  {
    size_t operator() (const student& s) const // hash value is constant
    {
      return hash<int>()(s.age) ^ hash<string>()(s.name) << 1;
    }
  };
}


int main()
{
  string str1 = "you";
  string str2 = "you";  

  cout << (&str1==&str2) << '\n';
  cout << (str1==str2) << '\n';

  unordered_set<string> set1; // unordered_set 
  set1.insert(str1);  
  cout << (set1.find("you") != set1.end()) << '\n';
  cout << set1.size() << '\n';

  set1.insert(str2);
  cout << set1.size() << '\n';
  set1.erase(str1);
  cout << set1.size() << '\n';

  unordered_map<int, string> map; // unordered_map
  map[1] = "David";
  map[2] = "Jack";
  cout << (map.find(1) != map.end()) << '\n';
  cout << map[2] << '\n';

  student s1(17, "Alex");
  student s2(17, "Alex");//diff addresses
  student s3(19,"Bob");

  unordered_map<student, string> map1;
  map1[s1] = "This is Alex";
  cout << (map1.find(s2) != map1.end()) << '\n';
  map1[s2] = "this is other alex";
  cout << map1.size() << '\n';  
  cout << map1[s1] << '\n';
  map1[s3] = "This is Bob";

  for(const auto& pair : map1)
  {
    cout << pair.first.age <<" ";
  }
  cout << '\n';

  set<employee, employee_comparator> recruiter;

  recruiter.insert(employee(3,54));
  recruiter.insert(employee(5,34));
  recruiter.insert(employee(3,54));
  recruiter.insert(employee(2,25));
  recruiter.insert(employee(2,54));



  for(const auto& e : recruiter)
  {
    cout << e.company << ", " << e.age << " ";
  }
  
  string ss1 = "China"; // lexicographical order
  string ss2 = "american";

  cout << ss1.compare(ss2) << '\n';
  cout << ss2.compare(ss1) << '\n';

}

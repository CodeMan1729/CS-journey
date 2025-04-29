#include<iostream>
using namespace std;


class TreeNode
{
public:
  int val;
  TreeNode* left;
  TreeNode* right;

  TreeNode(int value) : val(value),left(nullptr),right(nullptr) {}
  TreeNode(int value,TreeNode* l,TreeNode* r) : val(value),left(l),right(r) {}
};

class Stack
{
private:
  static int stack[capacity];
  int top;
  size_t size;
  int capacity = 1000;

public:
  Stack()
  {
    top = -1;//没有值
    size = 0;
  }

  bool isEmpty()
  {
    return size == 0;
  }

  bool isFull()
  {
    return size == capacity;
  }

  void push(int x)
  {
    if(isFull()) {return;}
    top++;
    stack[top] = x;//[0,top] is valid range
    size++; 
  }

  void pop()
  {
    if(isEmpty())
    {
      return;
    }
    top--;
  }
};

/////////////////////////////////////////////////////////递归实现（\/）/////////////////////////////////////////////////////////////

void preOrder(TreeNode* node)
{
  if(node == nullptr)
  {
    return;
  }
  cout << node->val << " ";
  preOrder(node->left);
  preOrder(node->right);
}

void inOrder(TreeNode* node)
{
  if(node == nullptr)
  {
    return;
  }
  preOrder(node->left);
  cout << node->val << " ";
  preOrder(node->right);
}

void posOrder(TreeNode* node)
{
  if(node == nullptr)
  {
    return;
  }
  posOrder(node->left);
  posOrder(node->right);
  cout << node->val << " ";
}

////////////////////////////////////////////////////递归实现(/\)//////////////////////////////////////////////////////////////

TreeNode* head = nullptr;


/////////////////////////////////////////////////////////非递归实现（\/）/////////////////////////////////////////////////////////////

void preOrder_1(TreeNode* node)//node is head node of a binary tree
{
    Stack s;
    s.push(node);
    while(!s.isEmpty())
    {
        node = s.pop();
        cout << node->val << " ";
        if(node->right != nullptr)
        {
            s.push(node->right);
        }
        if(node->left != nullptr)
        {
            s.push(node->left);
        }
    }
}

void inOrder_1(TreeNode* node)
{
  if(node != nullptr)
  {
    Stack s;

    while(!isEmpty() || node!=nullptr)
    {
      if(node != nullptr)
      {
	s.push(node);
	node = node->left;
      }
      else
      {
	node = s.pop();
	cout << node->val << " ";
	node = node->right;
      }
    }
  }
}

void posOrder_1(TreeNode* node)
{
}

////////////////////////////////////////////////////非递归实现（/\）//////////////////////////////////////////////////////////////

int main()
{
  std::random_device rd;
  std::mt19937 gen(rd());//有机会探讨下这种随机生成的数学原理
  std::uniform_int_distribution<int> val_range(0,1001); 
  std::uniform_int_distribution<int> node_range(1,101);
  std::uniform_int_distribution<int> select_child(0,4);//0 没有孩子节点，1插入左孩子节点，2插入右孩子节点，3插入两个孩子节点
  std::uniform_int_distribution<int> select_whereToGo(0,2);//0:去左边   1:去右边OB
  ////////////////////////////////////标记出已经创建的节点//////////////////////


  int remainingNode = 0;
  int N = 0;//节点总数
  int s_child = 0;//选择孩子个数
  

  for(int i = 0;i<10;++i)//实验10次
  {
    N = node_range(gen);
    remainingNode = N;
    head = new TreeNode(val_range(gen));//初始化头节点

    while(remainingNode != 0)
    {
      s = select(gen);
      if()
    }
  } 
}



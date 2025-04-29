#include<iostream>

enum {RED,BLACK};

template<typename K,typename V>
class rbTree
{
private:
  struct Node
  {
    unsigned int color;
    K key;
    V value; 

    Node* left;
    Node* right;
    Node* parent;

    Node(K k,V val) :color(RED),key(k),value(val)// initial node color is red
		    ,left(nullptr),right(nullptr)
		    ,parent(nullptr) {}

    Node(unsigned int c,K k,V val,Node* l,Node* r,Node* p) :color(c),key(k)
							  ,value(val),left(l)
							  ,right(r),parent(p) {}
  };
  Node* root;
  Node* nil = new Node(BLACK,K(),V(),nullptr,nullptr,nullptr);

public:
  rbTree(){root == nullptr;}

  ~rbTree()//RALL,we will free memory only in ~reTree() and delete()
  {
      
  }

  void left_rotate(Node* root,Node* x)//both those Sare pointer,save memory(don't select x is a node)
  {
    Node* y = x->right;

    //change first thread
    x->right = y->left;
    if(y->left!=nil && y->left->left!=nil)//y and y's left tree not are leaf nodes
    {
      y->left->parent = x;
    }

    //second thread
    if(x == root) { root = y; }//update my root node
    else
    {
      y->parent = x->parent;
      if(x->parent->left == x)
      {
	x->parent->left = y;
      }
      else {x->parent->right = y;}
    }

    //third thread
    y->left = x;
    x->parent = y;
  }

  void right_rotate(Node*& root,Node* x)
  {
    Node* x = y->left;

    //change first thread
    y->left = x->right;
    if(x->right!=nil && x->right->right!=nil)//y and y's left tree not are leaf nodes
    {
      x->parent->parent = y;
    }

    //second thread
    if(y == root) { root = x; }//update my root node
    else
    {
      x->parent = y->parent;
      if(y->parent->right == y)
      {
	y->parent->right = x;
      }
      else {y->parent->left = x;}
    }

    //third thread
    x->left = y;
    y->parent = x;
  }

  void insert(K key,V value)
  {
        
  }

  void traverse_preOrder(Node* root)
  {
    
  }

  V find(K k)//k is what I wanna find key. 
  {
    
  }
};


int main()
{
  
}


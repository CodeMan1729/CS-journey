#include<iostream>
using namespace std;

static unsigned char RED = 1;
static unsigned char BLACK = 0;

template<typename  k,typename v>
  struct rbTree
  {
      struct Node
      {
        v value;
        k key;

        unsigned char color;// It can present 256 colors. And unsigned char just occupy 1 bit memory.
        Node* right;
        Node* left;
        Node* parent;
	
	Node(k ke,v val,unsigned char c,Node* l,Node* r,Node* p)
	  : key(ke),value(val),color(c),left(l),right(r),parent(p)  {}	  
      };

      Node*  root;
      Node*  nil;

      rbTree()
      {
	nil = new Node(0,0,BLACK,nullptr,nullptr,nullptr);
	root = nil;
      }

      ~rbTree()
      {
	delete nil;
	//Implement a method to delete all nodes
      }

     void insertNode(k key,v val)
      {
	if(root == nil)//havn't node
	{
	  root = new Node(key,val,BLACK,nil,nil,nil);//according to defination, this is head node.
	}
	else
	{
	  //create node normally
	}
      }

      void left_rotate(rbTree r,Node x)
      {
	Node y = x->right;

	//modify three thread.we modify x--b firstly.
	x->right = y->left;
	if(y.left != nil)//compare pointer
	{
	  y->left->parent = y->parent;
	}

	//modify second thread: x's parent -- y
	y->parent = x->parent;
	if(x.parent == nil)//x is root node
	{
	  r->root = y;
	}
	else
        {
	  if(x->parent->left == x)
	  {
	    x->parent->left = y;
	  }
	  else
	  {
	    x->parent->right = y;
	  }
	}
	
	//The third thread
	y->left = x;
	x->parent = y;

      }
      
      void right_rotate(rbTree r,Node y)
      {
	Node x = y->left;

	//modify three thread.we modify y--b firstly.
	y->left = x->right;
	if(x.right != nil)//compare pointer
	{
	  x->right->parent = x->parent;//nil->parent = nullptr, so we should judge
	}

	//modify second thread: y's parent -- x
	x->parent = y->parent;
	if(y.parent == nil)//y is root node
	{
	  r->root = x;
	}
	else
        {
	  if(y->parent->right == y)
	  {
	    y->parent->right = x;
	  }
	  else
	  {
	    y->parent->left = x;
	  }
	}
	
	//The third thread
	x->right = y;
	y->parent = x;	
      }

      void insert_fixup(rbTree r,Node z)// z color = red
      {
        while(z->parent.color == RED)
	{
	  Node uncle;
	  //define uncle node
	  if(z->parent == z->parent->parent->left)
	  {
	    uncle = z->parent->right;
	  }
	  else
	  {
	     uncle = z->parent->left;
	  }
	  
	  //modify
	  if(uncle.color == RED)//z = RED,father = RED, uncle = RED,grandfather = BLACK
	  {
	    uncle->parent.color = RED;
	    uncle.color = BLACK;
	    z->parent.color = BLACK;
	    // if grandfather's father = RED, we need take while loop???????????????
	  }
	  else// uncle = BLACK
	  {
	    if(z == z->parent->right)// z is right node of father
	    {
	      z = z->parent;
	      left_rotate(r,z);
	    }
	      z->parent = BLACK;
	      z->parent->parent = RED;
	      right_rotate(r,z->parent->parent);
	  }

	 }
      }
      

      void insertNode(rbTree r,Node z)
       {
         if(r.root == r.nil)//havn't node
   	 {
	   r.root = new Node(z.key,z.value,BLACK,nil,nil,nil);//according to defination, this is head node.
	 }
	 else
	 {
	   Node x = *root;//x is root node,then travel
	   
	   //find where insert
	   while(x.left != r.nil)//x.right also is correct
	  {
	    if(z.key < x.key)
	    {
	      x = x->left;
	    }
	    else if(z.key > x.key)
	    {
	      x = x->right;
	    }
	    else
	    {
	      // equal according the real condition
	    }

	    //insert z
	    if(x.key > z.key)
	    {
	      x->left = z;
	      z.parent = x;
	      z.left = r.nil;
	      z.right = r.nil;
	      z.color = RED;//this operation doesn't change black node number, we will rotate to avoid red node is adjacent

	    }
	    else if(x.key < z.key)
	    {
	      x->right = z;
	      z.parent = x;
	      z.left = r.nil;
	      z.right = r.nil;
	      z.color = RED;
	    }
	    else
	    {
	      //
	    }


	  }
	 }
       }
  };




int main()
{
  
}


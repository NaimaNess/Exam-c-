#include<iostream>

using namespace std;
	
//template <typename T>

class BST{
	struct node{
		int data;
//		int value;
		node *left;
    	node *right;
		};

	node* root;

//	node* makeEmpty(node* t);
//	node* insert(int x, node* t);
//   	node* remove(int x, node* t);
//        node* find(node* t, int x);
//     	void inorder(node* t);



	node* makeEmpty(node* t){
        	if(t == nullptr)
            	return nullptr;
        	{
            	makeEmpty(t->left);
            	makeEmpty(t->right);
           	delete t;
        	}
        	return nullptr;
    	}

	node* insert(int x, node* t){
        if(t == nullptr)
        {
            t = new node;
            t->data = x;
            t->left = t->right = nullptr;
        }
        else if(x < t->data)
            t->left = insert(x, t->left);
        else if(x > t->data)
            t->right = insert(x, t->right);
        return t;
    	}
    
    node* findMin(node* t){
        	if(t == nullptr)
            		return nullptr;
        	else if(t->left == nullptr)
            		return t;
        	else
            		return findMin(t->left);
    	}

    node* findMax(node* t){
        	if(t == nullptr)
            		return nullptr;
        	else if(t->right == nullptr)
            		return t;
        	else
            		return findMax(t->right);
    	}

	node* remove(int x, node* t){
        	node* temp;
        	if(t == nullptr)
            		return nullptr;
        	else if(x < t->data)
            		t->left = remove(x, t->left);
        	else if(x > t->data)
            		t->right = remove(x, t->right);
        	else if(t->left && t->right)
        	{
            		temp = findMin(t->right);
            		t->data = temp->data;
            		t->right = remove(t->data, t->right);
        	}
        	else{
            		temp = t;
            		if(t->left == nullptr)
                		t = t->right;
            		else if(t->right == nullptr)
                		t = t->left;
            		delete temp;
        	}

        	return t;
    	}
    node* find(node* t, int x){
        	if(t == nullptr)
            		return nullptr;
        	else if(x < t->data)
            		return find(t->left, x);
        	else if(x > t->data)
        		return find(t->right, x);
        	else
           		return t;
   	 }
    void inorder(node* t){
        	if(t == nullptr)
            		return;
      		inorder(t->left);
        	cout << t->data << " ";
        	inorder(t->right);
    	}	
 	
public:
  	BST(){
        	root = nullptr;
    	}

    	~BST(){
       		root = makeEmpty(root);
   	}
	void insert(int x){
        	root = insert(x, root);
    	}
	void remove(int x){
        	root = remove(x, root);
    	}
	void display(){
        	inorder(root);
        	cout << endl;
    	}
	void search(int x){
        	root = find(root, x);
    	}
};

int main()
{
    BST t;
    t.insert(20);
    t.insert(25);
    t.insert(1599);
    t.insert(103);
    t.insert(30333);
    t.display();
    t.remove(20);
    t.display();
    t.remove(25);
    t.display();
    t.remove(30);
    t.display();
    return 0;
 }

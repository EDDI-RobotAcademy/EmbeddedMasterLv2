#include <stdio.h>
#include <stdlib.h>

typedef struct _stack stack;
typedef struct _queue queue;
typedef struct _tree tree;

struct _stack
{
	int data;
	struct _stack* link;
};

struct _queue
{
	int data;
	struct _queue* link;
};

struct _tree
{
	int data;
	struct _tree* left;
	struct _tree* right;
};

stack* create_new_stack_node()
{
	stack* tmp = malloc(sizeof(stack));
	tmp->link = NULL;
	return tmp;
}

queue* create_new_queue_node()
{
	queue* tmp = malloc(sizeof(queue));
	tmp->link = NULL;
	return tmp;
}

tree* create_new_tree_node()
{
	tree* tmp = malloc(sizeof(tree));
	tmp->left = NULL;
	tmp->right = NULL;
	return tmp;
}

stack* push_stack_data(stack* root, int data)
{
	stack* tmp = root;
	root = create_new_stack_node();
	root->data = data;
	root->link = tmp;
	return root;
}

stack* pop_stack_data(stack* root)
{
	stack* tmp = root;
	root = root->link;
	free(tmp);
	return root;
}

void print_stack_data(stack* root)
{
	while(root)
	{
		printf("stack root : %d\n",root->data);
		root = root->link;
	}
}
queue* insert_queue_data(queue* root, int data)
{
	queue* tmp = root;

	if(!root)
	{
		root = create_new_queue_node();
		root->data = data;
		return root;
	}
	else
	{
		while(root->link)
		{
			root = root->link;
		}
		root->link = create_new_queue_node();
		root->link->data = data;
		return tmp;
	}
}

queue* delete_queue_data(queue* root)
{
	printf("deletequeue\n");
	queue* tmp = root;
	root = root->link;
	free(tmp);
	return root;
}

void print_queue_data(queue* root)
{
	while(root)
	{
		printf("queue root : %d\n", root->data);
		root = root->link;
	}
	printf("print fininshed\n");
}

tree* insert_tree_data(tree* root, int data)
{
	tree* tmp = root;

		
	if(!root)
	{
		root = create_new_tree_node();
		root->data = data;
		return root;
	}
	while(root)
	{
		if(root->data > data)
		{
			if(root->left)
				root = root->left;
			else
			{
				root->left = create_new_tree_node();
				root->left->data = data;
				return tmp;
			}
			
		}
		else if(root->data < data)
		{
			if(root->right)
				root = root->right;
			else
			{
				root->right = create_new_tree_node();
				root->right->data = data;
				return tmp;
			}
		}
	}
	return tmp;
}

tree* delete_tree_data(tree* root, int data)
{
	tree* tmp = root;
	tree* prev_cursor = root;
	int left = 0;
	int flag_root = 1;
	if(!root)
	{
		printf("there is no %d in tree\n",data);
	}

	while(root->data !=  data)
	{
		if(root->data < data)
		{
			prev_cursor = root;
			root = root->right;
			left = 0;
			flag_root = 0;
		}
		else
		{
			prev_cursor = root;
			root = root->left;
			left = 1;
			flag_root = 0;
		}
		if(!root)
		{
			printf("there is no %d in tree\n", data);
			return tmp;
		}
	}
	if(root->right && root->left)
	{
		tree* cursor = root->right;
		tree* prev = root;
		while(cursor->left)
		{
			prev = cursor;
			cursor = cursor->left;
		}
		tree* tmp2 = cursor;
		root->data = cursor->data;
		if(prev == root)
			prev->right = cursor->right;
		else
			prev->left = cursor->right;
		free(tmp2);
	}
	else if(root->right)
	{
		tree* tmp2 = root;
		if(flag_root)
		{
			prev_cursor = root->right;
			free(tmp2);
			return prev_cursor;
		}
		if(left)
			prev_cursor->left = root->right;
		else
			prev_cursor->right = root->right;
		free(tmp2);
	}
	else if(root->left)
	{
		tree* tmp2 = root;
		if(flag_root)
		{
			prev_cursor = root->left;
			free(tmp2);
			return prev_cursor;
		}
		if(left)
			prev_cursor->left = root->left;
		else
			prev_cursor->right = root->left;
		free(tmp2);
	}
	else
	{
		free(root);
		if(flag_root)
			return NULL;
		if(left)
			prev_cursor->left = NULL;
		else
			prev_cursor->right = NULL;
	}

	return tmp;
}

void print_tree_data(tree* root)
{
	if(root)
	{
		print_tree_data(root->left);
		printf("root : %d\n",root->data);
		print_tree_data(root->right);
	}
}

int main()
{
	stack* root_stack = NULL;
	queue* root_queue = NULL;
	tree* root_tree = NULL;

	int i;

	int data[10] = {8,3,10,1,6,4,7,14,13,2};
	
	printf("///// stack /////\n");
	for(i=0;i<10;i++)
		root_stack = push_stack_data(root_stack, data[i]);
	printf("after push\n");
	print_stack_data(root_stack);
	for(i=0;i<10;i++)
		root_stack = pop_stack_data(root_stack);
	printf("after pop\n");
	print_stack_data(root_stack);


	printf("///// queue /////\n");
	for(i=0;i<10;i++)
		root_queue = insert_queue_data(root_queue, data[i]);
	printf("after insertion\n");
	print_queue_data(root_queue);
	for(i=0;i<10;i++)
		root_queue = delete_queue_data(root_queue);
	printf("after deletion\n");
	print_queue_data(root_queue);


	printf("///// tree /////");
  for(i=0;i<10;i++)
    	root_tree = insert_tree_data(root_tree, data[i]);
  printf("after insertion\n");

  print_tree_data(root_tree);
#if 1
  for(i=9;i>=0;i--)
	{
		printf("delete %d\n",data[i]);
    root_tree = delete_tree_data(root_tree, data[i]);
		print_tree_data(root_tree);
	}
  printf("after deletion\n");
  print_tree_data(root_tree);
#endif

}

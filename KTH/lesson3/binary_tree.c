#include <stdlib.h>
#include <stdio.h>

typedef struct _tree tree;
struct _tree
{
	int data;
	struct _tree *left;
	struct _tree *right;
};

tree* create_tree_node(void)
{
	tree *tmp;

	tmp = (tree *)malloc(sizeof(tree));
	tmp->left = NULL;
	tmp->right = NULL;
	return tmp;
}

#if 1
void insert_tree_data(tree **root, int data)
{
	if(!(*root))
	{
		*root = create_tree_node();
		(*root)->data = data;
		return;
	}
	if((*root)->data < data)
	{
		insert_tree_data(&(*root)->right, data);
	}
	else // (*root)->data > data
	{
		insert_tree_data(&(*root)->left, data);
	}
	
}
#endif

void insert_tree_data(tree **root, int data)
{
	if(!(*root))
	{
		*root = create_tree_node();
		(*root)->data = data;
		return;
	}
	tree* tmp = *root;
	while(tmp)
	{
		if(tmp->data < data)
		{
			if(tmp->right)
				tmp = tmp->right;
			else
			{
				tmp->right = create_tree_node();
				tmp->right->data = data;
				return;
			}
		}
		else
		{
			if(tmp->left)
				tmp = tmp->left;
			else
			{
				tmp->left = create_tree_node();
				tmp->left->data = data;
				return;
			}

		}
	}
}

tree* find_tree_data(tree **root, int data)
{
	if(!(*root))
	{
		printf("there is no %d in tree\n",data);
		return *root;
	}
	if((*root)->data < data)
	{
		return find_tree_data(&(*root)->right,data);
	}
	else if((*root)->data > data)
	{
		return find_tree_data(&(*root)->left,data);
	}
	else
		return *root;	
}

tree** find_parent_tree_data(tree **root, int data)
{
        if(!(*root))
        {
                printf("there is no %d in tree\n",data);
                return root;
        }
        if((*root)->data < data)
        {
                return find_parent_tree_data(&(*root)->right,data);
        }
        else if((*root)->data > data)
        {
                return find_parent_tree_data(&(*root)->left,data);
        }
        else
                return root;
}

tree** find_tree_node_max(tree **root)
{
	if((*root)->right)
		return find_tree_node_max(&(*root)->right);
	else
		return root;
}

tree** find_tree_node_min(tree **root)
{
	if((*root)->left)
                return find_tree_node_max(&(*root)->left);
        else
                return root;

}

#if 1
void delete_tree_data(tree **root, int data)
{
	tree** tmp = root;
	if(!(*tmp))
	{
		printf("there is no %d in tree\n",data);
		return;
	}
	while((*tmp)->data != data)
	{
		if((*tmp)->data < data)
		{
			tmp = &(*tmp)->right;
		}
		else
		{
			tmp = &(*tmp)->left;
		}
		if(!(*tmp))
		{
			printf("there is no %d in tree\n", data);
			return;
		}
	}
	
	if((*tmp)->right && (*tmp)->left)
	{
		tree** cursor = &(*tmp)->right;
		while((*cursor)->left)
		{
			cursor = &(*cursor)->left;
		}
		tree* tmp2 = (*cursor);
		(*tmp)->data = (*cursor)->data;
		*cursor = (*cursor)->right;
		free(tmp2);
	}
	else if((*tmp)->right)
	{
		tree* tmp2 = *tmp;
		(*tmp) = (*tmp)->right;
		free(tmp2);
	}
	else if((*tmp)->left)
	{
		tree* tmp2 = *tmp;
		(*tmp) = (*tmp)->left;
		free(tmp2);
	}
	else
	{
		free(*tmp);
		*tmp = NULL;
	}
}
#endif

void delete_tree_data(tree **root, int data)
{
	if(!(*root))
	{
		printf("there is no %d in tree\n", data);
		return;
	}
	
	if((*root)->data < data)
		delete_tree_data(&(*root)->right,data);
	else if((*root)->data > data)
		delete_tree_data(&(*root)->left, data);
	else
	{
		if((*root)->right && (*root)->left)
		{
                	tree** cursor = &(*root)->right;
                	while((*cursor)->left)
                	{
                        	cursor = &(*cursor)->left;
                	}
                        tree* tmp2 = (*cursor);
                	(*root)->data = (*cursor)->data;
                	*cursor = (*cursor)->right;
                	free(tmp2);
	        }
        	else if((*root)->right)
        	{
                	tree* tmp2 = *root;
                	(*root) = (*root)->right;
                	free(tmp2);
        	}
        	else if((*root)->left)
        	{
                	tree* tmp2 = *root;
                	(*root) = (*root)->left;
                	free(tmp2);
        	}
        	else
        	{
                	free(*root);
                	*root = NULL;
        	}

	}
}
#if 0
void delete_tree_data(tree **root, int data)
{
        if(!(*root))
        {
                printf("there is no %d in tree\n",data);
                return;
        }
        if((*root)->data < data)
	{
		if(!((*root)->right))
		{
			printf("there is no %d in tree\n",data);
			return;
		}
		if((*root)->right->data == data)
		{
			tree* tmp = (*root)->right;
			if((*root)->right->left)
			{
				tree** left_max = find_tree_node_max(&(*root)->right->left);
				(*root)->right = *left_max;
				*left_max = (*left_max)->left;
				(*root)->right->left = tmp->left;
				(*root)->right->right = tmp->right;
			}
			else
				(*root)->right = (*root)->right->right;
			free(tmp);
		}
		else
		{
			delete_tree_data(&(*root)->right,data);
		}
        }
        else if((*root)->data > data)
        {
		if(!((*root)->left))
		{
			printf("there is no %d in tree\n", data);
			return;
		}
		if((*root)->left->data == data)
		{
			tree* tmp = (*root)->left;
			if((*root)->left->right)
			{
				tree** right_min = find_tree_node_min(&(*root)->left->right);
				(*root)->left = *right_min;
				*right_min = (*right_min)->right;
				(*root)->left->right = tmp->right;
				(*root)->left->left = tmp->left;
			}
			else
				(*root)->left = (*root)->left->left;
			free(tmp);
		}
		else
		{
                	delete_tree_data(&(*root)->left,data);
		}
        }
        else // delete root
	{
		tree* tmp = (*root);
		if((*root)->left)
		{
                 	tree** left_max = find_tree_node_max(&(*root)->left);
			(*root) = *left_max;
			*left_max = (*left_max)->left;
			(*root)->left = tmp->left;
			(*root)->right = tmp->right;
		}
		else if((*root)->right)
		{
			tree** right_min = find_tree_node_min(&(*root)->right);
			(*root) = *right_min;
			*right_min = (*right_min)->right;
			(*root)->right = tmp->right;
			(*root)->left = tmp->left;
		}
		else
		{
			free(*root);
			*root = NULL;
		}	
	}

}

#endif

void print_tree(tree *root)
{
	if(root)
	{
		printf("tree root = %d\n", root->data);
		print_tree(root->left);
		//printf("tree root = %d\n", root->data);
		print_tree(root->right);
	}	
}

int main(void)
{
	int i;
	tree *root = NULL;
	int data[] = { 34, 17, 55, 10, 13, 12, 53, 57, 100, 1, 235, 32, 25 };
	int find_num = 102;
	for(i=0;i<sizeof(data)/sizeof(int);i++)
	{
		insert_tree_data(&root, data[i]);
	}
	printf("root : %d\n", root->data);
	print_tree(root);
	
	if(find_tree_data(&root,find_num))
		printf("find %d, %d\n",find_num, find_tree_data(&root,find_num)->data);
	printf("delete 55\n");
	delete_tree_data(&root, 55);
	print_tree(root);
	printf("delete 1\n");
	delete_tree_data(&root, 1);
	print_tree(root);
	printf("delete root 34\n");
	delete_tree_data(&root, 34);
	print_tree(root);
	printf("root is now %d\n",root->data);
	delete_tree_data(&root, 34);
	print_tree(root);
	printf("re delete 34\n");
#if 1
	for(i=0;i<sizeof(data)/sizeof(int);i++)
	{
		printf("delete %d\n",data[i]);
		delete_tree_data(&root, data[i]);
		print_tree(root);
	}
#endif
	return 0;
}
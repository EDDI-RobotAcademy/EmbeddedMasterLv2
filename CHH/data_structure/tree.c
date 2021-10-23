#include <stdlib.h>
#include <stdio.h>

typedef struct _tree tree;

struct _tree
{
	int data;
	struct _tree* left;
	struct _tree* right;
};


tree* create_tree_node()
{
	tree* tmp;
    
    tmp = (tree*)malloc(sizeof(tree));
    tmp->left = NULL;
    tmp->right = NULL;

    return tmp;
}

void insert_tree_data(tree** root, int data)
{

    if ((*root) == NULL)
    {
        *root = create_tree_node();
        (*root)->data = data;
        return;
    }

    if ((*root)->data > data)
    {
        insert_tree_data(&(*root)->left, data);
    }

    else
    {
        insert_tree_data(&(*root)->right, data);
    }

    return;
}

void delete_tree_data(tree** root, int data)
{
    tree* tmp;
    tree* delete_node;

    if ((*root) == NULL)
    {
        printf("deleted failed \n");
        return;
    }

    if ((*root)->data == data)
    {
        delete_node = (*root);
        tmp = delete_node->right;

        while(tmp)
        {
            tmp = tmp->left;
        }

        if 

    }

    if ((*root)->data > data)
    {
        delete_tree_data(&(*root)->left, data);
    }

    else
    {
        delete_tree_data(&(*root)->right, data);
    }

    return;

    
}

int search_tree_data(tree** root, int data)
{
    
    if ((*root) == NULL)
    {
        printf("search failed!!\n");
        return -1;
    }
    
    if ((*root)->data == data)
    {
        printf("data = %d \n", (*root)->data);
        return (*root)->data;
    }
 
    if ((*root)->data > data)
    {
        search_tree_data(&(*root)->left, data);
    }
    
    else
    {
        search_tree_data(&(*root)->right, data);
    }

    return 0;
}

tree* find_tree_node(tree** root, int data)
{
    if ((*root) == NULL)
    {
        return NULL;
    }

    if ((*root)->data == data)
    {
        return (*root);
    }

    if ((*root)->data > data)
    {
        search_tree_data(&(*root)->left, data);
    }

    else
    {
        search_tree_data(&(*root)->right, data);
    }

    return NULL;
}

void print_tree(tree* root)
{
    if(root)
    {
        printf("tree root = %d\n",root->data);
        print_tree(root->left);
        print_tree(root->right);
    }

}

int main(void)
{
    int i;
    tree* root= NULL;
    int data[] = { 34, 17, 55, 10, 13, 12, 53, 57 };
    
    for (i = 0; i < 8; i++)
	{
		insert_tree_data(&root, data[i]);
	}

	print_tree(root);

    search_tree_data(&root, 10);
    search_tree_data(&root, 53);
    search_tree_data(&root, 52);
	
    delete_tree_data(&root, 34);

    print_tree(root);

    return 0;

}

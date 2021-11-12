#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define LEVEL(X) ((X) ? X->black_level : 0)
#define TREE 1
#define AVL_TREE 2
#define REDBLACK_TREE 3
#define RED 0
#define BLACK 1

typedef struct _redblack redblack;
struct _redblack
{
	int header;
	int data;
	struct _redblack *parent;
	struct _redblack *left;
	struct _redblack *right;
	int color;
	int black_level;
};

typedef struct _stack stack;
struct _stack
{
	void **data;
	struct _stack *link;
};

stack *create_stack_node(void)
{
	stack *tmp;

	tmp = (stack *)malloc(sizeof(stack));
	tmp->link = NULL;

	return tmp;
}

bool stack_is_not_empty(stack* top)
{
	if(top)
		return true;
	else
		return false;
}

void push(stack** top, void* data)
{
	if(!(*((redblack **)data)))
		return;
	stack *tmp = *top;

	*top = create_stack_node();
	(*top)->data = data;
	(*top)->link = tmp;
}

void* pop(stack** top)
{
	void* data;
	stack *tmp;

	if(!(*top))
	{
		printf("Stack is empty\n");
		return NULL;
	}

	tmp = *top;

	data = tmp->data;
	*top = tmp->link;

	free(tmp);

	return data;
}

bool check_dup(int *data, int compare, int idx)
{
    int i;

    for (i = 0; i < idx; i++)
    {
        if (data[i] == compare)
        {
            return true;
        }
    }

    return false;
}

void init_data(int *data, int num)
{
    int i, tmp;

    for (i = 0; i < num; i++)
    {
        reassign:
        tmp = rand() % num + 1;

        if (check_dup(data, tmp, i))
        {
            goto reassign;
        }

        data[i] = tmp;
    }
}

void print_arr(int *data, int num)
{
    int i;

    for (i = 0; i < num; i++)
    {
        printf("%4d  ", data[i]);

        if (i % 7 == 6)
        {
            printf("\n");
        }
    }

    printf("\n");
}

redblack *create_redblack_node(void)
{

    redblack* tmp = (redblack *) malloc(sizeof(redblack));
    if (!tmp)
    {
        printf("Failed malloc\n");
    }

    tmp->header = REDBLACK_TREE;
    tmp->parent = NULL;
    tmp->left = NULL;
    tmp->right = NULL;
    tmp->color = RED;
    tmp->black_level = 0;

    return tmp;
}

void update_black_level(redblack **root)
{
	if((*root)->color == BLACK)
		(*root)->black_level = MAX(LEVEL((*root)->left), LEVEL((*root)->right)) + 1;
}

int balance_factor(redblack **root)
{
	return LEVEL((*root)->left) - LEVEL((*root)->right);
} 

void check_black_property(redblack **root)
{
#if 0
	if(LEVEL((*root)->left) != LEVEL((*root)->right))
		rotate(root);
#endif
}
void insert_redblack(redblack **root, int data)
{
	redblack** root_backup = root;
	redblack** tmp_parent = NULL;
	redblack** tmp_grandparent = NULL;
	redblack** cursor = NULL;
	
	redblack** tmp_bro = NULL;
	redblack** tmp_uncle = NULL;
	redblack** tmp_mine = NULL;

	redblack* rotate_middle;
	redblack* rotate_bro;
	redblack* rotate_mine;
	int black_cnt = 0;
	redblack* NIL;
	if(!(*root))
	{
        // make NIL singleton
        	NIL = (redblack *)malloc(sizeof(redblack));
        	NIL->header = REDBLACK_TREE;
        	NIL->parent = NULL;
        	NIL->left = NULL;
        	NIL->right = NULL;
        	NIL->color = BLACK;
        	NIL->black_level = 1;
		NIL->data = -1;
		goto create_node;
	}
	else
	{
		NIL = (*root)->parent;
	}
	while((*root)!=NIL)
	{
		if((*root)->color == BLACK)
		{
			black_cnt++;
		}
		if((*root)->data > data)
		{	
			tmp_grandparent = tmp_parent;
			tmp_parent = root;
			root = &(*root)->left;
		}
		else if((*root)->data < data)
		{
			tmp_grandparent = tmp_parent;
			tmp_parent = root;
			root = &(*root)->right;
		}
	}
	create_node:
	// tmp == NULL when first insert node
	*root = create_redblack_node();
	(*root)->data = data;
	(*root)->parent = tmp_parent ? *tmp_parent : NIL;
	(*root)->left = NIL;
	(*root)->right = NIL;
	(*root)->black_level = 1;
	(*root)->color = tmp_parent ? RED : BLACK;
	
#if 1
	cursor = root;
	while(*cursor)
	{
		update_black_level(cursor);
		cursor = &(*cursor)->parent;
	}	
	
	// RECONSTRUCTION
	if((*root)->parent->parent == NULL)
                return;
        if((*root)->parent->parent->data == -1)
                return;

        if((*root)->parent->parent->left == (*root)->parent)
        {
                //tmp_parent = &(*root)->parent->parent->left;
                tmp_uncle = &(*root)->parent->parent->right;
        }
        else
        {
                //tmp_parent = &(*root)->parent->parent->right;
                tmp_uncle = &(*root)->parent->parent->left;
        }

        if((*root)->parent->left == (*root))
        {
                tmp_mine = &(*root)->parent->left;
                tmp_bro = &(*root)->parent->right;
        }
        else
        {
                tmp_mine = &(*root)->parent->right;
                tmp_bro = &(*root)->parent->left;
        }

        if((*root)->parent->color == RED) // no adjacent red violation
        {
                // compare brother of parent
                if((*tmp_parent)->black_level == (*tmp_uncle)->black_level)
                {
			printf("parent color RED \n");
                        (*tmp_parent)->color = BLACK;
                        //after recoloring, same black level violation occurs.
                        //rotate
                        rotate_middle = (*tmp_parent);
                        rotate_mine = (*tmp_mine);
                        rotate_bro = (*tmp_grandparent);

                        rotate_bro->color = RED;
                        rotate_middle->parent = rotate_bro->parent;
                        rotate_bro->parent = rotate_middle;
                        *tmp_parent = *tmp_bro;
                        *tmp_bro = rotate_bro;
			*tmp_grandparent = rotate_middle;
			rotate_bro->black_level--;

                }
                else // real case?
                {

                }
        }
        else // BLACK
        {
                if((*tmp_parent)->black_level == (*tmp_bro)->black_level) // no violation
                {
                        return;
                }
                else // same black level violation
                {
                        if((*tmp_bro)->color == RED)
                                (*tmp_bro)->color == BLACK;
                        else//rotate
                        {
				
                        }
                }
        }

#endif	
}

void print_redblack(redblack *tree)
{

	redblack* tmp = tree;

	if(tmp)
	{
		print_redblack(tmp->left);
		if(tmp->data == -1)
			printf("data = NIL\t");
		else
			printf("data = %4d\t", tmp->data);

		if(tmp->parent)
		{
			if(tmp->parent->data == -1)
				printf("parent = NIL\t");
			else
				printf("parent = %4d\t", tmp->parent->data);
		}
		else
		{
			printf("parent = NULL\t");
		}
		if(tmp->left)
		{
			if(tmp->left->data == -1)
				printf("left = NIL\t");
			else
				printf("left = %4d\t", tmp->left->data);
		}
		else
		{
			printf("left = NULL\t");
		}

		if(tmp->right)
		{
			if(tmp->right->data == -1)
				printf("right = NIL\t");
			else
				printf("right = %4d\t", tmp->right->data);
		}
		else
		{
			printf("right = NULL\t");
		}

		printf("black level = %2d\t", tmp->black_level);
		
		if(tmp->color == RED)
		{
			printf("color = RED\n");
		}
		else
		{
			printf("color = BLACK\n");
		}
		print_redblack(tmp->right);
	}
}

void redblack_property_test(redblack **root)
{
	// 1. The color of root is black
	// NUMBER 1 condition will be checked in print function.

	// this function will be inserted in print function.
	// 2. There is no adjacent red node
	if((*root)->color == RED)
	{
		if((*root)->parent)
			assert((*root)->parent->color == BLACK);
		if((*root)->left)
			assert((*root)->left->color == BLACK);
		if((*root)->right)
			assert((*root)->right->color == BLACK);
	}
	// 3. Same black_level
	// compare left and right black level
	assert((*root)->right->black_level == (*root)->left->black_level);	
}

int main()
{
    redblack *root = NULL;

    int i;

#if 1
    int data[5] = { 0 };
#endif

#if 0
    int data[4095] = { 0 };
#endif
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);
#if 1
	data[0] = 5;
	data[1] = 1;
	data[2] = 4;
	data[3] = 2;
	data[4] = 3;
	len = 3;
	print_arr(data, len);
	// LR case is needed to be updated.
#endif
    for (i = 0; i < len; i++)
    {
	printf("insert data : %d\n", data[i]);
        insert_redblack(&root, data[i]);
	print_redblack(root);
    }
    // property 1 : The color of root node is BLACK
    assert((root ? root->color : RED) == BLACK);
    printf("The color of root node is BLACK\n");
    print_redblack(root);

#if 0
    printf("delete redblack tree\n");
    for (i = 0; i < len; i++)
    {
	    printf("delete redblack tree data : %4d\n", data[i]);
	    delete_redblack(&root, data[i]);
	    print_redblack(root);
    }
    print_redblack(root);
#endif
}

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define TREE 1
#define AVL_TREE 2
#define REDBLACK_TREE 3

#define DEBUG 1

typedef struct _avl avl;
struct _avl
{
	int header;
	int data;
	struct _avl *left;
	struct _avl *right;
	int level;
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
	if(!(*((avl **)data)))
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

avl *create_avl_node(void)
{
    avl *tmp = (avl *)malloc(sizeof(avl));

    if (!tmp)
    {
        printf("Failed malloc\n");
    }

    tmp->header = AVL_TREE;
    tmp->left = NULL;
    tmp->right = NULL;

    return tmp;
}

void update_level(avl **root)
{
#if 0
	int flag = 0;
	assert(*root);
	assert(root);
	if((*root)->right)
	{
		if((*root)->right->level == (*root)->level)
		{
			(*root)->level +=1;
			return;
		}
		if((*root)->right->level < (*root)->level - 1) // for delete
		{
			flag = 1;
		}
	}
	if((*root)->left)
	{
		if((*root)->left->level == (*root)->level)
		{
			(*root)->level +=1;
			return;
		}
		if((*root)->left->level < (*root)->level - 1) // for delete
		{
			if(flag)
			{
#if 0
				(*root)->level -= 1;
#endif
				(*root)->level = MAX((*root)->left->level,(*root)->right->level) + 1;
			}
		}
	}
	if(!(*root)->left && !(*root)->right)
	{
		(*root)->level = 1;
	}
#endif
#if 0
	printf("before update_level : root = %d, level = %d \n",(*root)->data,(*root)->level);
#endif
	(*root)->level = MAX((*root)->left ? (*root)->left->level:0, (*root)->right ? (*root)->right->level:0) + 1;
#if 0
	printf("after update_level : root = %d, level = %d \n",(*root)->data,(*root)->level);
#endif
#if 1
	assert((*root)->level == MAX((*root)->left ? (*root)->left->level:0, (*root)->right ? (*root)->right->level:0) + 1 && "update level FAIL");
#endif
}

int balance_factor(avl **root)
{
	assert((*root));

	return ((*root)->left ? (*root)->left->level : 0) - ((*root)->right ? (*root)->right->level : 0);
}

void adjust_balance(avl **root, int data)
{
	avl* rotate_middle;
	avl* rotate_left;
	avl* rotate_right;
	int balance = balance_factor(root);
	if(balance == 2)
	{
		balance = balance_factor(&(*root)->left);
		if(balance >= 0)
		{
#if 0
			printf("rotate right\n");
#endif
			rotate_middle = (*root)->left;
			rotate_right = (*root);
			rotate_left = (*root)->left->left;
			*root = (*root)->left;
			rotate_right->left = rotate_middle->right;
			rotate_middle->right = rotate_right;
#if 1	
			rotate_right->level -= 2;
#endif
#if 0
			update_level(&rotate_right);
			update_level(&rotate_middle);
#endif
		}
		else
		{
#if 0
			printf("rotate left and right\n");
#endif
			rotate_middle = (*root)->left->right;
			rotate_left = (*root)->left;
			
			(*root)->left = rotate_middle;
			rotate_left->right = rotate_middle->left;
			rotate_middle->left = rotate_left;
#if 1
			rotate_left->level -=1;
			rotate_middle->level += 1;
#endif
#if 0
			update_level(&rotate_left);
			update_level(&rotate_middle);
#endif
			rotate_middle = (*root)->left;
                        rotate_right = (*root);
                        rotate_left = (*root)->left->left;
                        *root = (*root)->left;
                        rotate_right->left = rotate_middle->right;
                        rotate_middle->right = rotate_right;
#if 1
			rotate_right->level -= 2;
#endif
#if 0
			update_level(&rotate_right);
			update_level(&rotate_middle);
#endif
		}
	}
	else if(balance == -2)
	{
		balance = balance_factor(&(*root)->right);
		if(balance <= 0)
		{
#if 0
			printf("rotate left\n");
#endif
			rotate_middle = (*root)->right;
			rotate_left = (*root);
			rotate_right = (*root)->right->right;
			*root = (*root)->right;
			rotate_left->right = rotate_middle->left;
			rotate_middle->left = rotate_left;
#if 1
			rotate_left->level -= 2;
#endif
#if 0
			update_level(&rotate_left);
			update_level(&rotate_middle);
#endif
		}
		else
		{
#if 0
			printf("rotate right and left\n");     
#endif
			rotate_middle = (*root)->right->left;
                        rotate_right = (*root)->right;
                        
                        (*root)->right = rotate_middle;
                        rotate_right->left = rotate_middle->right;
                        rotate_middle->right = rotate_right;
#if 1
                        rotate_right->level -=1;
                        rotate_middle->level += 1;
#endif
#if 0
			update_level(&rotate_right);
			update_level(&rotate_middle);
#endif

			rotate_middle = (*root)->right;
                        rotate_left = (*root);
                        rotate_right = (*root)->right->right;
                        *root = (*root)->right;
                        rotate_left->right = rotate_middle->left;
                        rotate_middle->left = rotate_left;
#if 1
                        rotate_left->level -= 2;
#endif
#if 0
			update_level(&rotate_left);
			update_level(&rotate_middle);
#endif
		}
	}
#if 1
	balance = balance_factor(&(*root));

	assert(balance == 0 || balance == 1 || balance == -1);
#if 0
	printf("balance after rebalancing : root = %d, level = %d, bf = %d\n",(*root)->data,(*root)->level, balance_factor(&(*root)));
#endif
	if((*root)->right)
	{
		balance = balance_factor(&(*root)->right);
#if 0
		printf("balance after rebalancing : right = %d, level = %d, bf = %d\n",(*root)->right->data,(*root)->right->level, balance);
#endif
	}
        assert(balance == 0 || balance == 1 || balance == -1);
	if((*root)->left)
	{
		balance = balance_factor(&(*root)->left);
#if 0
		printf("balance after rebalancing : left = %d, level = %d, bf = %d\n",(*root)->left->data,(*root)->left->level, balance);
#endif
	}
        assert(balance == 0 || balance == 1 || balance == -1);


#endif
}

void insert_avl(avl **root, int data)
{
    if (!(*root))
    {
        *root = create_avl_node();
        (*root)->data = data;
        (*root)->level = 1;
    }

    if ((*root)->data > data)
    {
        insert_avl(&(*root)->left, data);
    }
    else if ((*root)->data < data)
    {
        insert_avl(&(*root)->right, data);
    }

    update_level(root);

    adjust_balance(root, data);
}

avl **find_tree_data(avl **root, int data)
{
	while (*root)
	{
		if ((*root)->data > data)
			root = &(*root)->left;
		else if ((*root)->data < data)
			root = &(*root)->right;
		else
			return root;
	}

	return NULL;
}

avl *chg_node(avl *root)
{
	avl *tmp = root;

	if(!root->right)
		root = root->left;
	else if(!root->left)
		root = root->right;

	free(tmp);

	return root;
}

void find_max(avl **root, int *data)
{
	stack *top = NULL;
	while(*root)
	{
		push(&top, root);
		if((*root)->right)
		{
			root = &(*root)->right;
		}
		else
		{
			*data = (*root)->data;
			*root = chg_node(*root);
			pop(&top);
			push(&top, root);
			break;
		}
	}
	while(stack_is_not_empty(top))
	{
		avl **t = (avl **)pop(&top);
		update_level(t);
		adjust_balance(t,(*t)->data);
	}
}

void delete_avl(avl **root, int data)
{
	stack *top = NULL;
	int num;
        while (*root)
        {
		push(&top, root);
                if ((*root)->data > data)
		{
                        root = &(*root)->left;
		}
                else if ((*root)->data < data)
		{
                        root = &(*root)->right;
		}
                else
		{
                        break;
		}
        }


	if((*root)->left && (*root)->right)
	{
		find_max(&(*root)->left, &num);
		(*root)->data = num;
	}
	else
	{
		(*root) = chg_node(*root);
		pop(&top);
		push(&top, root);
	}
#if 0
	if(top)
		printf("top is not empty!\n");
	else
		printf("top is empty!\n");
#endif
	while(stack_is_not_empty(top))
	{

		avl **t = (avl **)pop(&top);
#if 0
		printf("stack : %d\n",(*t)->data);
#endif
		update_level(t);

		assert((*t));

		adjust_balance(t,(*t)->data);
	}	
}


void nr_insert_avl(avl **root, int data)
{
}

void nr_delete_avl(avl **root, int data)
{
}

void print_avl(void *tree)
{

	// TODO
	// 1. 순회하면서 level check
	// 2. 순회하면서 balance factor check
	// 두개 만족하면 avl tree가 깨지지 않은것.
    if(!tree)
	    return;
    int header = *((int*)tree); // NULL POINTER EXCEPTION
    avl* tmp;
#if 0
    printf("PRINT HEADER : %d\n",header);
#endif
    switch(header)
    {
	case TREE:
		break;
	case AVL_TREE:
		tmp = (avl *)tree;
		break;
	case REDBLACK_TREE:
		break;
	default:
		break;
    }

    if (tmp)
    {
	int balance = balance_factor(&tmp);
#if 1
	if(balance != 1 && balance != 0 || balance != -1 )
	{
#if 0
		printf("tmp->data %d\n", tmp->data);
		printf("balance %d\n", balance);
#endif
		assert(balance == 1 || balance == 0 || balance == -1);
	}
	assert(balance == 1 || balance == 0 || balance == -1 );

	if(tmp->level - MAX(tmp->left ? tmp->left->level : 0, tmp->right ? tmp->right->level : 0) != 1)
	{
#if 0
		printf("tmp->data : %d\n", tmp->data);
#endif
		assert(tmp->level - MAX(tmp->left ? tmp->left->level : 0, tmp->right ? tmp->right->level : 0) != 1);
	}
#endif
        print_avl(tmp->left);

        printf("data = %4d\t", tmp->data);
	
        if (tmp->left)
        {
            printf("left = %4d\t", tmp->left->data);
        }
        else
        {
            printf("left = NULL\t");
        }

        if (tmp->right)
        {
            printf("right = %4d\t", tmp->right->data);
        }
        else
        {
            printf("right = NULL\t");
        }

        printf("level = %2d\n", tmp->level);

        print_avl(tmp->right);
    }
}

int main(void)
{
    avl *root = NULL;

    int i;
#if 1
    int data[10000] = { 0 };
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);
#endif
#if 0
	data[0] = 5;
	data[1] = 6;
	data[2] = 3;
	data[3] = 4;
	data[4] = 2;
	data[5] = 1;
	len = 6;
	print_arr(data, len);

	int data[16] = {61,3,78,28,57,84,77,75,90,26,65,80,52,79,92,66};
	int len = 16;
#endif
    	for (i = 0; i < len; i++)
    {
        insert_avl(&root, data[i]);
    }
    print_avl(root);

    printf("delete avl tree\n");
    for (i = 0; i < len; i++)
    {
	    printf("delete avl tree data : %4d\n", data[i]);
	    delete_avl(&root, data[i]);
	    print_avl(root);
    }
    print_avl(root);

    return 0;
}

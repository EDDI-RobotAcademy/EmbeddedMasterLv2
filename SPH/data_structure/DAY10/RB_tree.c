#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define Guide_Code 1
#define old 0
//#define new 1
//#define delete 1

#define ABS(x) ((x > 0) ? (x) : -(x))

#define RR 1
#define RL 2
#define LL 3
#define LR 4

typedef enum _color color;
enum _color
{
	red = 0,
	black
};

typedef struct _RedBlack rb;
struct _RedBlack
{
	int data;
	struct _RedBlack *left;
	struct _RedBlack *right;
	struct _RedBlack *parent;
	color color; 
};

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
        printf("%4d", data[i]);

        if (i % 7 == 6)
        {
            printf("\n");
        }
    }

    printf("\n");
}

void print_rb(rb *tree)
{
    rb *tmp = tree;

    if (tmp)
    {
        print_rb(tmp->left);

        printf("data = %4d\t", tmp->data);

		if(tmp->parent)
			printf("parent = %4d\t", tmp->parent->data);
		else
			printf("parent = None\t");

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

		if(tmp->color== red)
			printf("color = red\n");
		else
			printf("color = black\n");

        print_rb(tmp->right);
    }
}

rb *create_rb_node(void)
{
    rb *tmp = (rb *)malloc(sizeof(rb));

    if (!tmp)
    {
        printf("Failed malloc\n");
    }

    tmp->left = NULL;
    tmp->right = NULL;

    return tmp;
}

rb **find_tree_data(rb **root, int data)
{
    while(*root)
    {
        if((*root)->data < data)
            root = &(*root)->right;
        else if((*root)->data > data)
            root = &(*root)->left;
        else
            return root;
    }
    return NULL;
}

bool chk_double_red(rb **root, rb **child)
{
	if((*root)->color== red && (*child)->color == red)
		return true;
	else
		return false;
}

void left_rotation(rb **root)
{
	rb *parent = (*root)->parent;
	rb *me = *root;
	rb *child = (*root)->right;
	rb *grand_parent = parent->parent;

	//printf("grand_parent = %d\tparent = %d\tme = %d\tchild = %d\n", grand_parent->data, parent->data, me->data, child->data);
	me->parent = grand_parent;
	parent->parent = me;

	if(me->left)
		me->left->parent = parent;
	parent->right = me->left;

	me->left = parent;

	//조부모가 NULL == 부모가 root라면
	if(!grand_parent)
	{
		parent = me;
		parent->color= black;
		me->color= black;
		child->color= black;
	}
	else
	{
		if(grand_parent->data > me->data)
			grand_parent->left = me;
		else
			grand_parent->right = me;

		child->color= black;
	}
}

void right_rotation(rb **root)
{
	rb *parent = (*root)->parent;
	rb *me = *root;
	rb *child = (*root)->left;
	rb *grand_parent = (!parent->parent) ? NULL : parent->parent;

	//printf("grand_parent = %d\tparent = %d\tme = %d\tchild = %d\n", grand_parent->data, parent->data, me->data, child->data);

	me->parent = grand_parent;
	parent->parent = me;

	if(me->right)
		me->right->parent = parent;
	parent->left = me->right;

	me->right = parent;

	//조부모가 NULL == 부모가 root라면
	if(!grand_parent)
	{
		parent = me;
		parent->color= black;
		me->color= black;
		child->color= black;
	}
	else
	{
		if(grand_parent->data > me->data)
			grand_parent->left = me;
		else
			grand_parent->right = me;
		child->color= black;
	}
}

void re_struct_tree(rb **root, rb **brother, rb **child)
{
	rb *tmp = *root;
	rb *parent = (*root)->parent;

	//RR, RL case
	if((*root)->data > parent->data)
	{
		if((*child)->data < (*root)->data)
		{
			printf("RL case\n");

			assert((*child) != NULL);

			(*child)->parent = parent;
			parent = *child;

			if((*child)->right)
				(*child)->right->parent = tmp;
			tmp->left = (*child)->right;

			(*child)->right = tmp;
			*root = *child;

			left_rotation(child);
			printf("RL rotation clear\n");
		}
		else
		{
			printf("RR case\n");
			left_rotation(root);
			printf("RR rotation clear\n");
		}
	}
	//LL, LR case
	else
	{
		if((*child)->data > (*root)->data)
		{
			printf("LR case\n");

			assert((*child) != NULL);

			(*child)->parent = parent;
			parent = *child;

			if((*child)->left)
				(*child)->left->parent = tmp;
			tmp->right = (*child)->left;

			(*child)->left = tmp;
			*root = *child;

			right_rotation(child);
			printf("LR rotation clear\n");
		}
		else
		{
			printf("LL case\n");
			right_rotation(root);
			printf("LL rotation clear\n");
		}
	}
}

void re_color_tree(rb **root, rb **brother, rb **child)
{
	rb *parent = (*root)->parent;
	rb *grand_parent = parent->parent;

	if(!grand_parent)
	{
		(*root)->color= black;
		(*brother)->color= black;
	}
	else
	{
		parent->color= red;
		(*root)->color= black;
		(*brother)->color= black;
	}
}

rb *find_child(rb **root)
{
	rb *child;

	if(!(*root)->left && !(*root)->right)
		return NULL;
	else if(!(*root)->left)
		child = (*root)->right;
	else if(!(*root)->right)
		child = (*root)->left;
	else
		child = ((*root)->left->color== black) ? (*root)->right : (*root)->left;

	return child;
}

#ifdef old
void chk_rb_balance(rb **root) 
{
	rb *child;
	rb *brother;
	rb *parent;

	child = find_child(root);

	if(!child)
		return;
	parent = (*root)->parent;
	brother = (parent->data > (*root)->data) ? parent->right : parent->left;

#if 0
	if(brother->color == black)
		printf("node %d's brother %d's color is black\n", (*root)->data, brother->data);
	else
		printf("node %d's brother %d's color is red\n", (*root)->data, brother->data);
#endif

	bool res_double_red;
	res_double_red = chk_double_red(root, &child);

	switch(res_double_red)
	{
		case true:
			printf("double red detected on %d nodes\n", (*root)->data);
			if(!brother || brother->color == black)
			{
				printf("resturct!\n");
				re_struct_tree(root, &brother, &child);
			}

			if(brother && brother->color == red)
			{
				printf("re-color!\n");
				re_color_tree(root, &brother, &child);
			}
			break;
		case false:
			printf("double red undetected!\n");
			return;
	}
}
#endif

void insert_rb_data(rb **root, rb *parent, int data)
{
	rb *backup_papa;

	if(!(*root))
	{
		*root = create_rb_node();
		(*root)->data = data;
		(*root)->parent = parent;
		(*root)->color = (!parent) ? black : red;
		return;
	}

	backup_papa = *root;

	if((*root)->data > data)
	{
		insert_rb_data(&(*root)->left, backup_papa, data);
#ifdef old
		*root = (*root)->left;
#endif
	}
	else if((*root)->data < data)
	{
		insert_rb_data(&(*root)->right, backup_papa, data);
#ifdef old
		*root = (*root)->right;
#endif
	}

#ifdef old
	chk_rb_balance(root);

	if(!(*root)->parent)
		return;
	else
		*root = (*root)->parent;
#endif
}

int set_del_data_num(int len)
{
	int num;
	int i;

	reassin:
	num = rand() % len + 1;

	if(ABS(num) > len)
		goto reassin;
	else
		return num;
}

void get_rand_idx(int *arr, int len)
{
	int i, j;
	int idx;

	for(i = 0; i < len; i++)
	{
		reassign:
		idx = rand() % len;
		arr[i] = idx;

		for(j = 0; j < i; j++)
		{
			if(arr[i] == arr[j])
				goto reassign;
		}
	}
}

void random_del_node(rb **root, int *data, int len)
{
	int rand_idx = 0;
	int del_data_num = 0;
	int i;

	srand(time(NULL));

	del_data_num = set_del_data_num(len);
	printf("del_data_num = %d\n", del_data_num);

	int *idx = (int *)malloc(sizeof(int) * del_data_num);
	get_rand_idx(idx, del_data_num);

	for(i = 0; i < del_data_num; i++)
	{
		printf("delete data = %d\n", data[idx[i]]);

		//nr_RB_delete_node(root, data[idx[i]]);

	}
}

int main(void)
{
    rb *root = NULL;
    int i;

#ifdef Guide_Code
    int data[500];
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);

    for (i = 0; i < len; i++)
    {
		printf("insert processing!\n");
		printf("insert data = %d\n", data[i]);
		insert_rb_data(&root, NULL, data[i]);
		print_rb(root);
    }
#else
	int data[] = {500, 50, 1000, 100, 25, 750, 1250, 75, 125, 37, 12, 625, 875, 1125, 1375, 6, 30, 40, 45};
	//int data[] = {87, 58, 50,  34,   6,  57,  41,  43,  84,  95,   9,  62,  28,  2, 78,  92,  52,   5,  55,  49,  86};
	//int data[] = {72, 194, 173, 161, 133, 158, 200};
	//int data[] = {173, 133, 194, 72, 161, 158, 200};
	//int data[] = {80, 45, 200, 40, 50, 176, 280, 32, 44, 49, 55, 150, 180, 277, 290, 31, 33, 43, 47, 54, 149, 179, 275, 285, 29};
	//int data[] = {55, 32, 60, 31, 40, 58, 65, 29};
	//int data[] = {55, 31, 29};

	//simple LL case
	//int data[] = {500, 300, 200}
	//simple LR case
	//int data[] = {500, 300, 400};
	//simple RR case
	//int data[] = {500, 1000, 2000};
	//simple RL case
	//int data[] = {500, 1000, 700};

	int len = sizeof(data)/sizeof(int);
	for(i = 0; i < len; i++)
	{
		printf("insert processing!\n");
		printf("insert data = %d\n", data[i]);
		insert_rb_data(&root, NULL, data[i]);
		print_rb(root);
	}
#endif

	printf("insert clear!\n");
	print_rb(root);

#ifdef delete
	printf("\ndelete node\n");
	random_del_node(&root, data, len);
	printf("delete clear!\n");
	print_rb(root);
#endif

    return 0;
}

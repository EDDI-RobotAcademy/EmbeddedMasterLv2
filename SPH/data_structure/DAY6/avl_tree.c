#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define Guide_Code 1
//#define Debug 2

typedef struct _avl avl;
struct _avl
{
	int data;
	struct _avl *left;
	struct _avl *right;
	int level;
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

void print_avl(avl *tree)
{
    avl *tmp = tree;

    if (tmp)
    {
        print_avl(tmp->left);

        printf("data = %4d\t", tmp->data);

        if (tmp->left)
        {
            printf("left = %4d\t", tmp->left->data);
#if Debug
			printf("left address = %x", tmp->left);
#endif
        }
        else
        {
            printf("left = NULL\t");
        }

        if (tmp->right)
        {
            printf("right = %4d\t", tmp->right->data);
#if Debug
			printf("right address = %x\t", tmp->right);
#endif
        }
        else
        {
            printf("right = NULL\t");
        }

        printf("level = %2d\n", tmp->level);
#if Debug
		printf("address = %x\n", tmp);
#endif

        print_avl(tmp->right);
    }
}

avl *create_avl_node(void)
{
    avl *tmp = (avl *)malloc(sizeof(avl));

    if (!tmp)
    {
        printf("Failed malloc\n");
    }

    tmp->left = NULL;
    tmp->right = NULL;

    return tmp;
}

avl **find_tree_data(avl **root, int data)
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

void update_level(avl **root)
{
	int left_level;
	int right_level;

	//왼쪽, 오른쪽 자식노드가 둘 다 없는 경우
	if(!(*root)->left && !(*root)->right)
		return;
	//오른쪽 자식노드만 있는 경우
	else if(!(*root)->left && (*root)->right)
		(*root)->level = (*root)->right->level + 1;
	//왼쪽 자식노드만 있는 경우
	else if((*root)->left && !(*root)->right)
		(*root)->level = (*root)->left->level + 1;
	//둘 다 있거나 같은 경우
	else
		(*root)->level = ((*root)->left->level >= (*root)->right->level) ? (*root)->left->level+1 : (*root)->right->level+1;
}

int calc_balance_factor(avl **root)
{
	int factor;
	if(!(*root)->left && !(*root)->right)
	{
		factor = 0;
	}
	else if((*root)->left && !(*root)->right)
		factor = (*root)->left->level;
	else if(!(*root)->left && (*root)->right)
		factor = 0-(*root)->right->level;
	else
		factor = (*root)->left->level - (*root)->right->level;

	return factor;
}

void rotation(int factor, avl **root, int data)
{
	avl *high;
	avl *mid;
	avl *low;

	//트리가 오른쪽 불균형일 때
	if(factor < 0)
	{
		high = *root;
		mid = (*root)->right;

		//RR
		if((*root)->right->data < data)
		{
			low = (*root)->right->right;

			high->right = low->left;
			high->level = high->level + factor;

			*root = mid;

			(*root)->left = high;

			(*root)->right = low;

			update_level(&(*root)->right);
			update_level(root);


		}
		//RL
		else
		{
			low = (*root)->right->left;

			mid->left = low->right;

			low->right = mid;
			low->right->level--;

			(*root)->right = low;

			rotation(factor, root, (*root)->right->right->data);
		}
	}
	//트리가 왼쪽 불균형 일 때
	else
	{
		high = *root;
		mid = (*root)->left;
		//LL
		if((*root)->left->data > data)
		{
			low = (*root)->left->left;

			high->left = mid->right;
			high->level = high->level - factor;

			*root = mid;
			(*root)->right = high;

			update_level(root);
		}
		//LR
		else
		{
			low = (*root)->left->right;

			mid->right = low->left;

			low->left = mid;
			mid->level--;

			(*root)->left = low;

			rotation(factor, root, (*root)->left->left->data);
		}
	}
}

void adjust_balance(avl **root, int data)
{
	int factor = calc_balance_factor(root);
	//printf("Node %d's balance factor = %d\n", (*root)->data, factor);

	if(factor>=-1 && factor<=1)
		return;
	else
		rotation(factor, root, data);
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

int main(void)
{
    avl *root = NULL;
    int i;

#if Guide_Code
    int data[511] = { 0 };
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);

    for (i = 0; i < len; i++)
    {
        insert_avl(&root, data[i]);
    }

#else
#if Debug
	int data[3] = {34, 55, 57};
	for(i = 0; i < 3; i++)
	{
		insert_avl(&root, data[i]);
	}
#else
	int data[9] = {34, 17, 13, 12, 10, 55, 53, 54, 57};
    int len = sizeof(data) / sizeof(int);

	for (i = 0; i < len; i++)
    {
        insert_avl(&root, data[i]);
    }
#endif
#endif

	print_avl(root);

    return 0;
}

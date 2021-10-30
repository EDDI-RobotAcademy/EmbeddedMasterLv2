#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define	LEFT			-2
#define RIGHT			2

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

void update_level(avl **root)
{
    int left, right;

    right = (*root)->right ? (*root)->right->level : 0;
    left = (*root)->left ? (*root)->left->level : 0;

    if (right > left)
    {
        (*root)->level = right + 1;
        return;
    }

    (*root)->level = left + 1;
}

void ll_rotation(avl **root)
{
    printf("ll rotation\n");

    avl *parent = (*root)->left;

    (*root)->left = parent->right;
    parent->right = *root;
    update_level(root);

    (*root) = parent;
    update_level(root);
}

void rr_rotation(avl **root)
{
    printf("rr rotation\n");

    avl *parent = (*root)->right;

    (*root)->right = parent->left;
    parent->left = *root;
    update_level(root);

    (*root) = parent;
    update_level(root);
}

void lr_rotation(avl **root)
{
    avl *parent = (*root)->left;

    printf("root = %4d\tparent = %4d\n", (*root)->data, parent->data);

    if (parent->right->left)
    {
        parent->right->right = *root;
        *root = parent->right;
        parent->right = parent->right->left;

        (*root)->left = parent;
        (*root)->right->left = NULL;

        /*
        (*root)->left = parent->right;
        parent->right = parent->right->left;
        (*root)->left->left = parent;
        (*root)->left->right = (*root);
        *root = (*root)->left;
        (*root)->right->left = NULL;
         */

        update_level(&(*root)->right);
        update_level(&(*root)->left);
        update_level(root);
    }
    else if (parent->right->right)
    {
        (*root)->left = parent->right->right;
        parent->right->right = *root;
        (*root) = parent->right;

        (*root)->left = parent;
        (*root)->left->right = NULL;

        update_level(&(*root)->right);
        update_level(&(*root)->left);
        update_level(root);
    }
    else
    {
        printf("LR Here is the problem!\n");
        parent->right->left = parent;
        parent->right->right = *root;
        *root = parent->right;

        parent->right = NULL;
        (*root)->right->left = NULL;

        update_level(&(*root)->right);
        update_level(&(*root)->left);
        update_level(root);
    }
    //printf("root->data = %d\n", (*root)->data);
    //printf("uncle->data = %d\n", uncle->data);
    printf("lr rotation\n");
}

void rl_rotation(avl **root)
{
    avl *parent = (*root)->right;

    printf("root = %4d\tparent = %4d\n", (*root)->data, parent->data);

    if (parent->left->left)
    {
        (*root)->right = parent->left->left;
        parent->left->left = *root;
        *root = parent->left;

        (*root)->right = parent;
        parent->left = NULL;

        update_level(&(*root)->right);
        update_level(&(*root)->left);
        update_level(root);
    }
    else if (parent->left->right)
    {
        /*
        parent->right->right = *root;
        *root = parent->right;
        parent->right = parent->right->left;
        (*root)->left = parent;
        (*root)->right->left = NULL;
        */

        parent->left->left = *root;
        *root = parent->left;
        parent->left = parent->left->right;

        (*root)->right = parent;
        (*root)->left->right = NULL;

        update_level(&(*root)->right);
        update_level(&(*root)->left);
        update_level(root);
    }
    else
    {
        printf("RL Here is the problem!\n");
        parent->left->right = parent;
        parent->left->left = *root;
        *root = parent->left;

        parent->left = NULL;
        (*root)->left->right = NULL;

        update_level(&(*root)->right);
        update_level(&(*root)->left);
        update_level(root);
    }

    printf("rl rotation\n");
}

void rotation(int factor, avl **root, int data)
{
    printf("balance factor = %3d\tnode data = %3d\t in data = %3d\n", factor, (*root)->data, data);

    switch (factor)
    {
        case LEFT:
            if ((*root)->left->data < data)
            {
                lr_rotation(root);
            }
            else
            {
                ll_rotation(root);
            }

            break;

        case RIGHT:
            if ((*root)->right->data > data)
            {
                rl_rotation(root);
            }
            else
            {
                rr_rotation(root);
            }

            break;
    }
}

void adjust_balance(avl **root, int data)
{
    int left, right, balance_factor;

    left = (*root)->left ? (*root)->left->level : 0;
    right = (*root)->right ? (*root)->right->level : 0;

    balance_factor = right - left;
    if (abs(balance_factor) > 1)
    {
        printf("Rotation Occurrence\n");
        rotation(balance_factor, root, data);
    }
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
    int data[511] = { 0 };
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);

    for (i = 0; i < len; i++)
    {
        insert_avl(&root, data[i]);
    }

    print_avl(root);

    return 0;
}

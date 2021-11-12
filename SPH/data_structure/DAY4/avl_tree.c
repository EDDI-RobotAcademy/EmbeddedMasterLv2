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

void delete_tree_node(avl **root, int data)
{
    avl *tmp;
    root = find_tree_data(root, data);

    tmp = *root;

    if (!(*root)->left)
    {
        *root = (*root)->right;
    }
    else if (!(*root)->right)
    {
        *root = (*root)->left;
    }

    free(tmp);
}

int proc_left_max(avl **root)
{   
    int max;
    avl *tmp = (*root)->left; //삭제하려는 노드의 왼쪽 노드를 백업
    
    //삭제하려는 노드의 왼쪽노드에 오른쪽 값이 있다면 
    //오른쪽 노드에서 최대값을 찾는다.
    if(tmp->right)
    {
        while(tmp)
        {
            max = tmp->data;
            tmp = tmp->right;
        }
    }
    else
    {
        max = tmp->data;
    }
    
    return max;
}

int proc_right_min(avl **root)
{
    int min;
    avl *tmp = (*root)->right;

    if(tmp->left)
    {
        while(tmp)
        {
            min = tmp->data;
            tmp = tmp->left;
        }
    }
    else
    {
        min = tmp->data;
    }

    return min;
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
	//둘 다 있는 경우
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
	avl *tmp = *root;
	int tmp_data;

	if(factor == -1 || factor == 0 || factor == 1)
		return;
	else
	{
		//현재 루트의 레벨이 오른쪽으로 치우친 경우
		if(factor < 0)
		{
			//오른쪽->오른쪽
			if((*root)->right->data < data)
			{
				//1단계 : 현재 루트의 왼쪽에 새 노드를 생성 후 현재 루트의 값을 저장
				(*root)->left = create_avl_node();
				(*root)->left->data = tmp->data;
				(*root)->left->level = 1;
				//2단계 : 현재 루트에 오른쪽 노드의 값을 저장
				(*root)->data = tmp->right->data;
				//3단계 : 현재 루트에 새로 추가되는 값을 저장
				(*root)->right->data = data;
				//4단계 : 새로 추가되는 값이 저장된 기존 노드 삭제
				delete_tree_node(&(*root)->right->right, data);
				//5단계 : 현재 루트, 하위 노드의 레벨 다운 
				(*root)->level = (*root)->level - 1;
				(*root)->right->level = (*root)->right->level - 1;
			}
			//오른쪽->왼쪽 : 오른쪽->오른쪽의 형태로 회전한 후 밸런싱 진행
			else
			{
				if((*root)->left)
				{
					int right_min = proc_right_min(root);

					(*root)->left->left = create_avl_node();
					(*root)->left->left->data = (*root)->left->data;
					(*root)->left->left->level = 1;
					(*root)->left->data = tmp->data;
					(*root)->data = right_min;
					delete_tree_node(&(*root)->right, right_min);
					update_level(&(*root)->left);
					//factor = calc_balance_factor(root);
					rotation(factor, &(*root)->right, data);
				}
				else
				{
					//1단계 : 오른쪽->왼쪽의 경우 현재 루트의 오른쪽 자식노드의 값을 백업해야함
					//        tmp에 백업된 값은 새로 추가되는 값으로 변경되기 때문에 tmp의 값을 사용하면 안됨
					tmp_data = (*root)->right->data; 
					//2단계 : 새로 추가되는 값을 오른쪽 자식노드에 저장
					(*root)->right->data = data;
					//3단계 : 현재 루트의 오른쪽 자식노드의 오른쪽에 새 노드 추가, 이전 루트의 오른쪽 자식노드 값을 저장
					(*root)->right->right = create_avl_node();
					(*root)->right->right->data = tmp_data;
					(*root)->right->right->level = 1;
					//4단계 : 새로 추가되는 값이 저장된 기존 노드 삭제
					delete_tree_node(&tmp->right->left, data);
					//5단계 : 오른쪽->오른쪽 형태의 밸런싱 진행
					rotation(factor, root, tmp_data);
				}
			}
		}
		//현재 루트의 레벨이 왼쪽으로 치우친 경우
		else
		{
			//왼쪽 노드 -> 왼쪽 노드인 경우
			if((*root)->left->data > data)
			{
				if((*root)->right)
				{
					int left_max = proc_left_max(root);

					(*root)->right->right = create_avl_node();
					(*root)->right->right->data = tmp->right->data;
					(*root)->right->right->level = 1;
					(*root)->right->data = tmp->data;
					(*root)->data = left_max;
					delete_tree_node(&(*root)->left, left_max);
					update_level(&(*root)->right);
					factor = calc_balance_factor(root);
					rotation(factor, &(*root)->left, data);
				}
				else
				{
					(*root)->right = create_avl_node();
					(*root)->right->data = tmp->data;
					(*root)->right->level = 1;
					(*root)->data = tmp->left->data;
					(*root)->left->data = data;
					delete_tree_node(&tmp->left->left, data);
					(*root)->level = (*root)->level - 1;
					(*root)->left->level = tmp->left->level - 1;
				}
			}
			//왼쪽 노드 -> 오른쪽 노드인 경우
			else
			{
				(*root)->left->data = data;
				(*root)->left = create_avl_node();
				(*root)->left->data = tmp->left->data;
				(*root)->left->level = 1;
				delete_tree_node(&(*root)->left->right, data);
				rotation(factor, root, data);
			}
		}
	}
}

void adjust_balance(avl **root, int data)
{
	int factor = calc_balance_factor(root);
	printf("Node %d's balance factor = %d\n", (*root)->data, factor);

	//rotation(factor, root, data);
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
	return;
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

#if Guide_Code
    int data[511] = { 0 };
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	//print_arr(data, len);

    for (i = 0; i < len; i++)
    {
        insert_avl(&root, data[i]);
    }

#else
#if Debug
	int data[7] = {34, 17, 55, 19, 15, 13, 10};
	for(i = 0; i < 7; i++)
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

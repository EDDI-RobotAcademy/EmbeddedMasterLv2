#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

//#define Guide_Code 1
//#define old 0
#define new 1
//#define delete 1

#define ABS(x) ((x > 0) ? (x) : -(x))

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
	if((*root)->color == red && (*child)->color == red)
		return true;
	else
		return false;
}

void left_rotation(rb **top_root, rb **root)
{
	rb *parent = (*root)->parent;
	rb *me = *root;
	rb *child = (*root)->right;
	rb *grand_parent = parent->parent;

	me->parent = grand_parent;
	parent->parent = me;

	if(me->left)
		me->left->parent = parent;
	parent->right = me->left;

	me->left = parent;

	//조부모가 NULL == 부모가 root라면
	if(!grand_parent)
		*top_root = me;
	else if(grand_parent->data > me->data)
		grand_parent->left = me;
	else
		grand_parent->right = me;
}

void right_rotation(rb **top_root, rb **root)
{
	rb *parent = (*root)->parent;
	rb *me = *root;
	rb *child = (*root)->left;
	rb *grand_parent = (!parent->parent) ? NULL : parent->parent;

	me->parent = grand_parent;
	parent->parent = me;

	if(me->right)
		me->right->parent = parent;
	parent->left = me->right;

	me->right = parent;

	//조부모가 NULL == 부모가 root라면
	if(!grand_parent)
		*top_root = me;
	else if(grand_parent->data > me->data)
		grand_parent->left = me;
	else
		grand_parent->right = me;
}

rb *find_child(rb **root)
{
	rb *left = (*root)->left;
	rb *right = (*root)->right;

	if(!left && !right)
		return NULL;
	else if(!left)
		return right;
	else if(!right)
		return left;
	else
		return (left->color == black) ? right : left;
}

/***************TODO***************/
/*
#top_root = main문의 root, root = 자식노드가 red인 노드
#지역변수 : me = 자식노드가 red인 노드, parent = me의 부모, brother = me의 형제, child = me의 자식
1. child 찾기 
2. child 또는 parent가 NULL이면 return
3. brother 찾기
4. me와 child가 red인지 확인
	4.1 me와 child가 red가 아니라면 return
	4.2 me와 child가 red라면 balancing 시작
5. 형제노드가 black 또는 NULL인지 red인지 확인
	5.1 형제노드가 black 또는 NULL이면 restruct실행
		5.1.1 me > parent 우측편향
			5.1.1.1 me > child = RL : child를 parent의 우측에 연결한다 -> child를 기준으로 left rotation 실행
			5.1.1.2 me < child = RR : left rotation 실행
			5.1.1.3 me의 parent가 top_root가 아니라면 회전 후 me의 오른쪽 자식 색상을 black으로 변경
		5.1.2 me < parent 좌측편향
			5.1.2.1 me < child = LR : child를 parent의 좌측에 연결한다 -> child를 기준으로 right rotation 실행
			5.1.2.2 me > child = LL : right rotation 실행 
			5.1.2.3 me의 parent가 top_root가 아니라면 회전 후 me의 왼쪽 자식 색상을 black으로 변경
		5.1.3 me의 parent가 top_root라면 회전 후 me와 me의 좌우측 색상을 black으로 변경
	5.2 형제노드가 red이면 re-color실행
		5.2.1 parent가 top_root이면 top_root의 좌,우 black으로 변경
		5.2.2 parent가 top_root가 아니면 parent = red, parent 좌우 black
*/
void chk_rb_balance(rb **top_root, rb **root) 
{
	rb *parent;
	rb *me;
	rb *brother;
	rb *child;

	me = *root;
	parent = ((*root)->parent) ? (*root)->parent : NULL;
	//1
	child = find_child(root);
	//2
	if(!parent || !child)
		return;
	//3
	brother = (parent->data > (*root)->data) ? parent->right : parent->left;
	//4
	bool res_double_red;
	res_double_red = chk_double_red(root, &child);
	switch(res_double_red)
	{
		case true:
			printf("double red = %d\n", (*root)->data);
			//5
			//5.1
			if(!brother || brother->color == black)
			{
				//5.1.1
				if(me->data > parent->data)
				{
					//5.1.1.1
					if(me->data > child->data)
					{
						printf("RL case\n");

						child->parent = parent;
						me->parent = child;

						if(child->right)
							child->right->parent = me;

						me->left = child->right;

						child->right = me;
						//*root = child;

						left_rotation(top_root, &child);
						//rotation 후 me가 child로 변경되므로
						me = child;
					}
					//5.1.1.2
					else
					{
						left_rotation(top_root, root);
					}
					//5.1.1.3
					me->right->color = black;
				}
				//5.1.2
				else
				{
					//5.1.2.1
					if(me->data < child->data)
					{
						printf("LR case\n");

						child->parent = parent;
						me->parent = child;

						if(child->left)
							child->left->parent = me;

						me->right = child->left;

						child->left = me;
						//*root = child;

						right_rotation(top_root, &child);
						//rotation후 me가 child로 변경된다
						me = child;
						printf("LR rotation clear\n");
					}
					//5.1.2.2
					else
					{
						right_rotation(top_root, root);
					}
					//5.1.2.3
					me->left->color = black;
				}

				//5.1.3
				if(!me->parent)
				{
					me->color= black;
					me->right->color = black;
					me->left->color= black;
				}
			}
			//5.2
			else
			{
				//5.2.1
				if(parent->data == (*top_root)->data)
				{
					me->color= black;
					brother->color= black;
				}
				//5.2.2
				else
				{
					parent->color= red;
					me->color= black;
					brother->color= black;
				}
			}
			break;
		case false:
			printf("double red undetected\n");
			return;
	}
}

void insert_rb_data(rb **top_root, rb **root, rb *parent, int data)
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
		insert_rb_data(top_root, &(*root)->left, backup_papa, data);
	}
	else if((*root)->data < data)
	{
		insert_rb_data(top_root, &(*root)->right, backup_papa, data);
	}

	chk_rb_balance(top_root, root);
}

rb **find_left_max(rb **root)
{
	rb **left_max = root;

	if(!(*root)->right)
		return left_max; 
	else
		left_max = find_left_max(&(*root)->right);

	return left_max;
}

void chg_node(rb **root)
{
	printf("노드 삭제 시작!\n");
	rb *tmp = *root;

	if((*root)->left)
	{
		(*root)->right->parent = (*root)->parent;
		*root = (*root)->right;
	}
	else if((*root)->right)
	{
		(*root)->left->parent = (*root)->parent;
		*root = (*root)->left;
	}
	else
		*root = NULL;

	free(tmp);
}

rb **find_brother(rb **root)
{
	rb *parent = (*root)->parent;

	if(parent->data > (*root)->data)
		return &parent->right;
	else if(parent->data < (*root)->data)
		return &parent->left;
	else
		return NULL;
}

//black노드의 자식 특징
//1. black인 경우 : rb 규칙상 1개만 가질 수 없다.
//2. red인 경우 : rb 규칙상 1개만 가질 수 있다.
bool chk_brother_re_color(rb **root)
{
	printf("형제의 자식노드 확인 시작!\n");
	rb *left, *right;

	left = (*root)->left;
	right = (*root)->right;

	if(!left && !right) 
		return true;
	else if(left->color == black && right->color == black)
		return true;
	else
		return false;
}

void balance_rb_re_chk(rb **top_root, rb **brother)
{
	printf("밸런스 재확인!\n");
	printf("brother = %d\n", (*brother)->data);
	if(!(*brother))
		return;
	else if((*brother)->color == black)
	{
		rb *cursor = *brother;

		while(chk_brother_re_color(&cursor))
		{
			cursor->color = red;
			if(cursor->parent == *top_root)
				break;
			cursor = *find_brother(&cursor->parent);
			printf("cursor = %d\n", cursor->data);
		}
#if 0
		if(chk_brother_re_color)
		{
			printf("형제의 색상 재배치 시작\n");

			rb *loop = *brother;
			rb *parent = loop->parent;

			while((parent != *top_root) && (loop->color != red))
			{
				loop->color = red;
				loop = *find_brother(&parent);
				parent = loop->parent;
			}
		}
		else
		{
		}
#endif
	}
	else
	{
	}
}

/*TODO*/
/*
# 트리 노드 삭제에서 홑 노드 경우를 제외하고 메모리 해제가 되는 노드는 왼쪽 최대(또는 오른쪽 최소)노드이다
# 왼쪽 최대 또는 오른쪽 최대인 경우 black이면 트리 삽입규칙상 자식이 1개인 경우는 자식이 red만 존재
                                    red이면 트리 삽입규칙상 1개의 자식을 가질 수 없다
1. 홑 노드인 경우 
	1-1 노드가 red인 경우 : 삭제해도 RB트리의 규칙을 만족한다.
	1-2 노드가 black인 경우 : 삭제하면 black노드 개수 규칙이 깨짐 
	1-2-1 형제노드가 black : 형제노드를 기준으로 left or right rotation 실행
	1-2-2 형제노드가 red 
	1-2-2-1 형제노드를 기준으로 left or right rotation 실행
	1-2-2-2 회전 후 형제노드와 부모노드의 색을 서로 바꾼다
	1-2-2-3 형제노드의 자식을 기준으로 옮기고 balance chk를 진행한다

2. 홑 노드가 아닌 경우
	2-1 노드가 red인 경우 : 자식노드와 색상과 위치를 서로 바꾼 후 삭제
	2-2 노드가 black인 경우 : 자식노드는 red이므로 색상과 위치를 서로 바꾼 후 삭제 
*/
void delete_rb_data(rb **top_root, rb **root, int data)
{
	rb **del_brother;
	rb **del_node;
	color del_color = red;

	if((*root)->data > data)
		delete_rb_data(top_root, &(*root)->left, data);
	else if((*root)->data < data)
		delete_rb_data(top_root, &(*root)->right, data);
	else if((*root)->left && (*root)->right)
	{
		del_node = find_left_max(&(*root)->left);
		del_brother = find_brother(del_node);
		del_color = (*del_node)->color;

		printf("brother = %d\n", (*del_brother)->data);

		(*root)->data = (*del_node)->data;

		//left max가 black인 경우 홑 노드이거나 왼쪽 자식이 red인 1개인 경우이다.
		//left max가 red인 경우 홑 노드만 존재한다.
		if((*del_node)->left)
			(*del_node)->left->color = del_color;

		chg_node(del_node);
	}
	else
	{
		printf("홑노드 또는 자식이 1개인 노드 삭제!\n");
		del_node = root;
		del_brother = find_brother(del_node);
		del_color = (*del_node)->color;
		chg_node(del_node);
		printf("노드 삭제 완료!\n");
	}

	if(del_color == black)
	{
		printf("삭제 밸런싱 시작!\n");
		assert(*del_brother != NULL);
		balance_rb_re_chk(top_root, del_brother);
		printf("삭제 밸런싱 완료!\n");
	}
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
    int data[5];
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);

    for (i = 0; i < len; i++)
    {
		//printf("insert processing!\n");
		//printf("insert data = %d\n", data[i]);
#ifdef new
		insert_rb_data(&root, &root, NULL, data[i]);
#endif
		//print_rb(root);
    }
#else
	//int data[] = {500, 50, 1000, 100, 25, 750, 1250, 75, 125, 37, 12, 625, 875, 1125, 1375, 6, 30, 40, 45};
	//int data[] = {87, 58, 50,  34,   6,  57,  41,  43,  84,  95,   9,  62,  28,  2, 78,  92,  52,   5,  55,  49,  86};
	//int data[] = {72, 194, 173, 161, 133, 158, 200};
	//int data[] = {173, 133, 194, 72, 161, 158, 200};
	//int data[] = {80, 45, 200, 40, 50, 176, 280, 32, 44, 49, 55, 150, 180, 277, 290, 31, 33, 43, 47, 54, 149, 179, 275, 285, 29};
	//int data[] = {55, 32, 60, 31, 40, 58, 65, 29};
	//int data[] = {55, 31, 29};
	//int data[] = {500, 200, 1000, 100, 300, 450, 435};
	int data[] = {6, 48, 17, 47, 13, 49, 50, 25, 27, 29, 46, 33, 19, 5, 44, 10, 32, 30, 23, 26, 15, 12, 38, 20, 18, 24, 4, 2,
				  43, 9, 7, 37, 45, 40, 16, 42, 31, 3, 11, 8, 14, 36, 35, 22, 34, 41, 39, 21, 28, 1};
	//delete
	//case 1
	//int data[] = {35, 22, 55, 57};
	//case 2
	//int data[] = {55, 35, 75, 95, 65, 150, 165};
	//case 3
	//int data[] = {55, 35, 75, 95, 105, 205, 305, 405, 505, 605, 705, 805, 905, 1005, 1015};
	//case 4
	//int data[] = {55, 35, 75, 95, 105, 205, 305, 405, 505, 605, 705};

	//simple LL case
	//int data[] = {500, 300, 200};
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
#ifdef new
		insert_rb_data(&root, &root, NULL, data[i]);
#endif
		print_rb(root);
	}
#endif

	printf("insert clear!\n");
	print_rb(root);

	printf("\ndelete node!\n");
	//case 1
	//delete_rb_data(&root, &root, 13);
	//case 2
	//delete_rb_data(&root, &root, 22);
	//case 3
	//delete_rb_data(&root, &root, 55);
	//case 4
	//delete_rb_data(&root, &root, 55);

	printf("delete clear!\n");
	print_rb(root);

#ifdef delete
	printf("\ndelete node\n");
	random_del_node(&root, data, len);
	printf("delete clear!\n");
	print_rb(root);
#endif

    return 0;
}

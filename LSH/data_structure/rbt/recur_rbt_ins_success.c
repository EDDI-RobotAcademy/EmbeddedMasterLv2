#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct _rbt rbt;
struct _rbt
{
	int data;
	struct _rbt *left;
	struct _rbt *right;
	struct _rbt *parent;
	int color;
};

rbt *nil;

#define BLACK		3
#define RED			7

#define LEFT		1
#define RIGHT		2
#define UNKNOWN		0

#define ABS(x)		((x > 0) ? (x) : -(x))

#define LL			-2
#define RR			2

#define LR			-3
#define RL			3

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
        printf("%4d,", data[i]);

        if (i % 10 == 9)
        {
            printf("\n");
        }
    }

    printf("\n");
}

rbt *create_rbt_node(void)
{
	rbt *tmp;

	tmp = (rbt *)malloc(sizeof(rbt));
	tmp->left = nil;
	tmp->right = nil;
	tmp->parent = nil;
	tmp->color = RED;

	return tmp;
}

bool check_adjacent_node_color(rbt **root)
{
	if ((*root)->color == RED)
	{
		if ((*root)->parent->color == RED)
			return true;
		else
			return false;
	}
	else
		return false;
}

int check_left_right_bias(rbt **root)
{
	if ((*root)->parent == (*root)->parent->parent->left)
		return LEFT;
	else
		return RIGHT;
}

rbt *get_uncle_node(rbt **root, int direction)
{
	if (direction == RIGHT)
		return (*root)->parent->parent->right;
	else
		return (*root)->parent->parent->left;
}

void change_color(rbt **left, rbt **right, int color)
{
	(*left)->color = BLACK;
	(*right)->color = BLACK;
}

bool check_me(rbt **root, int direction)
{
	if (direction == LEFT)
	{
		if ((*root)->parent->left == *root)
			return true;
		else
			return false;
	}
	else if (direction == RIGHT)
	{
		if ((*root)->parent->right == *root)
			return true;
		else
			return false;
	}
}

void right_rotation(rbt *root)
{
	printf("우회전\n");

	rbt *left = root->left;

	root->left = left->right;

	if (left->right != nil)
		left->right->parent = root;

	left->parent = root->parent;

	if (root->parent->left == root)
		root->parent->left = left;
	else
		root->parent->right = left;

	left->right = root;
	root->parent = left;
}

void left_rotation(rbt *grand_parent)
{
	printf("좌회전\n");

	rbt *parent = grand_parent->right;

	grand_parent->right = parent->left;

	if (parent->left != nil)
		parent->left->parent = grand_parent;

	parent->parent = grand_parent->parent;

	if (grand_parent == grand_parent->parent->left)
		grand_parent->parent->left = parent;
	else
		grand_parent->parent->right = parent;

	parent->left = grand_parent;
	grand_parent->parent = parent;
}

void recur_insert_rbt_data(rbt **root, rbt *parent, int data)
{
	int real_root_flag = 0;
	rbt **real_root = root;
	rbt *backup;
	rbt *uncle;

	if (*root != nil && (*root)->parent == nil)
		real_root_flag = 1;

	if (*root == nil)
	{
		*root = create_rbt_node();
		(*root)->data = data;
		(*root)->parent = parent;
		goto rbt_condition_handler;
	}
	else if ((*root)->data > data)
		recur_insert_rbt_data(&(*root)->left, *root, data);
	else if ((*root)->data < data)
		recur_insert_rbt_data(&(*root)->right, *root, data);

rbt_condition_handler:

	//printf("data: %d\n", data);

	// 1 & 2
	if (check_adjacent_node_color(root))
	{
		//printf("부모 노드가 빨강색이므로 색상을 변경하거나 회전을 수행해야한다!!!\n");

		backup = *root;

		// 3
		if (check_left_right_bias(root) == LEFT)
		{
			//printf("좌측 편향\n");

			// 4: 삼촌은 오른쪽
			uncle = get_uncle_node(root, RIGHT);

			// 5
			if (uncle->color == RED)
			{
				(*root)->parent->color = BLACK;
				uncle->color = BLACK;

				// 6
				(*root)->parent->parent->color = RED;
			}
			// 7
			else if (uncle->color == BLACK)
			{
				if (check_me(root, RIGHT))
				{
					//printf("나는 오른쪽에 있다!\n");
					backup = (*root)->parent;

					// 8
					left_rotation(backup);

					// 재귀 방식이므로 여기에서 별도로 하위 처리를 다시 진행함
					backup->parent->color = BLACK;
					backup->parent->parent->color = RED;

					right_rotation(backup->parent->parent);
				}
				else
				{
					// 9
					backup->parent->color = BLACK;
					backup->parent->parent->color = RED;

					// 10
					right_rotation(backup->parent->parent);
				}
			}
		}
		else
		{
			//printf("우측 편향\n");

			// 11: 삼촌은 왼쪽
			uncle = get_uncle_node(root, LEFT);

			// 12
			if (uncle->color == RED)
			{
				(*root)->parent->color = BLACK;
				uncle->color = BLACK;
				// 13
#if 0
				if ((*root)->parent->parent->parent != nil)
					(*root)->parent->parent->color = RED;
#endif
				(*root)->parent->parent->color = RED;
			}
			// 14
			else if (uncle->color == BLACK)
			{
				if (check_me(root, LEFT))
				{
					//printf("나는 왼쪽에 있다!\n");

					backup = (*root)->parent;
					// 15
					right_rotation(backup);

					// 재귀 방식이므로 여기에서 별도로 하위 처리를 다시 진행함
					backup->parent->color = BLACK;
					backup->parent->parent->color = RED;

					left_rotation(backup->parent->parent);
				}
				else
				{
					// 16 
					//(*root)->parent->color = BLACK;
					backup->parent->color = BLACK;
#if 0
					if ((*root)->parent->parent->parent != nil)
						(*root)->parent->parent->color = RED;
#endif
					//(*root)->parent->parent->color = RED;
					backup->parent->parent->color = RED;

					// 17
					//left_rotation((*root)->parent->parent);
					left_rotation(backup->parent->parent);
				}
			}
		}
	}
	else
	{
		//printf("부모 노드는 검정색이다\n");
	}

#if 0
	1) 현재 노드가 빨강인지 검사한다.
	2) 부모 노드에 빨강이 인접해있는지 검사한다.
	3) 회전을 수행하기 위해 빨강이 좌측/우측으로 기울었는지 판정한다.

	4) 좌측으로 기운 경우 삼촌의 색상이 빨강인지 검정인지 판정한다.
	5) 삼촌이 빨강이라면 부모와 삼촌을 검정으로 바꾼다.
	6) 조부모는 빨강으로 바꾼다.
	7) 삼촌이 검정이라면 자신이 오른쪽에 있는지 판정한다.
	8) 오른쪽에 있다면 L 회전 수행

	9) 부모의 색상을 검정으로 바꾸고 조부모는 빨강으로 변경한다.
	10) 그리고 R 회전을 수행한다.

	11) 우측으로 기운 경우에도 삼촌의 색상을 판정한다.
	12) 삼촌이 빨강이라면 부모와 삼촌을 검정으로 바꾼다.
	13) 조부모는 빨강으로 바꾼다.
	14) 삼촌이 검정이라면 자신이 왼쪽에 있는지 판정한다.
	15) 왼쪽에 있다면 R 회전을 수행한다.

	16) 부모를 검정색으로, 조부모는 빨강색으로 변경한다.
	17) L 회전을 수행한다.

	18) 모든 처리 이후에 root 노드는 검정으로 변경한다.
#endif
	if (real_root_flag)
	{
		while ((*root)->parent != nil)
		{
			root = &(*root)->parent;
		}

		*real_root = *root;
	}

	// 18
	if ((*root)->parent == nil)
		(*root)->color = BLACK;
}

void print_tree(rbt *root)
{
	if (root != nil)
	{
#if 0
		loop_cnt++;
		printf("cnt = %d,\tdata = %d,\tlevel = %d,\t", loop_cnt, root->data, root->level);
#else
		printf("data = %d,\tcolor = %s,\t", root->data, root->color == RED ? "Red" : "Black");
#endif

		if(root->parent)
			printf("parent = %d,\t", root->parent->data);
		else
			printf("parent = NULL,\t");

		if(root->left)
			printf("left = %d,\t", root->left->data);
		else
			printf("left = NULL,\t");

		if(root->right)
			printf("right = %d\n", root->right->data);
		else
			printf("right = NULL\n");

		print_tree(root->left);
		print_tree(root->right);
	}
}

#if 1
void chg_node(rbt **root)
{
    rbt *tmp = *root;

    if ((*root)->right)
    {
        (*root) = (*root)->right;
    }
    else
    {
        (*root) = (*root)->left;
    }

    free(tmp);
}

void find_right_min(rbt **root, int *min)
{
    if ((*root)->left)
    {
        find_right_min(&(*root)->left, min);
    }
    else
    {
        *min = (*root)->data;
		//*color = (*root)->color;
        chg_node(root);
    }
}
#endif

#if 0
void rbt_delete_post_process(rbt *)
{
}
#endif

void recur_delete_rbt_data(rbt **root, rbt *parent, int data)
{
	int min, color;

	// 1
	if ((*root)->data > data)
	{
		recur_delete_rbt_data(&(*root)->left, *root, data);
	}
	else if ((*root)->data < data)
	{
		recur_delete_rbt_data(&(*root)->right, *root, data);
	}
	// 3
	else if ((*root)->left && (*root)->right)
	{
		printf("오른쪽에서 최소값을 찾는다!\n");

		//find_right_min(&(*root)->right, &min, &color);
		printf("right min = %d, color = %s\n", min, color == RED ? "Red" : "Black");
		(*root)->data = min;
		(*root)->color = color;

#if 0
		if (color == BLACK)
		{
			rbt_delete_post_process();
		}
#endif
	}
	// 2
	else
	{
		printf("There are one leaf node or not\n");
		chg_node(root);
	}

#if 0
	1) 재귀호출을 통해 먼저 삭제하고자 하는 데이터를 찾는다.
	2) 찾은 노드가 잎사귀 혹은 단일 자식인지 여부를 검사한다.
	3) 양쪽 자식인지 여부를 판정한다.

	4) 잎사귀의 경우 successor(후임)이 없고 nil은 BLACK이다.
	   자신이 왼쪽에 있는지 오른쪽에 있는지 판별한다.
	5) 형제가 있는지 살펴본다.
	6) 형제의 색상이 빨강인지 판별한다.
	7) 빨강이라면 형제의 색상을 검정으로 변경한다.
	8) 부모의 색상을 빨강으로 변경한다.
	9) 오른쪽으로 회전한다.
	10) 형제 노드를 갱신한다(회전을 했으므로)

	11) 형제의 오른쪽과 왼쪽이 모두 검정인지 판별한다.
	12) 형제의 색상을 빨강으로 변경한다.

	13) 만약 형제중 오른쪽 혹은 왼쪽이 검정이 아니라면 
#endif
}

void create_rbt_nil(void)
{
	nil = (rbt *)malloc(sizeof(rbt));
	nil->left = nil;
	nil->right = nil;
	nil->parent = nil;
	nil->color = BLACK;
}

int main(void)
{
	int i;
	rbt *root;

	// 이중 회전 발생 데이터 셋
	//int data[] = { 400, 300, 500, 200, 350, 450, 600, 100, 250, 325, 375, 425, 475, 650, 50, 150, 225, 275, 310, 340, 360, 390, 410, 210 };

	// 2^14 - 1 = 2^0 + 2^1 + ... + 2^13: pass
	//int data[16384] = { 6, 3, 8, 1, 5, 7, 9, 2, 4, 10 };
	//int del_data[16384] = { 3, 5, 1, 7 };

#if 0
	/* First Set */
	int data[16384] = {
		1, 2, 3, 4, 5
	};
	int del_data[16384] = { 12, 19, 6, 14, 20, 9 };
#endif

#if 0
	/* Second Set */
	int data[16384] = {
		5, 4, 3, 2, 1
	};
	int del_data[16384] = { 8, 13, 20, 6, 2, 16, 19, 14, 18, 12, 15, 7 };
#endif

#if 0
	/* Third Set */
	int data[16384] = {
		9, 13, 16, 19, 8, 10, 12, 14, 18, 20
	};
	int del_data[16384] = {
		10,   1,   6,   5,   7,   8,   9,   4,   2,   3
	};
#endif

#if 1
	/* Fourth Set */
	int data[16384] = {
		22, 13, 4, 25, 12, 29, 18, 24, 6, 17
	};
	int del_data[16384] = {
		15, 5, 18, 16, 10, 9, 14, 7
	};
#endif

#if 0
	/* Fifth Set */
	int data[16384] = {
		11, 6, 17, 2, 8, 15, 19, 1, 4, 7,
		9, 13, 16, 18, 20, 3, 5, 10, 12, 14
	};
	int del_data[16384] = {
		1, 8
	};
#endif

#if 0
	/* Sixth Set */
	int data[16384] = {
		11, 7, 17, 3, 9, 14, 19, 2, 5, 8,
		10, 12, 16, 18, 20, 1, 4, 6, 13, 15
	};
	int del_data[16384] = {
		16, 18, 5, 14, 15
	};
#endif

#if 0
	/* 7th Set */
	int data[16384] = {
		9, 6, 15, 3, 7, 11, 17, 2, 4, 8,
		10, 13, 16, 19, 1, 5, 12, 14, 18, 20
	};
	int del_data[16384] = {
		16, 2, 10, 12, 13
	};
#endif

#if 0
	/* 8th Set */
	int data[16384] = {
		91, 13, 62, 14, 98, 29, 11, 3, 43, 81
	};
	int del_data[16384] = {
	};
#endif

#if 0
	/* 9th Set */
	int data[16384] = {
		88, 75, 6
	};
	int del_data[16384] = {
	};
#endif

#if 0
	/* 10th Set */
	int data[16384] = {
		11, 97, 12
	};
	int del_data[16384] = {
	};
#endif

#if 0
	/* 11th Set */
	int data[16384] = {
		13,  20,   6,  17,   8,  11,  10,   9,   3,  19,
   2,   1,  12,  15,   5,  14,  18,   4,  16,   7
	};
	int del_data[16384] = {
		19,  11,   9,  18,   2,  17,   3,   1,  16,   5,
   7,  14,  20,   8,   6,  15,   4,  10,  13,  12
	};
#endif

#if 0
	/* 11th Set */
	int data[16384] = {
		2,  19,   1,  13,  17,  16,  15,   5,   8,   4,
   7,  10,  18,   3,  20,  11,  14,   9,   6,  12
	};
	int del_data[16384] = {
		15,   9,  20,   2,   8,   6,  18,   5,  16,  17,
   7,  10,  11,   4,  14,  13,  12,  19,   3,   1
	};
#endif

#if 0
	/* 12th Set */
	int data[16384] = {
		11,  17,  19,   3,  13,  12,   6,   5,  10,   2,
  16,   4,  20,   1,   9,   7,  18,  14,   8,  15
	};
	int del_data[16384] = {
		12,   8,   9,  15,   7,  11,  19,   3,   6,   4,
  16,  17,   2,  20,  13,  14,  10,  18,   5,   1
	};
#endif

#if 0
	/* 13th Set */
	int data[16384] = {
		3,  40,  32,  22,  33,  93,  77,   1,  76,  21,
  58,  24,  92,   2,  85, 100,  81,  23,  46,  73,
  94,  57,  70,  35,  38,   4,  29,  87,  13,  61,
  79,  42,  55,  14,  30,  71,  88,  28,  89,  47,
  11,  97,  86,  43,  74,  60,  26,  66,  91,  56,
  96,  48,   9,  34,  37,  99,  52,  39,  72,  20,
  80,  10,  45,  75,  49,  41,  84,  50,  59,  19,
  63,  54,  82,  67,  51,  98,  44,  64,  16,  27,
  65,   7,   8,   6,  25,  95,  15,  17,  83,  68,
  90,  69,  36,   5,  18,  12,  31,  78,  53,  62
	};
	int del_data[16384] = {
		11,  95,  80,  69,  24,  75,   9,  29,  73,  51,
   1,  65,  63,  20,  64,  70,  54,  68,   3,  31,
   8,  23,  83,  81, 100,  97,  30,  87,   7,  91,
  50,  22,  18,  76,  48,  79,  57,  37,  71,  46,
  40,  62,  14,  92,  21,   2,  59,  33,  94,  77,
  93,  55,  35,  43,  10,  34,  49,  89,  32,  26,
  44,  56,  36,  16,  52,  58,  82,   4,  90,  61,
  13,  98,  27,  96,   5,  47,  72,  74,  19,  67,
  45,  38,  41,  42,  15,  99,  85,  86,  28,  78,
  39,  17,  25,  53,  12,   6,  60,  84,  88,  66
	};
#endif


	//int len = sizeof(data) / sizeof(int);
	int len = 10;

	srand(time(NULL));

#if 0
	init_data(data, len);
	init_data(del_data, len);
	printf("입력: \n");
	print_arr(data, len);
	printf("삭제: \n");
	print_arr(del_data, len);
#endif

	create_rbt_nil();
	root = nil;

	for (i = 0; i < len; i++)
	{
		//printf("입력 데이터: %d\n", data[i]);
		recur_insert_rbt_data(&root, nil, data[i]);
		//print_tree(root);
		//printf("\n");
	}

	print_tree(root);

#if 0
	for (i = 0; i < len; i++)
	{
		recur_delete_rbt_data(&root, del_data[i]);
	}
#endif

	return 0;
}

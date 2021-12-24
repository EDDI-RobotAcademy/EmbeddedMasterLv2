#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

//#define Guide_Code 1
//#define old 0
//#define new 1
#define delete 1

#define ABS(x) ((x > 0) ? (x) : -(x))

#define RR case1
#define RL case2
#define LL case3
#define LR case4
#define rot_case del_case

typedef enum _color color;
enum _color
{
	red = 0,
	black
};

typedef enum _case del_case;
enum _case
{
	case1 = 1,
	case2 = 2,
	case3 = 3,
	case4
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

rb *find_brother(rb **top_root, rb **root)
{
	printf("start find brother\n");
	rb *parent = (*root != *top_root) ? (*root)->parent : NULL;
	rb *brother;

	if(!parent)
		brother = NULL;
	else if(parent->data > (*root)->data)
		brother = parent->right;
	else
		brother = parent->left;

	printf("finish find brother\n");
	return brother;
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
	printf("왼쪽 최대값 찾기 시작!\n");
	rb **left_max = root;

	if(!(*root)->right)
		return left_max; 

	left_max = find_left_max(&(*root)->right);

	return left_max;
}

void chg_node(rb **root)
{
	printf("start change node\n");
	rb *tmp = *root;

	if((*root)->left)
	{
		(*root)->left->parent = (*root)->parent;
		*root = (*root)->left;
	}
	else if((*root)->right)
	{
		(*root)->right->parent = (*root)->parent;
		*root = (*root)->right;
	}
	else
		*root = NULL;

	printf("finish change node\n");

	free(tmp);
}

//black노드의 자식 특징
//1. black인 경우 : rb 규칙상 1개만 가질 수 없다.
//2. red인 경우 : rb 규칙상 1개만 가질 수 있다.
//3. true : 자식노드가 NULL이거나 둘 다 black인 경우
//4. false : 자식노드가 1개만 존재하거나 둘 다 red인 경우
bool chk_child_status(rb *root)
{
	printf("check brother's child\n");
	bool res;

	if(!root->left && !root->right)
		res = true;
	else if(!root->left)
		res = false;
	else if(!root->right)
		res = false;
	else if(root->left->color == red || root->right->color == red)
		res = false;
	else if(root->left->color == red && root->right->color == red)
		res = false;
	else
		res = true;

	printf("finish check brother's child\n");
	return res;
}

void swap_color(rb *x, rb *y)
{
	color tmp = x->color;

	x->color = y->color;
	y->color = tmp;
}

rot_case chk_rot_case(rb *parent, rb *root)
{
	printf("rotation case check\n");
	rot_case result;

	//child가 없는 경우는 chk_child_status에서 확인함
	if(parent->data < root->data)
	{
		printf("RR, RL case\n");
		if(root->left && !root->right)
			result = RL;
		else if(!root->left && root->right)
			result = RR;
		else if(root->left->color == red && root->right->color == black)
			result = RL;
		else
			result = RR;
	}
	else
	{
		printf("LL, LR case\n");
		if(!root->left && root->right)
			result = LR;
		else if(root->left && !root->right)
			result = LL;
		else if(root->left->color == black && root->right->color == red)
			result = LR;
		else
			result = LL;
	}
	printf("finish rotation case check\n");
	return result;
}

/*TODO*/
/*
   case 1 : 삭제노드 = black, 형제노드 = red
   case 2 : 삭제노드 = black, 형제노드 = black, 형제의 자식 = black or null
   case 3 : 삭제노드 = black, 형제노드 = black, 형제의 자식 = red

   0. 파라메터 : del_node, brother
   1. 예외 : 형제가 없으면 0 반환
   2. del_node가 black이고 자식노드가 없으면
   2-1. 형제노드 = red case1 반환
   2-2. 형제노드 = black
   2-2-1. 형제의 자식 = black or null case2 반환
   2-2-2. 형제의 자식 = red case3 반환
*/
del_case black_del_case(rb **root, rb *brother)
{
	printf("confirm delete case\n");
	//1
	if(!brother)
		return 0;
	//2-1
	else if(brother->color == red)
		return case1;
	//2-2-1
	else if(chk_child_status(brother))
		return case2;
	//2-2-2
	else
		return case3;
}

/*TODO*/
/*
   case 1 : 삭제노드 = black, 형제노드 = red
   case 2 : 삭제노드 = black, 형제노드 = black, 형제의 자식 = black or null
   case 3 : 삭제노드 = black, 형제노드 = black, 형제의 자식 = red

   0. 파라메터 : del_node
   1. case1 : 형제 = red
   2. case2,3 : 형제 = black
   2-1. case2 : 형제의 자식 = black or null
   (1) 형제노드의 색상을 red로 변경한다
   (2) del_node를 부모로 변경한다
   (3) 재귀로 case handler를 실행한다
   (4) (2)에서 del_node가 top_root이면 return 
   (5) (2)에서 부모가 red이면 black으로 변경한 뒤 (3)실행 
   2-2. case3 : 형제의 자식 = red
   (1) 형제노드의 위치를 확인
   (2) 형제노드 > 부모 : 형제노드의 오른쪽에 자식이 있으면, RR <-> RL
   (3) 형제노드 < 부모 : 형제노드의 왼쪽에 자식이 있으면, LL <-> LR 
*/

void black_del_case_handler(rb **top_root, rb **del_root, rb **del_node)
{
	printf("start black delete case handler\n");
	printf("del_root = %d\tdel_node = %d\n", (*del_root)->data, (*del_node)->data);
	rb *parent = ((*del_node)->parent) ? (*del_node)->parent : NULL;
	rb *brother = find_brother(top_root, del_node);

	if(!parent || *top_root == *del_node)
		return;

	switch(black_del_case(del_node, brother))
	{
		case case1:
			printf("case1 : black without child, brother=red\n");
			switch(chk_rot_case(parent, brother))
			{
				//형제가 오른쪽에 있는 경우
				case RR:
					if(chk_child_status(brother->left))
						swap_color(brother, brother->left);
					else
						swap_color(parent, brother);

					left_rotation(top_root, &brother);
					//del_node가 del_root의 left max라면 회전 후 del_root를 변경 
					//*del_root = (*del_root != *del_node) ? brother->left : *del_root;
					*del_root = (*del_root == parent) ? brother->left : *del_root;
					black_del_case_handler(top_root, del_root, &brother);
#if 0
					black_del_case_handler(top_root, del_root, del_node);
#endif
					break;
				//형제가 왼쪽에 있는 경우
				case LL:
					if(chk_child_status(brother->right))
						swap_color(brother, brother->right);
					else
						swap_color(parent, brother);

					right_rotation(top_root, &brother);
					//del_node가 del_root의 left max라면 회전 후 del_root를 변경 
					//*del_root = (*del_root != *del_node) ? brother->right : *del_root;
					*del_root = (*del_root == parent) ? brother->right : *del_root;
					black_del_case_handler(top_root, del_root, &brother);
#if 0
					black_del_case_handler(top_root, del_root, del_node);
#endif
					break;
			}
			printf("finish case1 handle\n");
			break;
		case case2:
			printf("case2 : black without child, brother=black, nephew=NULL\n");
			if(parent->color != red)
				brother->color = red;
#if 0
			black_del_case_handler(top_root, del_root, &parent);
#endif
			//parent = red, brother = black, nephew = NULL인 경우
			//brother와 parent의 색을 서로 변경
			else if(chk_child_status(brother))
			{
				swap_color(brother, parent);
				return;
			}
			black_del_case_handler(top_root, del_root, &parent);
#if 0
			if(parent->color == red)
				swap_color(parent, brother);
			else
			{
				brother->color = red;
				black_del_case_handler(top_root, del_root, &parent);
			}
#endif
			printf("finish case2 handle\n");
			break;
		case case3:
			printf("case3 : black without cihld, brother=black, nephew=red\n");
			rb *child;
			switch(chk_rot_case(parent, brother))
			{
				case RL:
					printf("RL rotation\n");

					child = brother->left;

#if 0
					if(brother->right && brother->right->color == black)
						swap_color(child, child->right);
					else
						child->color = parent->color;
#endif
#if 0
					child->color = parent->color;
					parent->color = brother->color;
#endif
#if 1
					child->color = brother->color;
					brother->color = parent->color;
					parent->color = child->color;
#endif

					child->parent = parent;
					brother->parent = child;

					if(child->right)
						child->right->parent = brother;

					brother->left = child->right;
					child->right = brother;
					parent->right = child;

					left_rotation(top_root, &child);
					//del_node가 del_root의 left max라면 회전 후 del_root를 변경 
					//*del_root = (*del_root != *del_node) ? child->left : *del_root;
					*del_root = (*del_root == parent) ? child->left : *del_root;
					break;
				case RR:
					printf("RR rotation\n");

					child = brother->right;
#if 0
					if(child->left && child->left->color == red)
						swap_color(child, child->left);
					else
						child->color = brother->color;
#endif

					child->color = brother->color;
					brother->color = parent->color;
					parent->color = child->color;

					left_rotation(top_root, &brother);
					//del_node가 del_root의 left max라면 회전 후 del_root를 변경 
					//*del_root = (*del_root != *del_node) ? brother->left : *del_root;
					*del_root = (*del_root == parent) ? brother->left : *del_root;
					break;
				case LR:
					printf("LR rotation\n");

					child = brother->right;

#if 0
					if(child->left && child->left->color == red)
						swap_color(child, child->left);
					else
						child->color = parent->color;
#endif
#if 0
					child->color = parent->color;
					parent->color = brother->color;
#endif
#if 1
					child->color = brother->color;
					brother->color = parent->color;
					parent->color = child->color;
#endif

					child->parent = parent;
					brother->parent = child;

					if(child->left)
						child->left->parent = brother;

					brother->right = child->left;
					child->left = brother;
					parent->left = child;

					right_rotation(top_root, &child);
					//del_node가 del_root의 left max라면 회전 후 del_root를 변경 
					//*del_root = (*del_root != *del_node) ? child->right : *del_root;
					*del_root = (*del_root == parent) ? child->right : *del_root;
					break;
				case LL:
					printf("LL rotation\n");
					child = brother->left;

#if 0
					if(brother->right && brother->right->color == black)
						swap_color(child, child->right);
					else
						child->color = brother->color;
#endif
					child->color = brother->color;
					brother->color = parent->color;
					parent->color = child->color;

					right_rotation(top_root, &brother);
					//del_node가 del_root의 left max라면 회전 후 del_root를 변경 
					//*del_root = (*del_root != *del_node) ? brother->right : *del_root;
					*del_root = (*del_root == parent) ? brother->right : *del_root;
					break;
			}
			printf("finish case3 handle\n");
			break;
		default:
			break;
	}
	printf("finish black delete case handler\n");
}

void delete_rb_data(rb **top_root, rb **root, int data)
{
	rb **del_node = NULL;
	//밸런싱 회전에 의한 root 위치 변경 예외처리용 변수
	rb *backup;
	bool del_flg = false;

	if((*root)->data > data)
		delete_rb_data(top_root, &(*root)->left, data);
	else if((*root)->data < data)
		delete_rb_data(top_root, &(*root)->right, data);
	//자식 2개 인 경우
	//left max 찾기
	else if((*root)->left && (*root)->right)
	{
		del_flg = true;
		del_node = find_left_max(&(*root)->left);
		backup = *root;
	}
	//단일 노드 또는 자식 1개 인 경우
	//단일 노드 : red
	//자식 1개 : black, 자식 = right red
	else 
	{
		del_flg = true;
		backup = *root;
		del_node = root;
	}

	if(!del_node)
	{
		printf("there is no data in tree\n");
		return;
	}

	switch(del_flg)
	{
		case true:
			printf("delete data = %d\n", (*del_node)->data);
			if((*del_node)->color == black)
			{
				//chk_child_status
				//true : 자식모두 black 또는 NULL -> case handler 실행 후 삭제
				//false : 자식모두 red 또는 1개의 red -> 색변경 후 삭제
				if(chk_child_status(*del_node))
				{
					black_del_case_handler(top_root, &backup, del_node);
					backup->data = (*del_node)->data;
				}
				else
					swap_color(*del_node, ((*del_node)->left) ? (*del_node)->left : (*del_node)->right);
				chg_node(del_node);
			}
			else
			{
				chg_node(del_node);
			}
			break;
		case false:
			break;
	}
#if 0
	switch((*del_node)->color)
	{
		case black:
			printf("delete data = %d\n", (*del_node)->data);
			//chk_child_status
			//true : 자식모두 black 또는 NULL -> case handler 실행 후 삭제
			//false : 자식모두 red 또는 1개의 red -> 색변경 후 삭제
			if(chk_child_status(*del_node))
				black_del_case_handler(top_root, root, del_node);
			else
				swap_color(*del_node, ((*del_node)->left) ? (*del_node)->left : (*del_node)->right);
			print_rb(*top_root);
			chg_node(del_node);
			break;
		case red:
			chg_node(del_node);
			break;
	}
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

void random_del_node(rb **top_root, rb **root, int *data, int len)
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
		printf("delete root = %d\n", data[idx[i]]);
		delete_rb_data(top_root, root, data[idx[i]]);
		print_rb(*top_root);
	}
}

int main(void)
{
    rb *root = NULL;
    int i;

#ifdef Guide_Code
    int data[50];
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);

    for (i = 0; i < len; i++)
    {
		//printf("insert processing!\n");
		//printf("insert data = %d\n", data[i]);
		insert_rb_data(&root, &root, NULL, data[i]);
		//print_rb(root);
    }
#else
	//insert test bench
	//int data[] = {500, 50, 1000, 100, 25, 750, 1250, 75, 125, 37, 12, 625, 875, 1125, 1375, 6, 30, 40, 45};
	//int data[] = {87, 58, 50,  34,   6,  57,  41,  43,  84,  95,   9,  62,  28,  2, 78,  92,  52,   5,  55,  49,  86};
	//int data[] = {72, 194, 173, 161, 133, 158, 200};
	//int data[] = {173, 133, 194, 72, 161, 158, 200};
	//int data[] = {80, 45, 200, 40, 50, 176, 280, 32, 44, 49, 55, 150, 180, 277, 290, 31, 33, 43, 47, 54, 149, 179, 275, 285, 29};
	//int data[] = {55, 32, 60, 31, 40, 58, 65, 29};
	//int data[] = {55, 31, 29};
	//int data[] = {500, 200, 1000, 100, 300, 450, 435};
	//case1,2
	int data[] = {6, 48, 17, 47, 13, 49, 50, 25, 27, 29, 46, 33, 19, 5, 44, 10, 32, 30, 23, 26, 15, 12, 38, 20, 18, 24, 4, 2,
				  43, 9, 7, 37, 45, 40, 16, 42, 31, 3, 11, 8, 14, 36, 35, 22, 34, 41, 39, 21, 28, 1};

	//delete test bench
	//case1
	//int data[] = {55, 45, 65, 75, 60, 85, 59, 62, 70};
	//case3
	//RR : black parent
	//int data[] = {55, 45, 65, 75};
	//RR : red parent
	//int data[] = {55, 45, 65, 75, 85, 95};
	//RL : black parent
	//int data[] = {55, 45, 65, 60};
	//RL : red parent
	//int data[] = {55, 45, 65, 75, 85, 90};

	int len = sizeof(data)/sizeof(int);
	for(i = 0; i < len; i++)
	{
		printf("insert processing!\n");
		printf("insert data = %d\n", data[i]);
		insert_rb_data(&root, &root, NULL, data[i]);
		print_rb(root);
	}
#endif

	printf("insert clear!\n");
	print_rb(root);

#ifdef delete
	printf("\ndelete node\n");
	random_del_node(&root, &root, data, len);
	printf("delete clear!\n");
	print_rb(root);
#else
	printf("\ndelete node!\n");
	//case1
	//case1 -> case2
	//delete_rb_data(&root, &root, 45);
	//delete_rb_data(&root, &root, 44);
	//case1 -> case3
	//delete_rb_data(&root, &root, 55);
	//delete_rb_data(&root, &root, 45);

	//case2
	//delete_rb_data(&root, &root, 18);
	//delete_rb_data(&root, &root, 7);
	delete_rb_data(&root, &root, 10);

	//case3
	//RR,RL : black parent
	//delete_rb_data(&root, &root, 45);
	//RR,RL : red parent
	//delete_rb_data(&root, &root, 75);
	printf("delete clear!\n");
	print_rb(root);
#endif
    return 0;
}

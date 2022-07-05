#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define LEVEL(X) ((X) ? X->black_level : 0)
#define PRINTDATA(X) (printf(#X " data : %d\n" , X->data))
#define PRINTCOLOR(X) (X->color == RED)?(printf(#X " color : %s\n" , "RED")) : (printf(#X " color : %s\n" , "BLACK"))
#define TREE 1
#define AVL_TREE 2
#define REDBLACK_TREE 3
#define RED 0
#define BLACK 1
#define LEFT 3
#define RIGHT 6
#define LL 33
#define LR 36
#define RL 63
#define RR 66

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
	while((*root)->data != -1)
	{
		if((*root)->color == BLACK)
			(*root)->black_level = MAX(LEVEL((*root)->left), LEVEL((*root)->right)) + 1;
		else
			(*root)->black_level = MAX(LEVEL((*root)->left), LEVEL((*root)->right));
	
		root = &(*root)->parent;
	}
}

void recur_update_black_level(redblack **root)
{
	if(root==NULL)
		return;
	if((*root)==NULL)
		return;
	if((*root)->data != -1)
	{
		recur_update_black_level(&(*root)->left);
		recur_update_black_level(&(*root)->right);
		if((*root)->color == BLACK)
			(*root)->black_level = MAX(LEVEL((*root)->left), LEVEL((*root)->right)) + 1;
		else
			(*root)->black_level = MAX(LEVEL((*root)->left), LEVEL((*root)->right));
		
	}
}

int balance_factor(redblack **root)
{
	return LEVEL((*root)->left) - LEVEL((*root)->right);
} 

redblack** LL_rotation(redblack *rotate_middle, redblack *rotate_bro, redblack **tmp_parent)
{//RR case에서 왼쪽으로 회전하는걸 LL_rotation이라고 함.
// double pointer로 하면 *rotate_middle = *tmp_bro가 되어버리는 불상사..      
	printf("LL rotation\n");


	rotate_middle->parent = rotate_bro->parent;
	rotate_bro->parent = rotate_middle;
	rotate_bro->right = rotate_middle->left;

	
	if(rotate_middle->left->data != -1)
	{
		rotate_middle->left->parent = rotate_bro;
	}
	rotate_middle->left = rotate_bro;
	*tmp_parent = rotate_middle;

#if 0
	if(rotate_middle->parent->data == -1)
		rotate_middle->color = BLACK;
	else
		rotate_middle->color = RED;
	rotate_middle->left->color = BLACK;
	rotate_middle->right->color = BLACK;
	// TODO middle이 root인 경우 handling.
	// root면 parent가 nil인 것을 이용.
#endif

	// 하나의 함수엔 하나의 기능만
	PRINTDATA(rotate_middle);
	PRINTCOLOR(rotate_middle);
	PRINTDATA((rotate_middle->left));
	PRINTCOLOR((rotate_middle->left));
	PRINTDATA((rotate_middle->right));
	PRINTCOLOR((rotate_middle->right));

	return tmp_parent;

}

redblack** LR_rotation(redblack *rotate_middle, redblack *rotate_bro, redblack **tmp_parent)
{
	printf("LR rotation\n");
	//FOR RL case
	//Right rotation
	rotate_bro->right = rotate_middle->left;
	rotate_bro->right->parent = rotate_bro;
	rotate_middle->parent = rotate_bro->right;
	rotate_middle->left = rotate_bro->right->right;
	if(rotate_middle->left->data != -1)
		rotate_middle->left->parent = rotate_middle;
	rotate_bro->right->right = rotate_middle;

	rotate_middle = rotate_bro->right;


	rotate_middle->parent = rotate_bro->parent;
	rotate_bro->parent = rotate_middle;
	rotate_bro->right = rotate_middle->left;

	
	if(rotate_middle->left->data != -1)
	{
		rotate_middle->left->parent = rotate_bro;
	}
	rotate_middle->left = rotate_bro;
	*tmp_parent = rotate_middle;

#if 0
	if(rotate_middle->parent->data == -1)
		rotate_middle->color = BLACK;
	else
		rotate_middle->color = RED;
	rotate_middle->left->color = BLACK;
	rotate_middle->right->color = BLACK;
	// TODO middle이 root인 경우 handling.
#endif

	PRINTDATA(rotate_middle);
	PRINTCOLOR(rotate_middle);
	PRINTDATA((rotate_middle->left));
	PRINTCOLOR((rotate_middle->left));
	PRINTDATA((rotate_middle->right));
	PRINTCOLOR((rotate_middle->right));

	return tmp_parent;
}

redblack** RL_rotation(redblack *rotate_middle, redblack *rotate_bro, redblack **tmp_parent)
{
	printf("RL rotation\n");
	//FOR LR case
	//left rotation
	rotate_bro->left = rotate_middle->right;
	rotate_bro->left->parent = rotate_bro;
	rotate_middle->parent = rotate_bro->left;
	rotate_middle->right = rotate_bro->left->left;
	if(rotate_middle->right->data != -1)
		rotate_middle->right->parent = rotate_middle;
	rotate_bro->left->left = rotate_middle;

	rotate_middle = rotate_bro->left;


	rotate_middle->parent = rotate_bro->parent;
	rotate_bro->parent = rotate_middle;
	rotate_bro->left = rotate_middle->right;

	
	if(rotate_middle->right->data != -1)
	{
		rotate_middle->right->parent = rotate_bro;
	}
	rotate_middle->right = rotate_bro;
	*tmp_parent = rotate_middle;

#if 0
	if(rotate_middle->parent->data == -1)
		rotate_middle->color = BLACK;
	else
		rotate_middle->color = RED;
	rotate_middle->left->color = BLACK;
	rotate_middle->right->color = BLACK;
#endif

	PRINTDATA(rotate_middle);
	PRINTCOLOR(rotate_middle);
	PRINTDATA((rotate_middle->left));
	PRINTCOLOR((rotate_middle->left));
	PRINTDATA((rotate_middle->right));
	PRINTCOLOR((rotate_middle->right));

	return tmp_parent;
}

redblack** RR_rotation(redblack *rotate_middle, redblack *rotate_bro, redblack **tmp_parent)
{
	printf("RR rotation\n");
	PRINTDATA(rotate_middle);
	PRINTDATA(rotate_bro);
	PRINTDATA((*tmp_parent));

	rotate_middle->parent = rotate_bro->parent;
	rotate_bro->parent = rotate_middle;
	rotate_bro->left = rotate_middle->right;

	
	if(rotate_middle->right->data != -1)
	{
		rotate_middle->right->parent = rotate_bro;
	}
	rotate_middle->right = rotate_bro;
	*tmp_parent = rotate_middle;

#if 0
	if(rotate_middle->parent->data == -1)
		rotate_middle->color = BLACK;
	else
		rotate_middle->color = RED;
	rotate_middle->left->color = BLACK;
	rotate_middle->right->color = BLACK;
#endif

	PRINTDATA(rotate_middle);
	PRINTCOLOR(rotate_middle);
	PRINTDATA((rotate_middle->left));
	PRINTCOLOR((rotate_middle->left));
	PRINTDATA((rotate_middle->right));
	PRINTCOLOR((rotate_middle->right));

	return tmp_parent;
}

redblack** find_parent_to_me_ptr(redblack **root, redblack **root_backup)
{
	if((*root)->parent->data == -1)
	{
		return root_backup;
	}
	else if((*root)->parent->left == (*root))
	{
		return &(*root)->parent->left;
	}
	else if((*root)->parent->right == (*root))
	{
		return &(*root)->parent->right;
	}
}

redblack** find_parent_to_bro_ptr(redblack **root)
{
	if((*root)->parent->data == -1)
	{
		assert("0"&&"ROOT has no BROTHER\n");
	}
	else if((*root)->parent->left == (*root))
	{
		return &(*root)->parent->right;
	}
	else if((*root)->parent->right == (*root))
	{
		return &(*root)->parent->left;
	}
}

void alternate_color(redblack *tree, int color)
{
	printf("alternate color\n");
	redblack* tmp = tree;
	if(tmp)
	{
		if(tmp->data == -1)
			return;

		alternate_color(tmp->left, (color == BLACK) ? RED : BLACK);
		tmp->color = color;
		update_black_level(&tmp);
		alternate_color(tmp->right, (color == BLACK) ? RED : BLACK);
		update_black_level(&tmp);
	}
}

int check_4th_level(redblack **root, int level)
{
	if(*root)
	{
		if((*root)->data == -1)
			return level;
		return MAX(check_4th_level(&(*root)->left,level+1),check_4th_level(&(*root)->right,level+1));
	}
}

int check_child(redblack **root)
{
	if(*root)
	{
		if((*root)->left->data!=-1 && (*root)->right->data != -1)
		{
			return 2;
		}
		else if((*root)->left->data == -1 && (*root)->right->data == -1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

void print_redblack_for_check(redblack *tree);

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
	
	int LR_flag = 0;
	if(!(*root))
	{
        // make NIL singleton
    	NIL = (redblack *)malloc(sizeof(redblack));
    	NIL->header = REDBLACK_TREE;
    	NIL->parent = NULL;
    	NIL->left = NULL;
    	NIL->right = NULL;
    	NIL->color = BLACK;
    	NIL->black_level = 0;
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
	update_black_level(root);
	(*root)->color = tmp_parent ? RED : BLACK;
	

	while((*root)->parent->parent!=NIL)
	{
		// RECONSTRUCTION
		if((*root)->parent->parent == NULL)
		{
			printf("Root case, return\n");
		        return;
		}
		if((*root)->parent->parent->data == -1)
		{
			printf("second level case, return\n");
		        return;
		}

		tmp_grandparent = find_parent_to_me_ptr(&(*root)->parent->parent, root_backup);

		if((*root)->parent->parent->left == (*root)->parent)
		{
		    tmp_parent = &(*root)->parent->parent->left;
	        tmp_uncle = &(*root)->parent->parent->right;

			LR_flag += LEFT*10;
		}
		else
		{
	        tmp_parent = &(*root)->parent->parent->right;
	        tmp_uncle = &(*root)->parent->parent->left;
			LR_flag += RIGHT*10;
		}

		if((*root)->parent->left == (*root))
		{
	        tmp_mine = &(*root)->parent->left;
	        tmp_bro = &(*root)->parent->right;
			LR_flag += LEFT;
		}
		else
		{
	        tmp_mine = &(*root)->parent->right;
	        tmp_bro = &(*root)->parent->left;
			LR_flag += RIGHT;
		}

		if((*root)->parent->color == RED && (*root)->color == RED) // no adjacent red violation, while 문 돌면서 color가 바뀌므로 뒤에도 추가.
		{
			// bro가 존재할 수 없음. R자식으로는 Black 밖에 못오는데, 삽입할 때 형제가 있다면 이미 삽입된 시점에서 Black이 있어야함. 모순.
			// 이것도 초창기만.
			//printf("RED violation occur\n");
		        // compare brother of parent
		        if((*tmp_uncle)->color == BLACK)
		        {
					//printf("UNCLE is BLACK\n");
					// BLACK인 UNCLE은 NIL 밖에 없나? 아무리 봐도 UNCLE이 BLACK이 안될수는 없는데.
					// 처음에는 맞는데, while loop 타면서 올라가면 UNCLE이 black인 경우도 나옴.
					if(LR_flag == LL || LR_flag == RR)
					{
						//after recoloring, same black level violation occurs.
						rotate_middle = (*tmp_parent);
						rotate_bro = (*tmp_grandparent);

						rotate_middle->parent = rotate_bro->parent;
						rotate_bro->parent = rotate_middle;
						*tmp_parent = *tmp_bro;
						*tmp_bro = rotate_bro;
						*tmp_grandparent = rotate_middle;
						if((*tmp_parent)->data != -1)
							(*tmp_parent)->parent = rotate_bro; // NIL의 parent가 생기는 문제
						rotate_bro->color = RED;
						rotate_middle->color = BLACK;
						update_black_level(&rotate_bro);
						update_black_level(&rotate_middle);
					}
					else if(LR_flag == LR)
					{
						rotate_middle = (*tmp_mine);
						rotate_mine = (*tmp_parent);
						rotate_bro = (*tmp_grandparent);

						rotate_middle->parent = rotate_bro;
						*tmp_parent = rotate_middle;
						rotate_mine->parent = rotate_middle;
						*tmp_mine = rotate_middle->left;
						if((*tmp_mine)->data != -1)
							(*tmp_mine)->parent = rotate_mine;
						rotate_middle->left = rotate_mine;

						// LL rotate
						
						rotate_middle->parent = rotate_bro->parent;
						rotate_bro->parent = rotate_middle;
						*tmp_parent = rotate_middle->right;
						rotate_middle->right = rotate_bro;
						*tmp_grandparent = rotate_middle;
						if((*tmp_parent)->data != -1)
						        (*tmp_parent)->parent = rotate_bro; // NIL의 parent가 생기는 문제.
						update_black_level(&rotate_bro);

						rotate_bro->color = RED;
						rotate_middle->color = BLACK;
						update_black_level(&rotate_bro);
						update_black_level(&rotate_middle);
						root = &rotate_mine;
					}
					else if(LR_flag == RL)
					{

						rotate_middle = (*tmp_mine);
						rotate_mine = (*tmp_parent);
						rotate_bro = (*tmp_grandparent);

						rotate_middle->parent = rotate_bro;
						*tmp_parent = rotate_middle;
						rotate_mine->parent = rotate_middle;
						*tmp_mine = rotate_middle->right;
						if((*tmp_mine)->data != -1)
						        (*tmp_mine)->parent = rotate_mine;
						rotate_middle->right = rotate_mine;

						// RR rotate

						rotate_middle->parent = rotate_bro->parent;
						rotate_bro->parent = rotate_middle;
						*tmp_parent = rotate_middle->left;
						rotate_middle->left = rotate_bro;
						*tmp_grandparent = rotate_middle;
						if((*tmp_parent)->data != -1)
						        (*tmp_parent)->parent = rotate_bro; // NIL의 parent가 생기는 문제.
						
						rotate_bro->color = RED;
						rotate_middle->color = BLACK;

						update_black_level(&rotate_bro);
						update_black_level(&rotate_middle);
						root = &rotate_mine;						
					}
					else
					{
						printf("LR flag : %d\n", LR_flag);
						assert(0 && "exception case occur LR reconstruction\n");
					}
				
		        }
		        else // UNCLE is RED
		        {
					//printf("UNCLE is RED\n");
					if((*tmp_grandparent)!=(*root_backup))
						(*tmp_grandparent)->color = RED;
					(*tmp_parent)->color = BLACK;
					(*tmp_uncle)->color = BLACK;
					update_black_level(tmp_mine);
					update_black_level(tmp_uncle);
		        }
		}
		root = &(*root)->parent;
		LR_flag = 0;
	}	
}

void delete_redblack(redblack **root, int data)
{
	redblack** root_backup = root;
	redblack** cursor = root;
	redblack** tmp_parent = NULL;
	redblack** tmp_nephew = NULL;
	
	redblack** tmp_bro = NULL;
	redblack** tmp_mine = NULL;
	redblack** tmp_uncle = NULL;
	redblack** tmp_grandparent = NULL;

	redblack* NIL;

	int LR_flag = 0;
    int pre_LR_rotate = 0;
	int before_no_nephew = 0;
	int double_rotate = 0;
	
	redblack* replace_node;
	redblack* tmp;
	redblack** tmp_backup = root;

	int deleted_node_color;
	int parent_color;
	int uncle_color;
	int grandparent_color;
	int bro_color;
	int mine_color;

	int loop_cnt = 0;
	

	// 1. FIND
	if(!(*cursor))
	{
		printf("There is no node in redblack tree!\n");
		return;
	}
	else
	{
		NIL = (*cursor)->parent;
	}
	while(*cursor)
	{
		if((*cursor)->data > data)
		{
			tmp_backup = cursor;
			cursor = &(*cursor)->left;
		}
		else if((*cursor)->data < data)
		{
			tmp_backup = cursor;
			cursor = &(*cursor)->right;
		}
		else
		{
			break;
		}
	}
	

	// 2. DELETE
	PRINTDATA((*cursor));
	replace_node = *cursor;

	if((*cursor)->left != NIL && (*cursor)->right != NIL)
	{
		tmp_backup = cursor;
		cursor = &(*cursor)->left;
		while((*cursor)!=NIL)
		{
			if((*cursor)->right!=NIL)
			{
				tmp_backup = cursor;
				cursor = &(*cursor)->right;
			}
			else
			{
				replace_node->data = (*cursor)->data;
				tmp = *cursor;
				deleted_node_color = (*cursor)->color;
				if((*cursor)->right==NIL)
				{
					*cursor = (*cursor)->left;
					
					// (*cursor) can be NIL
					if((*cursor)!=NIL)
						(*cursor)->parent = *tmp_backup;
				}
				else if((*cursor)->left==NIL)
				{
					*cursor = (*cursor)->right;
					if((*cursor)!=NIL)
						(*cursor)->parent = *tmp_backup;
				}
				free(tmp);
				break;
			}
		}
	}
	else
	{
		tmp = *cursor;
		deleted_node_color = (*cursor)->color;
		if((*cursor)->right==NIL)
		{
			*cursor = (*cursor)->left;
			if((*cursor)!=NIL)
				(*cursor)->parent = *tmp_backup;
		}
		else if((*cursor)->left==NIL)
		{
			*cursor = (*cursor)->right;
			if((*cursor)!=NIL)
				(*cursor)->parent = *tmp_backup;
		}	
		free(tmp);
	}

	// 다 삭제 후에 root에 NIL이 있는 문제 해소.
	if((*root_backup)== NIL)
	{
		//final deletion
		free(*root_backup);
		*root_backup = NULL;
		printf("NIL free\n");
		return;
	}

	if((*cursor)==(*tmp_backup)) // (*root) == (*root_backup)
	{
		(*cursor)->color = BLACK;
		(*cursor)->parent = NIL;
		return;// root case
		// root case를 가장 먼저 검사해야함.
	}

	if(deleted_node_color == RED && (*cursor)->color == BLACK) // 1) Red 삭제 NIL come, 3) RED 삭제 BLACK come
		return;
	if(deleted_node_color == BLACK && (*cursor)->color == RED) // 4) Black 삭제 RED come
	{
		PRINTDATA(((*cursor)->parent));
		(*cursor)->color = BLACK;
		update_black_level(cursor);
		PRINTDATA((*cursor));
		return;
	}

	// 5) Black 삭제 Nil come
	// root가 NIL을 가리키는 문제 해결(삭제 후 NIL이 오는 경우)
	tmp_mine = cursor;
	cursor = tmp_backup;
	printf(" cursor tmp bckup : %d\n",(*cursor)->data);

	while((*cursor)!=NIL) // parent를 타고 loop를 돌다가 조카가 있는 red나 root를 만나면 return. // 전 loop에서 무엇을 하다 왔는지 check할 상태변수.
	{
		pre_LR_rotate = 0;
		if((*cursor)->parent == NULL)
		{
			printf("I'm NIL, return\n");
		        return;
		}
		tmp_parent = find_parent_to_me_ptr(cursor, root_backup);

		if((*cursor)->left == *tmp_mine)
		{
	        tmp_bro = &(*cursor)->right;
			LR_flag += 10*RIGHT; // bro의 방향, 조카의 위치를 대비하기 위한 LR_flag.
		}
		else
		{
	        tmp_bro = &(*cursor)->left;
			LR_flag += 10*LEFT;
		}

		// coloring에 사용될 변수
		parent_color = (*tmp_parent)->color;
		bro_color = (*tmp_bro)->color;
		mine_color = (*tmp_mine)->color;
		
		// 첫번째 삭제때는 아래 케이스가 예외고, 위로 BLACK이 연속해서 나올때.
		// 1. BRO == RED, PARENT == BLACK - 회전하면 회전 되는 쪽은 level이 유지. 회전되어 추가되는 쪽 채우고 끝
		// 2. BRO == BLACK, PARENT == RED - PARENT를 RED로 바꾸고 끝.
		// 3. PARENT == ROOT 그냥 끝.
		if((!before_no_nephew && loop_cnt > 0) && (bro_color == BLACK && parent_color == RED))
		{
			if(double_rotate == 2)
			{
				(*tmp_parent)->color = RED;
			}
			else
			{
				(*tmp_parent)->color = BLACK;
			}
			printf("loop_cnt>0, bro BLACK, parent RED\n");
			break;
		}
		else if(loop_cnt > 1 && (*tmp_parent) == (*root_backup))
		{
			// 주의 : 삭제하고 나서 조카가 없는 NIL - B - B 형태면 loop를 다시 돌게 됨.
			// 따라서 loop cnt가 2부터 체크해야함.
			printf("loop_cnt > 1, parent == ROOT!\n");
			break;
		}
		before_no_nephew = 0;//초기화
		

		PRINTDATA((*tmp_parent));
		PRINTDATA((*tmp_bro));
		PRINTDATA((*cursor));
		PRINTDATA((*cursor)->left);
		PRINTDATA((*cursor)->right);
		
		printf(" root tmp bro : %d\n",(*tmp_bro)->data);

		PRINTDATA((*tmp_parent)->parent);
		// 부모가 검은색인지 중요.
		// 조카가 있으면 회전 먼저.
		// 조카가 없는 경우는 사실 초반 말고는 없다.
		
		if((*tmp_bro)->left != NIL || (*tmp_bro)->right != NIL) 
		{
			printf("///////////////There is Nephew\n");

			if((*tmp_bro)->left != NIL && (*tmp_bro)->right != NIL)
			{
				if(LR_flag == 10*RIGHT)
				{
					tmp_nephew = &(*tmp_bro)->right;
					LR_flag += RIGHT;
				}
				else if(LR_flag == 10*LEFT)
				{
					tmp_nephew = &(*tmp_bro)->left;
					LR_flag += LEFT;
				}
				else
				{
					assert(0&&"DO NOT COME");
				}
			}
			else if((*tmp_bro)->right != NIL)
			{
				tmp_nephew = &(*tmp_bro)->right;
				LR_flag += RIGHT;
			}
			else // right 없고 left 있고
			{
				tmp_nephew = &(*tmp_bro)->left;
				LR_flag += LEFT;
			}
			PRINTDATA((*tmp_bro));
			PRINTDATA((*tmp_parent));

			if((*tmp_bro)->left != NIL && (*tmp_bro)->right != NIL)
			{
				printf("There are two Nephews\n");

				// uncle의 child = nephew가 되는건데.. RB BR BB BR에서 RB는 무조건 R쪽으로 로테이션 시켜야함.
				if((*tmp_bro)->color == RED)
				{
					if((*tmp_bro)->left->color == BLACK && (*tmp_bro)->right->color == BLACK)
					{
						
						if(check_child(&(*tmp_bro)->left) == 0 && check_child(&(*tmp_bro)->right)==0)
						{
							printf("cousin not have children\n");
							//초반부 BRB - B - BRB의 예외처리
							(*tmp_bro)->color = BLACK;
							(*tmp_bro)->left->color = RED;
							(*tmp_bro)->right->color = RED;
							LR_flag = 0;
							PRINTDATA((*cursor));
							tmp_mine = tmp_bro;
							cursor = &(*tmp_bro)->parent;
							PRINTDATA((*tmp_mine));
							PRINTDATA((*cursor));
							
							return;
							
						}
					}
				}
                else // (*tmp_bro)->color == BLACK
                {

                    // LR_flag -> bro의 방향, 즉 uncle의 방향임에 주의!!
                    if(LR_flag / 10 == RIGHT && ((*tmp_bro)->left->color == RED && (*tmp_bro)->right->color == BLACK))
                    {
                        // 왼쪽에 BBB case. 그리고 오른쪽에 uncle의 왼쪽이 RED, 오른쪽이 BLACK. 이러면 무조건 오른쪽으로 회전시켜야함.
                        // BRB - RBR 을 보면 왼쪽이 level이 높을 확률이 높고, 못해도 level이 동일한 경우인데 이래도 회전하면 오른쪽은 R B RR로 black level 1 유지가 가능하다.
                        printf("Uncle's left child is RED\n");
                        RR_rotation((*tmp_bro)->left,(*tmp_bro),tmp_bro);
                        // RED를 당겨 올라가니까 그 쪽은 black level이 안변함. RED의 parent는 무조건 BLACK. 반대쪽은 그러면 BLACK이 내려오게 됨.
                        // 따라서 내려오는 놈을 RED로 바꿔주면 끝.
                        (*tmp_bro)->right->color = RED;
                        pre_LR_rotate = RIGHT;
                        printf("(*tmp_bro)->right->color = RED\n");
                        PRINTDATA(((*tmp_bro)->right));
                        PRINTCOLOR(((*tmp_bro)->right));
                    } 
                    else if(LR_flag / 10 == RIGHT && ((*tmp_bro)->left->color == RED && (*tmp_bro)->right->color == RED))
                    {
                        // test7 delete 14
                        // NIL - R - B(RR자식)
                        printf("Uncle's left child is RED and right child is RED\n");
                        RR_rotation((*tmp_bro)->left,(*tmp_bro),tmp_bro);
                        // RED를 당겨 올라가니까 그 쪽은 black level이 안변함. RED의 parent는 무조건 BLACK. 반대쪽은 그러면 BLACK이 내려오게 됨.
                        pre_LR_rotate = RIGHT;
                        PRINTDATA(((*tmp_bro)->right));
                        PRINTCOLOR(((*tmp_bro)->right));
                    }
					else if(LR_flag / 10 == RIGHT && ((*tmp_bro)->left->color == BLACK && (*tmp_bro)->right->color == RED))
                    {
                        // 오른쪽에 BBB case. 그리고 왼쪽에 uncle의 오른쪽이 RED, 왼쪽이 BLACK. 이러면 무조건 왼쪽으로 회전시켜야함. // 위에 케이스랑 대칭성
                        printf("Uncle's right child is RED\n");
                        LL_rotation((*tmp_bro)->right,(*tmp_bro),tmp_bro);
                        // RED를 당겨 올라가니까 그 쪽은 black level이 안변함. RED의 parent는 무조건 BLACK. 반대쪽은 그러면 BLACK이 내려오게 됨.
                        // 따라서 내려오는 놈을 RED로 바꿔주면 끝.
                        (*tmp_bro)->left->color = RED;
                        pre_LR_rotate = LEFT;
                        printf("(*tmp_bro)->left->color = RED\n");
                        PRINTDATA(((*tmp_bro)->left));
                        PRINTCOLOR(((*tmp_bro)->left));
                    } 
                    else if(LR_flag / 10 == LEFT && ((*tmp_bro)->left->color == RED && (*tmp_bro)->right->color == RED))
                    {
                        // NIL - R - B(RR자식)
                        printf("Uncle's left child is RED and right child is RED\n");
                        LL_rotation((*tmp_bro)->right,(*tmp_bro),tmp_bro);
                        // RED를 당겨 올라가니까 그 쪽은 black level이 안변함. RED의 parent는 무조건 BLACK. 반대쪽은 그러면 BLACK이 내려오게 됨.
                        pre_LR_rotate = LEFT;
                        PRINTDATA(((*tmp_bro)->left));
                        PRINTCOLOR(((*tmp_bro)->left));
                    }
					double_rotate += 1;
                }
				
			}

			if(LR_flag == LL)
			{
				printf("Case : LL\n");
				tmp_parent = RR_rotation(*tmp_bro, *tmp_parent, tmp_parent);
			}
			else if(LR_flag == RR)
			{
				printf("Case : RR\n");
				tmp_parent = LL_rotation(*tmp_bro, *tmp_parent, tmp_parent);
			}
			else if(LR_flag == RL)
			{
				printf("Case : RL\n");
				tmp_parent = LR_rotation(*tmp_bro, *tmp_parent, tmp_parent);
			}
			else if(LR_flag == LR)
			{
				printf("Case : LR\n");
				tmp_parent = RL_rotation(*tmp_bro, *tmp_parent, tmp_parent);
			}
			else
			{
				assert(0&&"Exception Error : LR_flag, nephew\n");
			}
			double_rotate += 1;
			// After rotating, color the nodes

			if(parent_color == BLACK)
			{
				// 1(삭제로 인한) - BLACK - 2 인 상태였으면 일단은 balance를 유지하고 싶어하므로 부모 색도 black 유지해야하지 않을까 싶은데
                // 딸려오는애들은 건들지 말자.
				(*tmp_parent)->color = BLACK;
                printf("(*tmp_parent)->color = BLACK\n");
				if (LR_flag == RR && pre_LR_rotate == RIGHT)// RR이면 상위에선 LL rotate, 밑에선 RR rotate
                {
                    (*tmp_parent)->left->color = BLACK;
					(*tmp_parent)->right->color = BLACK; // test7 delete2, uncle의 자식이 R B 일때 당겨져 올라가는애는 BLACK이어야 양쪽이 level 2가 됨.
                    printf("(*tmp_parent)->left->color = BLACK\n");
					printf("(*tmp_parent)->right->color = BLACK\n");
                }
                else if(LR_flag == LL && pre_LR_rotate == LEFT)// RR이면 상위에선 LL rotate, 밑에선 RR rotate
                {
					(*tmp_parent)->left->color = BLACK;
                    (*tmp_parent)->right->color = BLACK;
					printf("(*tmp_parent)->left->color = BLACK\n");
                    printf("(*tmp_parent)->right->color = BLACK\n");
                }
                else if(LR_flag == LL && mine_color == RED)
				{
					(*tmp_parent)->right->color = BLACK;
                    printf("(*tmp_parent)->right->color = BLACK\n");
				}
				else if(LR_flag == LL && mine_color == BLACK) // BLACK이 딸려 올라오면 -1되니까 반대쪽도 -해줘야함.
				{
					(*tmp_parent)->right->color = RED;
                    printf("(*tmp_parent)->right->color = RED\n");
				}
				else if(LR_flag == RR && mine_color == RED)
				{
					(*tmp_parent)->left->color = BLACK;
                    printf("(*tmp_parent)->left->color = BLACK\n");
				}
				else if(LR_flag == RR && mine_color == BLACK)
				{
					(*tmp_parent)->left->color = RED;
                    printf("(*tmp_parent)->left->color = RED\n");
				}
				else if(LR_flag == RR)
				{
					(*tmp_parent)->left->color = BLACK;
					(*tmp_parent)->right->color = BLACK;
                    printf("(*tmp_parent)->left->color = BLACK\n");
				}
				else if(LR_flag == RL)
				{
					(*tmp_parent)->left->color = BLACK;
					//(*tmp_parent)->right->color = BLACK;
                    printf("(*tmp_parent)->left->color = BLACK\n");
				}
				else if(LR_flag == LR)
				{
					//(*tmp_parent)->left->color = BLACK;
					(*tmp_parent)->right->color = BLACK;
                    printf("(*tmp_parent)->right->color = BLACK\n");
				}
				//(*tmp_parent)->left->color = BLACK;
				//(*tmp_parent)->right->color = BLACK;
			}
			else
			{
				// 1 - RED - 2 인 상태였으면 회전한 뒤 1 - B - 1 을 만들고 싶을 것
				// 회전시에 딸려오는 색은 그대로 두자. 그 쪽은 black level property가 깨지지 않았기 때문.

                // 0(삭제) - RED - 1 인 것은 어떻게 처리하지? -> test7에서 나오는데, 이건 위에서 rotation 을 한번 더 해주면 처리됨.
                // 1 - RED - 1로 만들고 싶은지는 어캐알지?
                // 1 - RED - 1이면 회전하면 R - B - R이 되나?
                // RL 회전이나 LR 회전만 예외!!!
                // RR회전이나, LL회전은 RED의 근처는 다 BLACK이라는 특성때문에 회전 시 RBR이 만족함.(물론 RBB가 될 수도 있긴함.)
                
                
                if(LR_flag == RL) 
                {
                    (*tmp_parent)->color = BLACK;
                    printf("(*tmp_parent)->color = BLACK\n");
                    (*tmp_parent)->right->color = RED;
                    printf("(*tmp_parent)->right->color = RED\n");
                }
                else if(LR_flag == LR)
                {
                    (*tmp_parent)->color = BLACK;
                    printf("(*tmp_parent)->color = BLACK\n");
                    (*tmp_parent)->left->color = RED;
                    printf("(*tmp_parent)->left->color = RED\n");
                }
                else if (LR_flag == RR && pre_LR_rotate == RIGHT)// RR이면 상위에선 LL rotate, 밑에선 RR rotate
                {
                    (*tmp_parent)->color = RED;
                    printf("(*tmp_parent)->color = RED\n");
                    (*tmp_parent)->left->color = BLACK;
                    printf("(*tmp_parent)->left->color = BLACK\n");
                }
                else if(LR_flag == LL && pre_LR_rotate == LEFT)// RR이면 상위에선 LL rotate, 밑에선 RR rotate
                {
                    (*tmp_parent)->color = RED;
                    printf("(*tmp_parent)->color = RED\n");
                    (*tmp_parent)->right->color = BLACK;
                    printf("(*tmp_parent)->right->color = BLACK\n");
                }
				
			}

			if(pre_LR_rotate == LEFT || pre_LR_rotate == RIGHT)
			{
				// 두번 회전
				loop_cnt++;
				loop_cnt++;
			}


			

			

			if((*tmp_bro)!=NIL && loop_cnt == 0)
			{
                // 예외 처리를 위한 코드
				
				if((*tmp_bro)->color == BLACK && (*tmp_bro)->parent->color == BLACK)
				{
					// 회전하고 반대쪽은 black level이 -1이므로 우리쪽도 하나 줄여줘야함.
					if(check_child(&(*tmp_bro)->parent)==2)
					{
						(*tmp_bro)->parent->color = RED;
						(*tmp_bro)->parent->parent->color = BLACK;
						return;
					}
				}
				(*tmp_bro)->color = RED;
				printf("(*tmp_bro)->color = RED\n");
			}
			if(parent_color == BLACK)
			{
				if(bro_color == BLACK)
				{
					// 부모, 형제 모두 black이면 회전시에 level이 -1된다.
					// 허나 회전하고 나서 양쪽이 존재하면 BBB로 끝낼 수 있는 예외처리 가능.
					if((*tmp_parent)->right != NIL && (*tmp_parent)->left != NIL)
					{
						if(check_child(&(*tmp_parent)->left)==0 && check_child(&(*tmp_parent)->right)==0)
						{
							(*tmp_parent)->color = BLACK;
							(*tmp_parent)->right->color = BLACK;
							(*tmp_parent)->left->color = BLACK;
							return;
						}
					}
					(*tmp_parent)->color = BLACK;
				}
				(*tmp_parent)->color = BLACK;
			}
			else
			{
				// parent color가 RED였으면 NIL - R - B일텐데 이거 돌리면 RBR처럼 되므로 자식이 없으면!! return
				if(check_child(&(*tmp_parent)->left)==0 && check_child(&(*tmp_parent)->right)==0)
					return; // black level is balanced
				
			}
			if((*tmp_bro)!=NIL)
			{
				if(((*tmp_bro)->left->color == BLACK && (*tmp_bro)->right->color == BLACK) && (*tmp_bro)->color == BLACK)
				{
					// tmp_bro(회전 후 붙은 애)가 자식이 2명이고 부모 RED, tmp_bro가 BLACK이면 색 바꾸기
					(*tmp_bro)->color = RED;
					(*tmp_bro)->parent->color = BLACK;
				}
			}
			
			PRINTDATA((*cursor));
			PRINTDATA((*tmp_bro));
			PRINTCOLOR((*tmp_bro));
			PRINTDATA((*tmp_parent));


			// 여기도 while로 합칠 수 있는지 Concern.
			if((*tmp_bro)!=NIL)
			{
				bro_color = (*tmp_bro)->color;
				printf("회전하고 온 tmp bro가 존재할때\n");

				// bro color가 black이면 회전 안해도 되나?
				if(bro_color == BLACK)
				{
					break;
				}
				else
				{
					(*tmp_bro)->color = BLACK;
				}
				if(LR_flag / 10 == RIGHT)
				{
					if((*tmp_bro)->right != NIL)
					{
						tmp_mine = LL_rotation(*tmp_bro, (*tmp_bro)->parent, find_parent_to_me_ptr(&(*tmp_bro)->parent,root_backup));
					}
					else if((*tmp_bro)->left != NIL)
					{
						tmp_mine = LR_rotation(*tmp_bro, (*tmp_bro)->parent, find_parent_to_me_ptr(&(*tmp_bro)->parent,root_backup));
					}
					else
					{
						// 자식이 없으니 위에서 바꾼대로 하면됨.
                        break;
					}
				}
				else if(LR_flag / 10 == LEFT)
				{
					if((*tmp_bro)->left != NIL)
					{
						tmp_mine = RR_rotation(*tmp_bro, (*tmp_bro)->parent, find_parent_to_me_ptr(&(*tmp_bro)->parent,root_backup));
					}
					else if((*tmp_bro)->right!=NIL)
					{
						tmp_mine = RL_rotation(*tmp_bro, (*tmp_bro)->parent, find_parent_to_me_ptr(&(*tmp_bro)->parent,root_backup));
					}
					else
					{
						break;
					}
				}
				else
				{
					assert(0);
				}
				if((*tmp_mine)->parent->color == BLACK)
				{
					(*tmp_mine)->color = RED;
					(*tmp_mine)->left->color = BLACK;
					(*tmp_mine)->right->color = BLACK;
                    printf("(*tmp_mine)->color = RED\n");
                    printf("(*tmp_mine)->left->color = BLACK\n");
                    printf("(*tmp_mine)->right->color = BLACK\n");
				}
				else
				{
					(*tmp_mine)->color = BLACK;
					(*tmp_mine)->left->color = RED;
					(*tmp_mine)->right->color = RED;
                    printf("(*tmp_mine)->color = BLACK\n");
                    printf("(*tmp_mine)->left->color = RED\n");
                    printf("(*tmp_mine)->right->color = RED\n");
				}
				LR_flag = 0;
				cursor = &(*tmp_mine)->parent->parent;

				PRINTDATA((*tmp_mine));
				PRINTDATA(((*tmp_mine)->parent));
				PRINTCOLOR(((*tmp_mine)->parent));
				// (*tmp_mine) == rotate middle
				if((*tmp_mine)->parent->color == BLACK)
				{
					// 위에서 (*tmp_parent)->color = BLACK;이 이미 되어있음.
					return;//LEVEL 균형 맞추고 끝.
				}
					
			}
			else
			{
				PRINTDATA((*cursor));
				if((*cursor) == (*root_backup))
				{
					return;
				}
				printf("LOOP IS TO BE CONTINUED\n");
				LR_flag = 0;
				cursor = &(*cursor)->parent;
				continue;
			}
		}
		else
		{
			printf("There is No Nephew\n");

			before_no_nephew = 1;
			// 기존 LR flag가 조카를 대비하기 위한 것이었으므로
			// 삼촌 위치 파악하기 위해서 다시 판단해야함.
			printf("RESET the LR flag\n");
			if((*cursor)->parent->left == *cursor)
			{
				LR_flag = 10*LEFT; // cursor의 방향. uncle을 cursor쪽으로 회전시켜야함.
			}
			else
			{
				LR_flag = 10*RIGHT;
			}
			printf("LR flag : %d\n",LR_flag);

			if((*cursor)->color == RED && (*tmp_bro)->color == BLACK)
			{
				if((*tmp_mine)==NIL)//맨 처음 삭제되었을때 예외 처리. 낮은 height에서 발생되므로 return.
				{
					(*tmp_bro)->color = RED;//parent 로 올라가기 전에 black level을 1 줄이는 행위.
					(*cursor)->color = BLACK;//삭제되었을때.
					return;
				}
				
			}
			else if((*cursor)->color == BLACK && (*tmp_bro)->color == BLACK)
			{
				if((*cursor)==(*root_backup)) // 대부분 지우고 node 3개 BBB로 남았을 때 예외처리.
				{
					(*tmp_bro)->color = RED;
				}
				else
				{
					//이 경우는 삼촌을 봐야하는데.. 그냥 회전할까?
					// 삼촌보는 경우는 recur이 안됨.
					// 삼촌에서 내쪽으로 회전하자.
					printf("NIL B B case - LR flag : %d\n",LR_flag);
					// LR_flag 갱신해야 함. bro의 방향만 저장되어있기 때문에 부모가 조부모로부터 어디방향인지는 모름.
					(*tmp_bro)->color = RED;
					printf("bro will be RED before rotating\n");
					//회전하기 전에 RED만들기

				}
			}
			else
			{
			
			}
			LR_flag = 0;
			
			tmp_mine = cursor;
			cursor = &(*cursor)->parent;
			PRINTDATA((*cursor));
			PRINTDATA((*tmp_mine));
		}
		if(parent_color == RED)
		{
			// parent node의 color가 RED이면 색 변경을 하든 아래에서 끝내든 끝낼수 있음.
			break;
		}
		else
		{
			// parent를 타고 go할때 BLACK이 연달아서 나오는게 문제.
			printf("LOOP IS TO BE CONTINUED\n");
		}
		
		loop_cnt++;
	}
}

void redblack_property_test(redblack** root);
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

        if(tmp->color == RED)
		{
			printf("color = RED\t");
		}
        else if(tmp->data == -1)
        {
            printf("\t\t");
        }
		else
		{
			printf("color = BLACK\t");
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
			//printf("left = NULL\t");
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
			//printf("right = NULL\t");
		}

		if(tmp->parent)
		{
			if(tmp->parent->data == -1)
			{
				printf("parent = ROOT\t");
				printf("parent color = BLACK\t");
			}
			else
			{
				printf("parent = %4d\t", tmp->parent->data);
				if(tmp->parent->color == BLACK)
				{
					printf("parent color = BLACK\t");
				}
				else
				{
					printf("parent color = RED\t");
				}
			}
		}
		else
		{
			//printf("parent = NULL\t");
			//printf("parent color = NULL\t");
		}
		
        if(tmp->data != -1)
		    printf("black level = %4d\n", tmp->black_level);
        else
            printf("\n");
		
		print_redblack(tmp->right);
		redblack_property_test(&tmp);
	}
}
void print_redblack_for_check(redblack *tree)
{
	redblack* tmp = tree;

	if(tmp)
	{
		print_redblack_for_check(tmp->left);
		if(tmp->data == -1)
			printf("data = NIL\t");
		else
			printf("data = %4d\t", tmp->data);

		if(tmp->parent)
		{
			if(tmp->parent->data == -1)
			{
				printf("parent = NIL\t");
				printf("parent color = BLACK\t");
			}
			else
			{
				printf("parent = %4d\t", tmp->parent->data);
				if(tmp->parent->color == BLACK)
				{
					printf("parent color = BLACK\t");
				}
				else
				{
					printf("parent color = RED\t");
				}
			}
		}
		else
		{
			printf("parent = NULL\t");
			printf("parent color = NULL\t");
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

		printf("black level = %4d\t", tmp->black_level);
		
		if(tmp->color == RED)
		{
			printf("color = RED\n");
		}
		else
		{
			printf("color = BLACK\n");
		}
		print_redblack_for_check(tmp->right);
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
		{
			//PRINTDATA((*root)->parent);
			assert((*root)->parent->color == BLACK);
		}
		if((*root)->left)
		{
			assert((*root)->left->color == BLACK);
		}
		if((*root)->right)
		{
			assert((*root)->right->color == BLACK);
		}
	}
	// 3. Same black_level
	// compare left and right black level

	if((*root)->right && (*root)->left)
	{
		assert((*root)->right->black_level == (*root)->left->black_level);
	}	
}

void repeat_insert_delete(redblack** root, int* data, int len)
{
	int i;
	for (i = 0; i < len; i++)
    {
		//printf("insert data : %d\n", data[i]);
		insert_redblack(root, data[i]);
		//printf("Now root is %d\n",(*root)->data);
		// property 1 : The color of root node is BLACK
    	assert(((*root) ? (*root)->color : BLACK) == BLACK);
		//print_redblack(*root);
    }
    // property 1 : The color of root node is BLACK
    assert(((*root) ? (*root)->color : BLACK) == BLACK);
    printf("The color of root node is BLACK\n");
    print_redblack(*root);

    for(i = 0; i < len; i++)
    {
    	printf("delete data : %d\n", data[i]);
    	delete_redblack(root, data[i]);
		recur_update_black_level(root);
		if((*root)!=NULL)
    		printf("Now root is %d\n",(*root)->data);
    	print_redblack(*root);
		// property 1 : The color of root node is BLACK
    	assert(((*root) ? (*root)->color : BLACK) == BLACK);
    }
    assert(((*root) ? (*root)->color : BLACK) == BLACK);
    printf("The color of root node is BLACK\n");
    print_redblack(*root);
}

void test1(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	int len;

	data[0] = 6;
	data[1] = 10;
	data[2] = 2;
	data[3] = 8;
	data[4] = 7;
	data[5] = 5;
	data[6] = 9;
	data[7] = 1;
	data[8] = 4;
	data[9] = 3;
	len = 10;
	print_arr(data, len);

	repeat_insert_delete(root, data, len);

}

void test2(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	int len;

	data[0] = 9;
	data[1] = 3;
	data[2] = 10;
	data[3] = 5;
	data[4] = 8;
	data[5] = 7;
	data[6] = 4;
	data[7] = 1;
	data[8] = 2;
	data[9] = 6;
	len = 10;
	print_arr(data, len);
	repeat_insert_delete(root, data, len);

}

void test3(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	//부모 BLACK, 형제 RED, 삼촌 X, 조카의 자식이 2명
	// 2 삭제에서 회전 2번해야 하는 케이스.
	// 형제가 RED면 자식이 반드시 BLACK이어야하므로 회전시 붙는 애는 BLACK이다.

	int len;

	data[0] = 9;
	data[1] = 3;
	data[2] = 2;
	data[3] = 1;
	data[4] = 5;
	data[5] = 7;
	data[6] = 10;
	data[7] = 6;
	data[8] = 4;
	data[9] = 8;
	len = 10;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);

}

void test4(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	//부모 BLACK, 형제 RED, 삼촌 X
	// 1 삭제에서 회전 2번해야 하는 케이스.

	int len;

	data[0] = 3;
	data[1] = 8;
	data[2] = 4;
	data[3] = 1;
	data[4] = 9;
	data[5] = 2;
	data[6] = 6;
	data[7] = 7;
	data[8] = 10;
	data[9] = 5;
	len = 10;
	print_arr(data, len);

	repeat_insert_delete(root, data, len);

}

void test5(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	//조카 있고, 부모 BLACK, 삼촌 BLACK, 형제 BLACK
	//삼촌도 봐야하나?
	
	int len;

	data[0] = 16;
	data[1] = 15;
	data[2] = 14;
	data[3] = 11;
	data[4] = 9;
	data[5] = 1;
	data[6] = 18;
	data[7] = 20;
	data[8] = 12;
	data[9] = 10;
	data[10] = 6;
	data[11] = 13;
	data[12] = 4;
	data[13] = 8;
	data[14] = 19;
	data[15] = 7;
	data[16] = 17;
	data[17] = 2;
	data[18] = 3;
	data[19] = 5;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);


}

void test6(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	int len;

	data[0] = 4;
	data[1] = 6;
	data[2] = 10;
	data[3] = 9;
	data[4] = 8;
	data[5] = 1;
	data[6] = 3;
	data[7] = 5;
	data[8] = 2;
	data[9] = 7;
	len = 10;
	
	repeat_insert_delete(root, data, len);

}

void test7(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	//조카 없고, 부모 BLACK, 삼촌 BLACK, 형제 BLACK

	
	int len;

	data[0] = 11;
	data[1] = 8;
	data[2] = 4;
	data[3] = 18;
	data[4] = 5;
	data[5] = 9;
	data[6] = 12;
	data[7] = 1;
	data[8] = 3;
	data[9] = 7;
	data[10] = 13;
	data[11] = 16;
	data[12] = 14;
	data[13] = 6;
	data[14] = 17;
	data[15] = 19;
	data[16] = 10;
	data[17] = 20;
	data[18] = 2;
	data[19] = 15;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);


}

void test8(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	//조카 없고, 부모 BLACK, 삼촌 RED, 형제 BLACK
	// test8 bro하고 uncle의 색만 바꾸면 됨. (delete 4할때)
	
	int len;

	data[0] = 5;
	data[1] = 7;
	data[2] = 8;
	data[3] = 16;
	data[4] = 3;
	data[5] = 15;
	data[6] = 10;
	data[7] = 17;
	data[8] = 4;
	data[9] = 13;
	data[10] = 6;
	data[11] = 20;
	data[12] = 18;
	data[13] = 19;
	data[14] = 9;
	data[15] = 2;
	data[16] = 12;
	data[17] = 1;
	data[18] = 14;
	data[19] = 11;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);


}

void test9(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);

	
	int len;

	data[0] = 2;
	data[1] = 20;
	data[2] = 4;
	data[3] = 15;
	data[4] = 1;
	data[5] = 14;
	data[6] = 7;
	data[7] = 5;
	data[8] = 11;
	data[9] = 8;
	data[10] = 10;
	data[11] = 12;
	data[12] = 13;
	data[13] = 3;
	data[14] = 19;
	data[15] = 17;
	data[16] = 16;
	data[17] = 6;
	data[18] = 9;
	data[19] = 18;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);


}

void test10(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);

	
	int len;

	data[0] = 12;
	data[1] = 9;
	data[2] = 7;
	data[3] = 8;
	data[4] = 3;
	data[5] = 6;
	data[6] = 5;
	data[7] = 16;
	data[8] = 14;
	data[9] = 2;
	data[10] = 18;
	data[11] = 19;
	data[12] = 1;
	data[13] = 10;
	data[14] = 17;
	data[15] = 11;
	data[16] = 13;
	data[17] = 4;
	data[18] = 20;
	data[19] = 15;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test11(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	// 조카 없고 조부모 RED
	
	int len;

	data[0] = 20;
	data[1] = 5;
	data[2] = 13;
	data[3] = 2;
	data[4] = 17;
	data[5] = 18;
	data[6] = 19;
	data[7] = 10;
	data[8] = 8;
	data[9] = 3;
	data[10] = 11;
	data[11] = 16;
	data[12] = 4;
	data[13] = 6;
	data[14] = 1;
	data[15] = 12;
	data[16] = 15;
	data[17] = 9;
	data[18] = 14;
	data[19] = 7;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test12(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 8;
	data[1] = 10;
	data[2] = 9;
	data[3] = 7;
	data[4] = 5;
	data[5] = 3;
	data[6] = 14;
	data[7] = 11;
	data[8] = 16;
	data[9] = 19;
	data[10] = 12;
	data[11] = 6;
	data[12] = 2;
	data[13] = 20;
	data[14] = 15;
	data[15] = 1;
	data[16] = 13;
	data[17] = 18;
	data[18] = 17;
	data[19] = 4;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test13(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 8;
	data[1] = 12;
	data[2] = 18;
	data[3] = 6;
	data[4] = 11;
	data[5] = 5;
	data[6] = 3;
	data[7] = 1;
	data[8] = 17;
	data[9] = 14;
	data[10] = 9;
	data[11] = 16;
	data[12] = 7;
	data[13] = 10;
	data[14] = 19;
	data[15] = 13;
	data[16] = 2;
	data[17] = 4;
	data[18] = 15;
	data[19] = 20;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test14(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 5;
	data[1] = 6;
	data[2] = 7;
	data[3] = 4;
	data[4] = 11;
	data[5] = 1;
	data[6] = 20;
	data[7] = 9;
	data[8] = 18;
	data[9] = 16;
	data[10] = 3;
	data[11] = 14;
	data[12] = 15;
	data[13] = 10;
	data[14] = 17;
	data[15] = 8;
	data[16] = 12;
	data[17] = 13;
	data[18] = 2;
	data[19] = 19;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test15(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] =19;
	data[1] = 8;
	data[2] = 16;
	data[3] = 7;
	data[4] = 6;
	data[5] = 12;
	data[6] = 17;
	data[7] = 11;
	data[8] = 18;
	data[9] = 4;
	data[10] = 15;
	data[11] = 2;
	data[12] = 20;
	data[13] = 13;
	data[14] = 10;
	data[15] = 9;
	data[16] = 1;
	data[17] = 3;
	data[18] = 14;
	data[19] = 5;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}


void test16(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 1;
	data[1] = 9;
	data[2] = 6;
	data[3] = 17;
	data[4] = 8;
	data[5] = 3;
	data[6] = 2;
	data[7] = 12;
	data[8] = 18;
	data[9] = 13;
	data[10] = 20;
	data[11] = 15;
	data[12] = 14;
	data[13] = 10;
	data[14] = 5;
	data[15] = 4;
	data[16] = 16;
	data[17] = 11;
	data[18] = 19;
	data[19] = 7;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test17(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 16;
	data[1] = 17;
	data[2] = 3;
	data[3] = 11;
	data[4] = 6;
	data[5] = 1;
	data[6] = 20;
	data[7] = 5;
	data[8] = 15;
	data[9] = 13;
	data[10] = 2;
	data[11] = 8;
	data[12] = 18;
	data[13] = 4;
	data[14] = 19;
	data[15] = 9;
	data[16] = 7;
	data[17] = 10;
	data[18] = 12;
	data[19] = 14;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test18(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 14;
	data[1] = 1;
	data[2] = 17;
	data[3] = 15;
	data[4] = 16;
	data[5] = 2;
	data[6] = 12;
	data[7] = 5;
	data[8] = 11;
	data[9] = 13;
	data[10] = 19;
	data[11] = 7;
	data[12] = 4;
	data[13] = 20;
	data[14] = 18;
	data[15] = 8;
	data[16] = 9;
	data[17] = 10;
	data[18] = 3;
	data[19] = 6;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test19(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 19;
	data[1] = 15;
	data[2] = 17;
	data[3] = 14;
	data[4] = 18;
	data[5] = 2;
	data[6] = 4;
	data[7] = 5;
	data[8] = 7;
	data[9] = 8;
	data[10] = 20;
	data[11] = 3;
	data[12] = 13;
	data[13] = 12;
	data[14] = 10;
	data[15] = 9;
	data[16] = 1;
	data[17] = 16;
	data[18] = 11;
	data[19] = 6;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test20(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 14;
	data[1] = 1;
	data[2] = 15;
	data[3] = 17;
	data[4] = 19;
	data[5] = 5;
	data[6] = 4;
	data[7] = 10;
	data[8] = 7;
	data[9] = 20;
	data[10] = 16;
	data[11] = 13;
	data[12] = 2;
	data[13] = 18;
	data[14] = 11;
	data[15] = 12;
	data[16] = 3;
	data[17] = 8;
	data[18] = 6;
	data[19] = 9;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}
void test21(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 18;
	data[1] = 4;
	data[2] = 20;
	data[3] = 5;
	data[4] = 8;
	data[5] = 3;
	data[6] = 13;
	data[7] = 15;
	data[8] = 1;
	data[9] = 17;
	data[10] = 12;
	data[11] = 11;
	data[12] = 6;
	data[13] = 10;
	data[14] = 2;
	data[15] = 9;
	data[16] = 19;
	data[17] = 14;
	data[18] = 7;
	data[19] = 16;
	len = 20;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test22(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	
	
	int len;

	data[0] = 26;
	data[1] = 11;
	data[2] = 10;
	data[3] = 1;
	data[4] = 43;
	data[5] = 14;
	data[6] = 50;
	data[7] = 2;
	data[8] = 20;
	data[9] = 3;
	data[10] = 46;
	data[11] = 25;
	data[12] = 45;
	data[13] = 9;
	data[14] = 18;
	data[15] = 24;
	data[16] = 12;
	data[17] = 29;
	data[18] = 31;
	data[19] = 17;
	data[20] = 16;
	data[21] = 21;
	data[22] = 5;
	data[23] = 8;
	data[24] = 22;
	data[25] = 39;
	data[26] = 47;
	data[27] = 32;
	data[28] = 49;
	data[29] = 15;
	data[30] = 6;
	data[31] = 13;
	data[32] = 42;
	data[33] = 19;
	data[34] = 35;
	data[35] = 7;
	data[36] = 36;
	data[37] = 37;
	data[38] = 38;
	data[39] = 44;
	data[40] = 28;
	data[41] = 4;
	data[42] = 48;
	data[43] = 33;
	data[44] = 34;
	data[45] = 41;
	data[46] = 40;
	data[47] = 23;
	data[48] = 30;
	data[49] = 27;

	len = 50;
	print_arr(data, len);
	
	repeat_insert_delete(root, data, len);
}

void test_random(redblack** root, int* data, int len)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);


	
	repeat_insert_delete(root, data, len);
}

int main()
{
    redblack *root = NULL;
 
    int data[50] = { 0 };
	int len = sizeof(data) / sizeof(int);


	test1(&root, data);
	test2(&root, data);
	test3(&root, data);
	test4(&root, data);
	test5(&root, data);
	test6(&root, data);
	test7(&root, data);
	test8(&root, data);
	test9(&root, data);
	test10(&root, data);
	test11(&root, data);
	test12(&root, data);
	test13(&root, data);
	test14(&root, data);
	test15(&root, data);
	test16(&root, data);
	test17(&root, data);
	test18(&root, data);
	test19(&root, data);
	test20(&root, data);
	test21(&root, data);
	test22(&root, data);
	test_random(&root, data, len);

}

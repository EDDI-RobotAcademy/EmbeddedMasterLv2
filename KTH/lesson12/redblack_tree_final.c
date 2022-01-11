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
	if((*root)->data == -1)
	{
		return root_backup;
		// cursor가 올라가서 root에 도달했을때 tmp_parent 넣는곳 에러처리.
	}
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

	bool start = true;
	bool state_after_rotation_search_bro = false;

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
	while(start || (*cursor)!=(*root_backup))
	{
		printf("while start\n");
		if(start)
		{
			if((*cursor) == (*root_backup)) // test1 1 3 4 9 에서 9 삭제 후 cursor가 root를 가리킬때
			{
				tmp_parent = cursor;
				cursor = &NIL;
			}
			else
				tmp_parent = tmp_backup;
		}

		if((*tmp_parent)->left == *cursor)
		{
			tmp_bro = &(*tmp_parent)->right;
			LR_flag += 10*RIGHT; // BRO의 방향
		}
		else if((*tmp_parent)->right == *cursor)
		{
			tmp_bro = &(*tmp_parent)->left;
			LR_flag += 10*LEFT; // BRO의 방향
		}
		else
		{
			assert(0);
		}
		
		printf("After setting\n");
		PRINTDATA((*tmp_parent));
		PRINTDATA((*tmp_bro));
		PRINTDATA((*cursor));

		if((*cursor)->color == BLACK)
		{
			printf("(*cursor)->color == BLACK\n");
			if((*tmp_parent)->color == RED)
			{
				printf("(*tmp_parent)->color == RED\n");
				if((*tmp_bro)->color == BLACK)
				{
					printf("BRB\n");
					// B R B -> 조카 봐야함
					if((*tmp_bro)->left != NIL && (*tmp_bro)->right != NIL)
					{
						if((*tmp_bro)->left->color == BLACK && (*tmp_bro)->right->color == BLACK)
						{
							//처음 삭제했을땐 여기 못들어옴.
							//삭제시에 black level이 0이 되므로 반대는 1인데 BRB이므로 조카는 무조건 RED
							// while을 반복할때 들어오는 곳.
							// cursor는 black node이고 black level이 1 부족한 상태.
							// 형제 색을 RED로 바꾸고(양 자식이 BLACK이라 위반이 아님), 부모 색을 검정으로 바꾸면 종료 가능.
							(*tmp_parent)->color = BLACK;
							(*tmp_bro)->color = RED;
							break;
						}
						else if((*tmp_bro)->left->color == RED && (*tmp_bro)->right->color == BLACK)
						{
							// RED 가 있는 쪽이 더 height가 클 확률이 높다.
							// RED 있는 쪽을 LR_flag에 넣자.
							printf("(*tmp_bro)->left->color == RED && (*tmp_bro)->right->color == BLACK\n");
							#if 1
							LR_flag += LEFT; // RED가 있는 곳이 길다.
							if(LR_flag == LL)
							{
								RR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
							}
							else if(LR_flag == RL)
							{
								LR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
								(*tmp_parent)->left->color = BLACK;
								// 위에가 BRB였는데 Left rotation하면 R이 왼쪽으로 내려오게됨.
								PRINTDATA(((*tmp_parent)->right));
							}
							else
							{
								assert(0);
							}
							// 그냥 회전해도 RBR이 된다. 부모 RED bro가 B였고 B의 right가 RED였으므로
							#endif
							
							break;
						}
						else if((*tmp_bro)->left->color == BLACK && (*tmp_bro)->right->color == RED)
						{
							printf("(*tmp_bro)->left->color == BLACK && (*tmp_bro)->right->color == RED\n");
							#if 1
							LR_flag += RIGHT; // RED가 있는 곳이 길다.
							if(LR_flag == RR)
							{
								LL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
							}
							else if(LR_flag == LR)
							{
								RL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
								(*tmp_parent)->right->color = BLACK;
								PRINTDATA(((*tmp_parent)->right));
							}
							else
							{
								assert(0);
							}
							// 그냥 회전해도 RBR이 된다. 부모 RED bro가 B였고 B의 right가 RED였으므로
							#endif
							
							break;
						}
						else if((*tmp_bro)->left->color == RED && (*tmp_bro)->right->color == RED)
						{
							//test 9 delete 20
							if(start)
							{
								if(LR_flag/10 == LEFT)
								{
									RR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
								}
								else if(LR_flag/10 == RIGHT)
								{
									LL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
								}
								else
								{
									assert(0);
								}
								(*tmp_parent)->color = RED;
								(*tmp_parent)->left->color = BLACK;
								(*tmp_parent)->right->color = BLACK;
							}
							else
							{
								if(LR_flag/10 == LEFT)
								{
									RR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
									(*tmp_parent)->color = RED;
									(*tmp_parent)->left->color = BLACK;
									(*tmp_parent)->right->color = BLACK;
									PRINTDATA((*tmp_parent));
								}
								else if(LR_flag/10 == RIGHT)
								{
									LL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
									(*tmp_parent)->color = RED;
									(*tmp_parent)->left->color = BLACK;
									(*tmp_parent)->right->color = BLACK;
									PRINTDATA((*tmp_parent));
								}
								else
								{
									assert(0);
								}
								
								
							}
							break;
						}
						else
						{
							assert(0);
						}
					}
					else if((*tmp_bro)->left != NIL && (*tmp_bro)->right == NIL)
					{
						LR_flag += LEFT;
						if(LR_flag == LL)
						{
							RR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						}
						else if(LR_flag == RL)
						{
							LR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						}
						else
						{
							assert(0);
						}
						(*tmp_parent)->color = RED; // root일리는 없음. 회전하기 전 부모의 색이 RED이므로.
						(*tmp_parent)->left->color = BLACK;
						(*tmp_parent)->right->color = BLACK;
						break;
					}
					else if((*tmp_bro)->left == NIL && (*tmp_bro)->right != NIL)
					{
						LR_flag += RIGHT;
						if(LR_flag == RR)
						{
							LL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						}
						else if(LR_flag == LR)
						{
							RL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						}
						else
						{
							assert(0);
						}
						(*tmp_parent)->color = RED;
						(*tmp_parent)->left->color = BLACK;
						(*tmp_parent)->right->color = BLACK;
						break;
					}
					else if((*tmp_bro)->left == NIL && (*tmp_bro)->right == NIL)
					{
						(*tmp_bro)->color = RED;
						(*tmp_parent)->color = BLACK;
						break;
					}	
				}
				else if((*tmp_bro)->color == RED)
				{
					// RED RED는 불가능.
					assert(0&&"RED RED is impossible\n");
				}
				else
				{
					assert(0&&"Error!\n");
				}
			}
			else if((*tmp_parent)->color == BLACK)
			{
				if((*tmp_bro)->color == BLACK)
				{
					printf("BBB\n");
					// B B B -> 조카 봐야함.
					// 조카 있을때와 없을때 나누기
					// 조카 없을때 형제 red만들고 올라가야함.

					if((*tmp_bro)->left != NIL && (*tmp_bro)->right != NIL)
					{
						if((*tmp_bro)->left->color == BLACK && (*tmp_bro)->right->color == BLACK)
						{
							printf("Nephew two BLACK\n");
							// 처음 삭제했을땐 여기 못들어옴.
							// 삭제시에 black level이 0이 되므로 반대는 1인데 BRB이므로 조카는 무조건 RED
							// while을 반복할때 들어오는 곳.
							// cursor는 black node이고 black level이 1 부족한 상태.
							// 형제 색을 RED로 바꾸고(양 자식이 BLACK이라 위반이 아님), 부모 색을 그대로 두면 자식들 균형은 맞추고 부모로 커서를 이동.
							(*tmp_bro)->color = RED;

							
							cursor = tmp_parent;
							PRINTDATA(((*cursor)->parent));
							tmp_parent = find_parent_to_me_ptr(&(*cursor)->parent,root_backup); // 이렇게 해야 정확히 left or right ptr이 삽입됨.
							
							start = false;
							LR_flag = 0;
							PRINTDATA((*cursor));
							PRINTDATA((*tmp_parent));
							printf("CONTINUE\n");
							continue;
						}
						else if((*tmp_bro)->left->color == RED && (*tmp_bro)->right->color == BLACK)
						{
							// 크게 회전해야 하는 케이스
							// R 쪽이 더 깊을 확률이 높으니까 안쪽에 있으면 밖으로 회전했다가 크게 회전 (RL rotation)
							printf("NEPHEW RED // BLACK(start case) OR COUSIN RED // BLACK\n");
							if(LR_flag/10 == LEFT)
							{
								RR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
								// RED가 딸려 올라오는데..
								(*tmp_parent)->left->color = BLACK;
							}
							else if(LR_flag/10 == RIGHT)
							{
								LR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
							}
							else
							{
								assert(0);
							}
							(*tmp_parent)->color = BLACK;
							break;

						}
						else if((*tmp_bro)->left->color == BLACK && (*tmp_bro)->right->color == RED)
						{
							// 크게 회전해야 하는 케이스 test8 delete 20
							printf("NEPHEW BLACK // RED(start case) OR COUSIN BLACK // RED\n");
							if(LR_flag/10 == LEFT)
							{
								RL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
							}
							else if(LR_flag/10 == RIGHT)
							{
								LL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
								(*tmp_parent)->right->color = BLACK;
							}
							else
							{
								assert(0);
							}
							(*tmp_parent)->color = BLACK;
							break;
						}
						else if((*tmp_bro)->left->color == RED && (*tmp_bro)->right->color == RED)
						{
							printf("NEPHEW RED // RED(start case) OR COUSIN RED // RED\n");
							if(LR_flag/10 == LEFT)
							{
								RR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
							}
							else if(LR_flag/10 == RIGHT)
							{
								LL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
							}
							else
							{
								assert(0);
							}
							if(state_after_rotation_search_bro) // 회전하고 나서 bro 탐색하는 경우
							{
								(*tmp_parent)->color = RED;
								(*tmp_parent)->left->color = BLACK;
								(*tmp_parent)->right->color = BLACK;
								// 회전 후 여기로 오거나, 삭제 후 여기로 왔을때 비 대칭인 상태에서 회전하면
								// NIL BBR -> 회전
								// BRB가 되어야 RED violation도 안생기고 black level도 1로 유지됨.
							}
							else // parent로 올라가다가 만난 경우
							{
								//당겨 올라오는쪽을 BLACK으로 바꾸어야함
								//case 7 delete 3
								if(LR_flag/10 == LEFT)
								{
									(*tmp_parent)->left->color = BLACK;
								}
								else if(LR_flag/10 == RIGHT)
								{
									(*tmp_parent)->right->color = BLACK;
								}
								else
								{
									assert(0);
								}
							}
							break;
						}
						else
						{
							assert(0);
						}
					}
					else if((*tmp_bro)->left != NIL && (*tmp_bro)->right == NIL)
					{
						printf("(*tmp_bro)->left != NIL && (*tmp_bro)->right == NIL\n");
						LR_flag += LEFT;
						if(LR_flag == LL)
						{
							RR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						}
						else if(LR_flag == RL)
						{
							LR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						}
						else
						{
							assert(0);
						}
						(*tmp_parent)->color = BLACK; // root일 수 있음.
						if(start)
						{
							(*tmp_parent)->left->color = BLACK;
							(*tmp_parent)->right->color = BLACK;
						}
						else // 회전 후 bro를 다시 볼때 오는 경우
						{
							(*tmp_parent)->left->color = RED;
							(*tmp_parent)->right->color = RED;
						}
						// 깊이가 깊어질때 조카가 없는경우는 없을듯.
						break;
					}	
					else if((*tmp_bro)->left == NIL && (*tmp_bro)->right != NIL)
					{
						printf("(*tmp_bro)->left == NIL && (*tmp_bro)->right != NIL\n");
						LR_flag += RIGHT;
						if(LR_flag == RR)
						{
							LL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						}
						else if(LR_flag == LR)
						{
							RL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						}
						else
						{
							assert(0);
						}
						if(start)
						{
							// 3~4개 node있는 케이스는 어떻게 처리하지?
							(*tmp_parent)->color = BLACK; 
							(*tmp_parent)->left->color = RED;
							(*tmp_parent)->right->color = RED;
							// start에서는 cursor가 NIL인데..
							PRINTDATA((*cursor));
							PRINTDATA((*tmp_parent));
							
							cursor = tmp_parent;
							tmp_parent = find_parent_to_me_ptr(&(*tmp_parent)->parent,root_backup);
							start = false;
							LR_flag = 0;
							continue;
						}
						else
						{
							//회전하고 들어오는 케이스.
							//test10 delete 7
							#if 0
							(*tmp_parent)->color = BLACK; 
							(*tmp_parent)->left->color = RED;
							(*tmp_parent)->right->color = RED;
							#endif
							#if 1
							(*tmp_parent)->color = RED; 
							(*tmp_parent)->left->color = BLACK;
							(*tmp_parent)->right->color = BLACK;
							#endif
							break;
							
						}
					}
					else if((*tmp_bro)->left == NIL && (*tmp_bro)->right == NIL)
					{
						printf("THERE IS NO NEPHEW\n");
						// 초기 case.
						(*tmp_bro)->color = RED;
						PRINTDATA((*tmp_bro));
						printf("(*tmp_bro)->color = RED;\n");
						PRINTDATA((*tmp_parent));
						if(start)
							cursor = tmp_parent;
						else
						{
							cursor = &(*tmp_parent)->parent;
							//break; // 회전하고 나서 tmp_bro를 볼 때 온 곳이라면 끝내야함. start가 아니라면 저 초기 case가 아니고 여기 올 경우는 회전하고 나서 bro 탐색하는 경우 밖에 없음.
						}
						tmp_parent = find_parent_to_me_ptr(&(*cursor)->parent,root_backup);
						start = false;
						LR_flag = 0;
						PRINTDATA((*cursor));
						PRINTDATA((*tmp_parent));
						printf("CONTINUE\n");
						if(state_after_rotation_search_bro) // 회전하고 나서 bro 탐색한 경우 끝내야함.
						{
							break;
						}
						continue;
					}
					else
					{

					}
				}
				else if((*tmp_bro)->color == RED)
				{
					printf("BBR\n");
					// B B R -> 회전
					// 조카 볼 필요 없이 회전
					// R은 무조건 BB 자식이 있어야하기때문. black level 만족하려면.
					
					if(LR_flag / 10 == LEFT)// 조카를 안보니까 /10해야함. bro의 방향임
					{
						RR_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						(*tmp_parent)->color = BLACK;
						if(!start)
							(*tmp_parent)->right->color = RED;
						tmp_parent = &(*tmp_parent)->right;
					}
					else if(LR_flag / 10 == RIGHT)
					{
						LL_rotation((*tmp_bro),(*tmp_parent),tmp_parent);
						(*tmp_parent)->color = BLACK;
						if(!start)
							(*tmp_parent)->left->color = RED;
						// START일 때는 NIL B R(BB)이고 START가 아닐 땐 B B R(BB)라서 좀 다르다.
						tmp_parent = &(*tmp_parent)->left;
					}
					else
					{
						assert(0);
					}
					// RED를 잡아 올려서 회전시켰으므로 오는건 BLACK이고 black level은 동일.
					// 붙는 애도 BLACK이니까 왼쪽도 BLACK level 동일하게 맞추려면 내려가는 애 RED로 만들고 middle을 BLACK로 만들면 된다.
					
					
					start = false;
					LR_flag = 0;
					PRINTDATA((*tmp_parent));
					PRINTDATA((*tmp_bro));
					PRINTDATA((*cursor));
					printf("GO TO set tmp_bro\n");
					printf("state_after_rotation_search_bro = true\n");
					state_after_rotation_search_bro = true;
					continue;

					break;
				}
				else
				{
					assert(0&&"Error!\n");
				}

			}
			else
			{
				assert(0&&"Error!\n");
			}
		}
		else if((*cursor)->color == RED) //삭제하고 났을때는 NIL이므로 BLACK, 삭제 후에 while문을 타고 parent로 이동하면 항상 BLACK이 아닐까?
		{
			// loop를 돌때는 BBB인 케이스이므로 
			// cursor가 RED인 케이스는 3~4개 node남았을때 회전하고 RBR만들었을때.
			break;
			assert(0&&"cursor must be BLACK\n");
		}
		else
		{
			assert(0&&"Error\n");
		}
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
#if 0
		printf("(*root)->data = %d\n",(*root)->data);
		printf("(*root)->right->data = %d\n",(*root)->right->data);
		printf("(*root)->left->data = %d\n",(*root)->left->data);
		printf("(*root)->black_level = %d\n",(*root)->black_level);
		printf("(*root)->right->black_level = %d\n",(*root)->right->black_level);
		printf("(*root)->left->black_level = %d\n",(*root)->left->black_level);
#endif
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
    	//print_redblack(*root);
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

void test23(redblack** root, int* data)
{
	printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////%s\n",__func__);
	

	int len;

	data[0] = 17;
	data[1] = 26;
	data[2] = 11;
	data[3] = 4;
	data[4] = 21;
	data[5] = 40;
	data[6] = 33;
	data[7] = 14;
	data[8] = 39;
	data[9] = 25;
	data[10] = 29;
	data[11] = 43;
	data[12] = 49;
	data[13] = 32;
	data[14] = 47;
	data[15] = 44;
	data[16] = 50;
	data[17] = 7;
	data[18] = 37;
	data[19] = 42;
	data[20] = 24;
	data[21] = 15;
	data[22] = 41;
	data[23] = 31;
	data[24] = 16;
	data[25] = 34;
	data[26] = 27;
	data[27] = 20;
	data[28] = 46;
	data[29] = 5;
	data[30] = 22;
	data[31] = 30;
	data[32] = 19;
	data[33] = 18;
	data[34] = 3;
	data[35] = 12;
	data[36] = 6;
	data[37] = 35;
	data[38] = 8;
	data[39] = 28;
	data[40] = 45;
	data[41] = 1;
	data[42] = 38;
	data[43] = 9;
	data[44] = 36;
	data[45] = 48;
	data[46] = 13;
	data[47] = 10;
	data[48] = 2;
	data[49] = 23;

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
 
  int data[10000] = { 0 };
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
	test23(&root, data);
	test_random(&root, data, len);

}

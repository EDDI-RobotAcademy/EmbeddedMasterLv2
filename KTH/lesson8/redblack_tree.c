#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define LEVEL(X) ((X) ? X->black_level : 0)
#define PRINTDATA(X) (printf(#X " data : %d\n" , X->data))
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

int balance_factor(redblack **root)
{
	return LEVEL((*root)->left) - LEVEL((*root)->right);
} 

void check_black_property(redblack **root)
{
#if 0
	if(LEVEL((*root)->left) != LEVEL((*root)->right))
		rotate(root);
#endif
}


void LL_rotation(redblack *rotate_middle, redblack *rotate_bro, redblack **tmp_parent)
{//RR case에서 왼쪽으로 회전하는걸 LL_rotation이라고 함.
// double pointer로 하면 *rotate_middle = *tmp_bro가 되어버리는 불상사..      
	printf("LL rotation\n");


	rotate_middle->parent = rotate_bro->parent;
	rotate_bro->parent = rotate_middle;
	rotate_bro->right = rotate_middle->left;

	
	if(rotate_middle->left->data != -1)
	{
		rotate_middle->left->parent = rotate_bro;
		if(rotate_bro->right->left->color != RED && rotate_bro->right->right->color != RED)
			rotate_bro->right->color = RED;
			//이렇게 바꿨는데 자식이 red이면 ERROR.
	}
	rotate_middle->left = rotate_bro;
	*tmp_parent = rotate_middle;

	rotate_middle->color = RED;
	rotate_middle->left->color = BLACK;
	rotate_middle->right->color = BLACK;

	PRINTDATA(rotate_middle);

	update_black_level(&rotate_middle->left);
	update_black_level(&rotate_middle->right);
	update_black_level(&rotate_middle);
}

void LR_rotation(redblack *rotate_middle, redblack *rotate_bro, redblack **tmp_parent)
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
		if(rotate_bro->right->left->color != RED && rotate_bro->right->right->color != RED)
			rotate_bro->right->color = RED;
			//이렇게 바꿨는데 자식이 red이면 ERROR.
	}
	rotate_middle->left = rotate_bro;
	*tmp_parent = rotate_middle;

	rotate_middle->color = RED;
	rotate_middle->left->color = BLACK;
	rotate_middle->right->color = BLACK;

	PRINTDATA(rotate_middle);

	update_black_level(&rotate_middle->left);
	update_black_level(&rotate_middle->right);
	update_black_level(&rotate_middle);
}

void RL_rotation(redblack *rotate_middle, redblack *rotate_bro, redblack **tmp_parent)
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
		if(rotate_bro->left->left->color != RED && rotate_bro->left->right->color != RED)
			rotate_bro->left->color = RED;
			//이렇게 바꿨는데 자식이 red이면 ERROR.
	}
	rotate_middle->right = rotate_bro;
	*tmp_parent = rotate_middle;

	rotate_middle->color = RED;
	rotate_middle->left->color = BLACK;
	rotate_middle->right->color = BLACK;

	PRINTDATA(rotate_middle);

	update_black_level(&rotate_middle->left);
	update_black_level(&rotate_middle->right);
	update_black_level(&rotate_middle);
}

void RR_rotation(redblack *rotate_middle, redblack *rotate_bro, redblack **tmp_parent)
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
		if(rotate_bro->left->left->color != RED && rotate_bro->left->right->color != RED)
			rotate_bro->left->color = RED;
			//이렇게 바꿨는데 자식이 red이면 ERROR.
	}
	rotate_middle->right = rotate_bro;
	*tmp_parent = rotate_middle;

	rotate_middle->color = RED;
	rotate_middle->left->color = BLACK;
	rotate_middle->right->color = BLACK;

	PRINTDATA(rotate_middle);

	update_black_level(&rotate_middle->left);
	update_black_level(&rotate_middle->right);
	update_black_level(&rotate_middle);
}



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
    	NIL->black_level = 1;
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
	
#if 1
#if 0
	cursor = root;
	while(*cursor)
	{
		update_black_level(cursor);
		cursor = &(*cursor)->parent;
	}
#endif
#if 1
	while((*root)->parent->parent!=NIL)
	{
#endif
#if 0
		printf("Reconstruction\n");
#endif
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
		if((*root)->parent->parent->parent == NULL)
		{
			printf("why not filtering?\n");
			return;
		}
#if 1
		if((*root)->parent->parent->parent->data == -1)
		{
			tmp_grandparent = root_backup;
		}

		else if((*root)->parent->parent->parent->left == (*root)->parent->parent)
		{
			tmp_grandparent = &(*root)->parent->parent->parent->left;
		}
		else
		{
			tmp_grandparent = &(*root)->parent->parent->parent->right;
		}
#endif
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
#if 1
			printf("RED violation occur\n");
#endif
		        // compare brother of parent
		        if((*tmp_uncle)->color == BLACK)
		        {
					if(LR_flag == LL || LR_flag == RR)
					{
#if 0
						printf("case : LL or RR \n");
#endif
					    	(*tmp_parent)->color = BLACK;
					    	//after recoloring, same black level violation occurs.
					    	//rotate
					    	rotate_middle = (*tmp_parent);
					    	//rotate_mine = (*tmp_mine);
					    	rotate_bro = (*tmp_grandparent);

					    	rotate_middle->parent = rotate_bro->parent;
					    	rotate_bro->parent = rotate_middle;
					    	*tmp_parent = *tmp_bro;
					    	*tmp_bro = rotate_bro;
						*tmp_grandparent = rotate_middle;
						if((*tmp_parent)->data != -1)
							(*tmp_parent)->parent = rotate_bro; // NIL의 parent가 생기는 문제
						rotate_bro->color = RED;
						update_black_level(&rotate_bro);
						update_black_level(&rotate_middle);
					}
					else if(LR_flag == LR)
					{
#if 0
						printf("case : LR\n");
#endif
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
#if 0
						printf("rotate_bro->color : %d\n",rotate_bro->color);
#endif
						rotate_bro->color = RED;
						rotate_middle->color = BLACK;
						update_black_level(&rotate_bro);
						update_black_level(&rotate_middle);
						root = &rotate_mine;
					}
					else if(LR_flag == RL)
					{
		#if 0
						printf("case : RL\n");
		#endif
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
		#if 0
								printf("RR rotate\n");
		#endif
						rotate_middle->parent = rotate_bro->parent;
						rotate_bro->parent = rotate_middle;
						*tmp_parent = rotate_middle->left;
						rotate_middle->left = rotate_bro;
						*tmp_grandparent = rotate_middle;
						if((*tmp_parent)->data != -1)
						        (*tmp_parent)->parent = rotate_bro; // NIL의 parent가 생기는 문제.
						
						rotate_bro->color = RED;
						rotate_middle->color = BLACK;
#if 0
						printf("RR rotate\n");
#endif
						update_black_level(&rotate_bro);
						update_black_level(&rotate_middle);
						root = &rotate_mine;
#if 0
						printf("RR rotate fin\n");
#endif						
					}
					else
					{
						printf("LR flag : %d\n", LR_flag);
						assert(0 && "exception case occur LR reconstruction\n");
					}
				
		        }
		        else // UNCLE is RED
		        {
#if 0
		        	printf("recoloring uncle and parent BLACK\n");
#endif
					if((*tmp_grandparent)!=(*root_backup))
						(*tmp_grandparent)->color = RED;
					(*tmp_parent)->color = BLACK;
					(*tmp_uncle)->color = BLACK;
					update_black_level(tmp_mine);
					update_black_level(tmp_uncle);
		        }
		}
#if 0
		else // BLACK
		{
		        if((*tmp_uncle)->color == BLACK) // no violation
		        {
		        // do nothing
		        }
		        else // same black level violation
		        {
		        	//rotation
		        }
		}
#endif
#if 0
		printf("after reconstruction\n");
		printf("root data = %d\n",(*root)->data);
		printf("root=&(*root)->parent\n");
#endif
		root = &(*root)->parent;
		LR_flag = 0;

		printf("INSERT LOOP \n");
#if 0
		printf("root data = %d\n",(*root)->data);
#endif
	}
#endif	
}

void delete_redblack(redblack **root, int data)
{
	redblack** root_backup = root;
	redblack** tmp_parent = NULL;
	redblack** tmp_nephew = NULL;
	redblack** cursor = NULL;
	
	redblack** tmp_bro = NULL;
	redblack** tmp_mine = NULL;

	redblack* rotate_middle;
	redblack* rotate_bro;
	redblack* rotate_mine;
	int black_cnt = 0;
	redblack* NIL;
	
	int LR_flag = 0;
	
	redblack* replace_node;
	redblack* tmp;
	redblack** tmp_backup = root;
	
	if(!(*root))
	{
		printf("There is no node in redblack tree!\n");
		return;
	}
	else
	{
		NIL = (*root)->parent;
	}
	while(*root)
	{
		if((*root)->data > data)
		{
			tmp_backup = root;
			root = &(*root)->left;
		}
		else if((*root)->data < data)
		{
			tmp_backup = root;
			root = &(*root)->right;
		}
		else
		{
			break;
		}
	}
	
	PRINTDATA((*root));
	replace_node = *root;
	int deleted_node_color;
	if((*root)->left != NIL && (*root)->right != NIL)
	{
	printf("LEFT RIGHT root DATA : %d\n",(*root)->data);
		tmp_backup = root;
		root = &(*root)->left;
		while((*root)!=NIL)
		{
			if((*root)->right!=NIL)
			{
				tmp_backup = root;
				root = &(*root)->right;
			}
			else
			{
				printf("find seg %d\n",1);
				replace_node->data = (*root)->data;
				tmp = *root;
				deleted_node_color = (*root)->color;
				if((*root)->right==NIL)
				{
					printf("root data ;  %d\n",(*root)->data);
					*root = (*root)->left;
					printf("find seg %d\n",2);
					
					//NIL일수잇네.
					if((*root)!=NIL)
						(*root)->parent = *tmp_backup;
				}
				else if((*root)->left==NIL)
				{
					printf("find seg %d\n",3);
					*root = (*root)->right;
					if((*root)!=NIL)
						(*root)->parent = *tmp_backup;
				}
				free(tmp);
				break;
			}
		}
	}
	else
	{
		tmp = *root;
		deleted_node_color = (*root)->color;
		if((*root)->right==NIL)
		{
			*root = (*root)->left;
			if((*root)!=NIL)
				(*root)->parent = *tmp_backup;
		}
		else if((*root)->left==NIL)
		{
			*root = (*root)->right;
			if((*root)!=NIL)
				(*root)->parent = *tmp_backup;
		}	
		free(tmp);
		
	}
	PRINTDATA((*root));
	// TODO
	// 삭제 시 왼쪽 node의 MAX를 찾으므로 삭제되는 Node의 오른쪽 child는 없다.
	// 1. RED 삭제 NIL come : do nothing
	// 2. RED 삭제 REd come : impossible
	// 3. RED 삭제 BLACK come : 조카가 없으면 bro하고 parent 색 바꿈 조카 있으면 회전 ?? black level이 안 바뀌니까 아무것도 안해도 됨.(삭제되는 Node는 한쪽 child만 있다는 전제 주의)
	// 4. BLACK 삭제 RED come : RED색을 BLACK으로 바꾸기.


	// 5. BLACK 삭제 NIL come : 조카가 없으면 bro하고 parent 색 바꿈 조카 있으면 회전. RR일때 왼쪽 조카, LL일때 오른쪽 조카 RED 만들고, rotate_middle은 root일때만 B, 다른 경우는 Red
	// 조카의 자식이 있는경우 red를 만들면 red violation이 생길 수 있음. 이러면 또 회전해야함.
	// 정리하면 조카가 있는 경우 조카의 자식이 있는지 보고, 있으면 회전을 두번해야함.
	// 조카의 자식이 없어도, 형 & 부모의 색이 BLACK이면 bro하고 parent 색바꾸는걸 못하니까 조부모 가서 회전해야함.
	// parent하고 내 색이 black이면, 회전했을 때 parent위에서 바라본 black level 이 1 줄어든다.
	// 마찬가지로 parent하고 형제의 색이 Black이면 조부모가서 회전해야한다..
	
	// 6. BLACK 삭제 BLACK come : 조카가 없으면 bro하고 parent 색 바꿈 조카 있으면 회전
	// 문제는, Black come에 child가 있을 수 있음.
	// 없나? 없을 가능성이 높음. 통계적 검증 해보자.
	// 통계적으로 5번경우가 많이 나옴.

	if((*root)==(*tmp_backup))
	{
		(*root)->color = BLACK;
		(*root)->parent = NIL;
		return;// root case
		// root case를 가장 먼저 검사해야함.
	}

	if(deleted_node_color == RED && (*root)->color == BLACK) // 1), 3)
		return;
	if(deleted_node_color == BLACK && (*root)->color ==RED) // 4)
	{
		PRINTDATA(((*root)->parent));
		(*root)->color = BLACK;
		update_black_level(root);
		PRINTDATA((*root));
		return;
	}
	if(deleted_node_color == BLACK && (*tmp_backup)->color == BLACK)
	{
		// grandparent rotation
	}
	// TODO ROOT case 처리하기.
	
	// ROOT가 free가 된 상태면 어떻게 하지?
	//find nephew
	printf(" root DATA : %d\n",(*root)->data);
	printf(" tmp_Backup data : %d\n",(*tmp_backup)->data);
#if 0
	// TODO tmp_backup 이 free가 되어버리면 안됨.
	if((*root)==(*tmp_backup))
	{
		(*root)->color = BLACK;
		(*root)->parent = NIL;
		return;// root case
	}
#endif	
	tmp_mine = root;
	root = tmp_backup;

	printf(" root tmp bckup : %d\n",(*root)->data);
	if((*root)!=NIL)
	{
		printf("Reconstruction\n");
		// RECONSTRUCTION
		if((*root)->parent == NULL)
		{
			printf("I'm NIL, return\n");
		        return;
		}
		printf("(*root)->data : %d\n",(*root)->data);
		if((*root)->parent == NIL)
		{
			tmp_parent = root_backup;
		}
		else if((*root)->parent->left == (*root))
		{
		    tmp_parent = &(*root)->parent->left;
		}
		else
		{
        	tmp_parent = &(*root)->parent->right;
		}

		if((*root)->left == *tmp_mine)
		{
	        tmp_bro = &(*root)->right;
			LR_flag += 10*RIGHT; // bro의 방향
		}
		else
		{
	        tmp_bro = &(*root)->left;
			LR_flag += 10*LEFT;
		}
		
		PRINTDATA((*tmp_parent));
		PRINTDATA((*tmp_bro));
		PRINTDATA((*root)->left);
		PRINTDATA((*root)->right);
		
		printf(" root tmp bro : %d\n",(*tmp_bro)->data);

		PRINTDATA((*tmp_parent)->parent);
		// 부모가 검은색인지 중요.
		// 조카가 있으면 회전 먼저.
		// 조카가 없으면 조부모가서 회전.
		if((*tmp_parent)->color == BLACK)
		{
			//조부모 회전 먼저해도되나?
			// 되네.
			// 조부모는 RED일 확률이 높음
			// 3대가 BLACK이면 insert를 잘못 설계한거임..
		}
		if((*tmp_bro)->left == NIL && (*tmp_bro)->right == NIL) // 5) 조카 없음 // TODO BBB인 경우도 있으므로 조카 없다고 색 바꾸면 안됨.. bro & parent가 BLACK이면 grandparent로 가서 회전해야함.
		{
			PRINTDATA((*tmp_bro));
			PRINTDATA((*tmp_parent));

			assert((*tmp_bro)->color != (*tmp_parent)->color); //
			
			(*tmp_bro)->color= RED;
			(*tmp_parent)->color= BLACK;
			update_black_level(tmp_bro);
			update_black_level(tmp_parent);
			return;
		}
		else // 조카가 있으면 회전
		{
			printf("There is Nephew\n");
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
			
			if(LR_flag == LL || LR_flag == RR)
			{
#if 1
				printf("case : LL or RR \n");
#endif            	
		    	rotate_middle = (*tmp_bro);
		    	rotate_bro = (*tmp_parent);
				PRINTDATA(rotate_middle);
				PRINTDATA(rotate_bro);

		    	rotate_middle->parent = rotate_bro->parent;
		    	rotate_bro->parent = rotate_middle;
		    	
		    	if(LR_flag == LL)
		    	{
					rotate_bro->left = rotate_middle->right;
		    		//*tmp_bro = rotate_middle->right;
					if(rotate_middle->right != NIL) // NIL의 색을 RED로 바꾸는 것을 방지
					{
		    			rotate_middle->right->parent = rotate_bro;
						if(rotate_bro->left->left->color != RED && rotate_bro->left->right->color != RED)
							(*tmp_bro)->color = RED;
						//이렇게 바꿨는데 자식이 red이면 ERROR.
					}
		    		rotate_middle->right = rotate_bro;
		    	}
		    	else
		    	{
					rotate_bro->right = rotate_middle->left;
		    		//*tmp_bro = rotate_middle->left;
					if(rotate_middle->left != NIL)
					{
		    			rotate_middle->left->parent = rotate_bro;
						if(rotate_bro->right->left->color != RED && rotate_bro->right->right->color != RED)
							(*tmp_bro)->color = RED;
							//이렇게 바꿨는데 자식이 red이면 ERROR.

					}
		    		rotate_middle->left = rotate_bro;
		    	}
				
				*tmp_parent = rotate_middle;
#if 0
				if(rotate_bro->left->data != -1)
					rotate_bro->left->parent = rotate_bro; // NIL의 parent가 생기는 문제
				// 위에 통합
#endif
				// rotate_Bro : B rotate_mid : R rotate_mine(*tmp_nephew) : B로 바꾸기 전에
				// 
				(*tmp_nephew)->color = BLACK;
				rotate_bro->color = BLACK;
				if((*root_backup)->data == rotate_middle->data)
					rotate_middle->color = BLACK; // TODO root일때는 BLACK
				else
					rotate_middle->color = RED;

				PRINTDATA((*root_backup));
				PRINTDATA((rotate_middle));
				update_black_level(tmp_nephew);
				update_black_level(tmp_bro);
				update_black_level(&rotate_bro);
				update_black_level(&rotate_middle);
				printf("LL or RR root data %d\n",(*root)->data);
				
#if 0
				// 1차 회전 끝
				printf("first rotation FINISH\n");
				PRINTDATA((*tmp_bro));
				if((*tmp_bro)==NIL)
					return;
#if 1
				if(LR_flag == RR)
				{
					if((*tmp_bro)->right != NIL) // (*tmp_bro)->right == NULL이면 여기 들어가진다. 주의!
					{
						LL_rotation(*tmp_bro, rotate_bro, &rotate_bro->parent->left);
					}
					else if((*tmp_bro)->left != NIL)
					{
						LR_rotation(*tmp_bro, rotate_bro, &rotate_bro->parent->left);
					}
					else
					{
						// 자식이 없으니 위에서 바꾼대로 하면됨.
					}
				}
				else if(LR_flag == LL)
				{
					if((*tmp_bro)->left != NIL)
					{
						RR_rotation(*tmp_bro, rotate_bro, &rotate_bro->parent->right);
					}
					else if((*tmp_bro)->right!=NIL)
					{
						LR_rotation(*tmp_bro, rotate_bro, &rotate_bro->parent->right);
					}
					else
					{
						
					}
				}
				else
				{
					assert(0);
				}
#endif
#endif
#if 0
// 옮겨온놈의 오른쪽 자식이 있으면 RR case라서 LL rotation해야함.
				if((*tmp_bro)->right != NIL)
				{
					//LL rotation
					rotate_middle = (*tmp_bro);
					//rotate_bro = rotate_bro;

					rotate_middle->parent = rotate_bro->parent;
		    		rotate_bro->parent = rotate_middle;
		    	
					LR_flag = RR;
					if(LR_flag == LL)
					{
						rotate_bro->left = rotate_middle->right;
						if(rotate_middle->right != NIL) // NIL의 색을 RED로 바꾸는 것을 방지
						{
							rotate_middle->right->parent = rotate_bro;
							if(rotate_bro->left->left->color != RED && rotate_bro->left->right->color != RED)
								rotate_bro->left->color = RED;
							//이렇게 바꿨는데 자식이 red이면 ERROR.
						}
						rotate_middle->right = rotate_bro;
					}
					else
					{
						rotate_bro->right = rotate_middle->left;
						if(rotate_middle->left != NIL)
						{
							rotate_middle->left->parent = rotate_bro;
							if(rotate_bro->right->left->color != RED && rotate_bro->right->right->color != RED)
								rotate_bro->right->color = RED;
								//이렇게 바꿨는데 자식이 red이면 ERROR.
						}
						rotate_middle->left = rotate_bro;
					}
					
					//*tmp_parent = rotate_middle;
					rotate_middle->parent->left= rotate_middle;
					rotate_middle->color = RED;
					rotate_middle->left->color = BLACK;
					rotate_middle->right->color = BLACK;

					PRINTDATA(rotate_middle);

					update_black_level(&rotate_middle->left);
					update_black_level(&rotate_middle->right);
					update_black_level(&rotate_middle);

				}
				
#endif
			}
			else if(LR_flag == LR)
			{
#if 1
				printf("case : LR\n");
#endif

				rotate_middle = (*tmp_nephew);
				rotate_mine = (*tmp_bro);
				rotate_bro = (*tmp_parent);

				rotate_middle->parent = rotate_bro;
				*tmp_bro = rotate_middle;
				rotate_mine->parent = rotate_middle;
				*tmp_nephew = rotate_middle->left;
				if((*tmp_nephew)->data != -1)
					(*tmp_nephew)->parent = rotate_mine;
				rotate_middle->left = rotate_mine;

				// LL rotate
				
				rotate_middle->parent = rotate_bro->parent;
				rotate_bro->parent = rotate_middle;
				*tmp_bro = rotate_middle->right;
				rotate_middle->right = rotate_bro;
				*tmp_parent = rotate_middle;
				if((*tmp_bro)->data != -1)
				        (*tmp_bro)->parent = rotate_bro; // NIL의 parent가 생기는 문제.
				update_black_level(&rotate_bro);
#if 0
				printf("rotate_bro->color : %d\n",rotate_bro->color);
#endif
				rotate_bro->color = RED;
				rotate_middle->color = BLACK;
				rotate_mine->color = RED;
				
				update_black_level(&rotate_mine);
				update_black_level(&rotate_bro);
				update_black_level(&rotate_middle);

			}
			else if(LR_flag == RL)
			{
#if 1
     			printf("case : RL\n");
#endif

				rotate_middle = (*tmp_nephew);
				rotate_mine = (*tmp_bro);
				rotate_bro = (*tmp_parent);

				rotate_middle->parent = rotate_bro;
				*tmp_bro = rotate_middle;
				rotate_mine->parent = rotate_middle;
				*tmp_nephew = rotate_middle->right;
				if((*tmp_nephew)->data != -1)
					(*tmp_nephew)->parent = rotate_mine;
				rotate_middle->right = rotate_mine;

				// RR rotate
				
				rotate_middle->parent = rotate_bro->parent;
				rotate_bro->parent = rotate_middle;
				*tmp_bro = rotate_middle->left;
				rotate_middle->left = rotate_bro;
				*tmp_parent = rotate_middle;
				if((*tmp_bro)->data != -1)
				        (*tmp_bro)->parent = rotate_bro; // NIL의 parent가 생기는 문제.
				update_black_level(&rotate_bro);
#if 0
				printf("rotate_bro->color : %d\n",rotate_bro->color);
#endif

#if 1
//case5 backup
				rotate_bro->color = RED;
				rotate_middle->color = BLACK;
				rotate_mine->color = RED;
#endif
#if 0
// all black 은 미봉책.
				rotate_bro->color = BLACK;
				rotate_middle->color = BLACK;
				rotate_mine->color = BLACK;
#endif
				update_black_level(&rotate_bro);
				update_black_level(&rotate_mine);
				update_black_level(&rotate_middle);
			}
			else
			{
				printf("LR flag : %d\n", LR_flag);
				assert(0 && "exception case occur LR reconstruction\n");
			}
			
		}
		PRINTDATA((*tmp_parent)->parent);
#if 1
		// 1차 회전 끝
		printf("first rotation FINISH\n");
		// 조카 있으면 회전 한번 더
		PRINTDATA((*tmp_bro));
		if((*tmp_bro)==NIL)
			return;
		if(LR_flag == RR)
		{
			if((*tmp_bro)->right != NIL) // (*tmp_bro)->right == NULL이면 여기 들어가진다. 주의!
			{
				LL_rotation(*tmp_bro, rotate_bro, &rotate_bro->parent->left);
			}
			else if((*tmp_bro)->left != NIL)
			{
				LR_rotation(*tmp_bro, rotate_bro, &rotate_bro->parent->left);
			}
			else
			{
				// 자식이 없으니 위에서 바꾼대로 하면됨.
			}
		}
		else if(LR_flag == LL)
		{
			if((*tmp_bro)->left != NIL)
			{
				RR_rotation(*tmp_bro, rotate_bro, &rotate_bro->parent->right);
			}
			else if((*tmp_bro)->right!=NIL)
			{
				LR_rotation(*tmp_bro, rotate_bro, &rotate_bro->parent->right);
			}
			else
			{
				
			}
		}
		else
		{
			assert(0);
		}
#endif

		PRINTDATA((*tmp_parent));
#if 0
		// 조카 있으면 회전 한번 더!!
		// LR로 구분하는게 싫으면 *tmp_bro도 가능.
		if(LR_flag % 10 == LEFT) // bro가 RIGHT, 그럼 bro의 left의 자식 조사
		{
			if(rotate_bro->left->left!=NIL && rotate_bro->left->right!=NIL)
			{
				rotate_middle = rotate_bro->left;
				// 오른쪽이 있으면 하나만 rotate.
				// 왼쪽'만' 있으면 double rotate.
			}
		}
		else
		{
			if(rotate_bro->right->left!=NIL && rotate_bro->right->right!=NIL)
			{
				rotate_middle = rotate_bro->right;
			}
		}
		
#endif	
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
		print_redblack(tmp->right);
		redblack_property_test(&tmp);
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
			PRINTDATA((*root)->parent);
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

int main()
{
    redblack *root = NULL;

    int i;

#if 0
    int data[5] = { 0 };
#endif

#if 1
    int data[20] = { 0 };
#endif
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);
#if 0
//case1
// CLEAR
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
#endif
#if 0
//case2
// CLEAR
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
#endif
#if 0
//case3
//부모 BLACK, 형제 RED, 삼촌 X, 조카의 자식이 2명
// 2 삭제에서 회전 2번해야 하는 케이스.
// 형제가 RED면 자식이 반드시 BLACK이어야하므로 회전시 붙는 애는 BLACK이다.
// CLEAR
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
#endif
#if 0
//case4
//부모 BLACK, 형제 RED, 삼촌 X
// 1 삭제에서 회전 2번해야 하는 케이스.
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
#endif
#if 0
//case5
//조카 있고, 부모 BLACK, 삼촌 BLACK, 형제 BLACK
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
//삼촌도 봐야하는 케이스?
#endif
#if 0
//case6
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
#endif
    for (i = 0; i < len; i++)
    {

		printf("insert data : %d\n", data[i]);
		insert_redblack(&root, data[i]);
		printf("Now root is %d\n",root->data);
		// property 1 : The color of root node is BLACK
    	assert((root ? root->color : RED) == BLACK);
		print_redblack(root);
    }
    // property 1 : The color of root node is BLACK
    assert((root ? root->color : RED) == BLACK);
    printf("The color of root node is BLACK\n");
    print_redblack(root);
#if 1
    for(i = 0; i < len; i++)
    {
    	printf("delete data : %d\n", data[i]);
    	delete_redblack(&root, data[i]);
    	printf("Now root is %d\n",root->data);
    	print_redblack(root);
		// property 1 : The color of root node is BLACK
		PRINTDATA(root);
    	assert((root ? root->color : RED) == BLACK);
    }
    assert((root ? root->color : RED) == BLACK);
    printf("The color of root node is BLACK\n");
    print_redblack(root);
	PRINTDATA(root);
#endif

}

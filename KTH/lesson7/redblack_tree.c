#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define LEVEL(X) ((X) ? X->black_level : 0)
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
#if 0
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
	redblack* tmp_backup;
	
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
			tmp_backup = *root;
			root = &(*root)->left;
		}
		else if((*root)->data < data)
		{
			tmp_backup = *root;
			root = &(*root)->right;
		}
		else
		{
			break;
		}
	}
	
	replace_node = *root;
	int deleted_node_color;
	if((*root)->left != NIL && (*root)->right != NIL)
	{
		tmp_backup = *root;
		root = &(*root)->left;
		while((*root)!=NIL)
		{
			if((*root)->right!=NIL)
			{
				tmp_backup = *root;
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
						(*root)->parent = tmp_backup;
				}
				else if((*root)->left==NIL)
				{
					printf("find seg %d\n",3);
					*root = (*root)->right;
					if((*root)!=NIL)
						(*root)->parent = tmp_backup;
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
				(*root)->parent = tmp_backup;
		}
		else if((*root)->left==NIL)
		{
			*root = (*root)->right;
			if((*root)!=NIL)
				(*root)->parent = tmp_backup;
		}	
		free(tmp);
		
	}
	// TODO
	// 1. RED 삭제 NIL come : do nothing
	// 2. RED 삭제 REd come : impossible
	// 3. RED 삭제 BLACK come : 조카가 없으면 bro하고 parent 색 바꿈 조카 있으면 회전
	// 4. BLACK 삭제 RED come : 조카가 없으면 bro하고 parent 색 바꿈 조카 있으면 회전
	// 5. BLACK 삭제 NIL come : 조카가 없으면 bro하고 parent 색 바꿈 조카 있으면 회전
	// 6. BLACK 삭제 BLACK come : 조카가 없으면 bro하고 parent 색 바꿈 조카 있으면 회전
	if(deleted_node_color == RED && (*root) == NIL)
		return;
	
	// TODO ROOT case 처리하기.
	//find nephew
	tmp_mine = root;
	root = &tmp_backup; //
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
		printf(" root tmp bro : %d\n",(*tmp_bro)->data);
		// 조카가 없으면
		if((*tmp_bro)->left == NIL && (*tmp_bro)->right == NIL)
		{
			(*tmp_bro)->color= RED;
			(*tmp_parent)->color= BLACK;
			update_black_level(tmp_bro);
			update_black_level(tmp_parent);
		}
		else // 조카가 있으면 회전
		{
			if((*tmp_bro)->right != NIL)
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
#if 0
				printf("case : LL or RR \n");
#endif            	
            	rotate_middle = (*tmp_bro);
            	rotate_bro = (*tmp_parent);

            	rotate_middle->parent = rotate_bro->parent;
            	rotate_bro->parent = rotate_middle;
            	*tmp_bro = rotate_middle->right;
            	rotate_middle->right = rotate_bro;
				*tmp_parent = rotate_middle;
				if(rotate_bro->left->data != -1)
					rotate_bro->left->parent = rotate_bro; // NIL의 parent가 생기는 문제
				(*tmp_nephew)->color = RED;
				update_black_level(&rotate_bro);
				update_black_level(&rotate_middle);
			}
			else if(LR_flag == LR)
			{
#if 0
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
				update_black_level(&rotate_bro);
				update_black_level(&rotate_middle);

			}
			else if(LR_flag == RL)
			{
#if 0
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
				rotate_bro->color = RED;
				rotate_middle->color = BLACK;
				rotate_mine->color = RED;
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
				printf("parent = NIL\t");
			else
				printf("parent = %4d\t", tmp->parent->data);
		}
		else
		{
			printf("parent = NULL\t");
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
			assert((*root)->parent->color == BLACK);
		if((*root)->left)
			assert((*root)->left->color == BLACK);
		if((*root)->right)
			assert((*root)->right->color == BLACK);
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
    int data[10000] = { 0 };
#endif
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);
#if 0
	data[0] = 3;
	data[1] = 1;
	data[2] = 8;
	data[3] = 9;
	data[4] = 10;
	data[5] = 5;
	data[6] = 4;
	data[7] = 6;
	data[8] = 2;
	data[9] = 7;
	len = 10;
	print_arr(data, len);
	// LR case is needed to be updated.
#endif
    for (i = 0; i < len; i++)
    {

		printf("insert data : %d\n", data[i]);
		insert_redblack(&root, data[i]);
		printf("Now root is %d\n",root->data);
//		print_redblack(root);
    }
    // property 1 : The color of root node is BLACK
    assert((root ? root->color : RED) == BLACK);
    printf("The color of root node is BLACK\n");
    print_redblack(root);
#if 0
    for(i = 0; i < len; i++)
    {
    	printf("delete data : %d\n", data[i]);
    	delete_redblack(&root, data[i]);
    	printf("Now root is %d\n",root->data);
    	print_redblack(root);
    }
    assert((root ? root->color : RED) == BLACK);
    printf("The color of root node is BLACK\n");
    print_redblack(root);
#endif
#if 0
    printf("delete redblack tree\n");
    for (i = 0; i < len; i++)
    {
	    printf("delete redblack tree data : %4d\n", data[i]);
	    delete_redblack(&root, data[i]);
	    print_redblack(root);
    }
    print_redblack(root);
#endif
}

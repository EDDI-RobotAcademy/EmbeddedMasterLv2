#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RED	1
#define BLACK	2
#define NONE	3

typedef struct _rbt rbt;
struct _rbt
{
	int data;
	struct rbt *left;
	struct rbt *right;
	struct rbt *parent;
	int color;
} rbt;

rbt* create_rb_tree_node(int data)
{
	rbt* node = (rbt*)malloc(sizof(rbt));
	node->left = NULL;
	node->right = NULL;
	node->color = NONE;
       	node->data = data;

	return node;	
}

void insert_rb_tree_node(rbt **root)
{
	if (*root == NULL)
	{
		
	}


}

int main(void)
{

	return 1;

}

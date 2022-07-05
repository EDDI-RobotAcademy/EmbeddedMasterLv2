#ifndef __GLOBAL_VAR_H__
#define __GLOBAL_VAR_H__

#include "./global_var.h"

typedef struct _vertex2D vertex2D;
struct _vertex2D
{
	double x;
	double y;
};
extern vertex2D basis;
extern int dx;
extern int dy;

#endif

#include "../include/symbol/symbol_library.h"
#include "../include/global_var.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>

#define SLICE	360

symbol_queue *R = NULL;
vertex2D basis;
int dx;
int dy;

#if 0
	이와 같은 vertex 배열의 개수
	x, y, z를 표현할 수 있는 vertex 포인터(갯수가 가변이니까)
	어디랑 어디가 연결되는지에 대한 정보가 필요함(인덱스)
	- closed 되는 구성의 성분들끼리 모아놓고
	  opened 되는 구성의 성분들은 별도로 관리함

	- (시작점, 끝점) 방식의 관리
	  for루프 돌릴때 특별히 생각할 사항이 없어진다는 이점이 있음
#endif

void draw_2D_grid(void)
{
	double i = 1;
	glColor3f(1, 1, 1);
	for(i = -10; i <= 10; i++)
	{
		glBegin(GL_LINE_LOOP);
		if(i == 0)
			continue;
		glVertex3f(10.0*(double)i, 100.0, 0);
		glVertex3f(10.0*(double)i, -100.0, 0);
		glEnd();

		glBegin(GL_LINE_LOOP);
		if(i == 0)
			continue;
		glVertex3f(100.0, 10.0*(double)i, 0);
		glVertex3f(-100.0, 10.0*(double)i, 0);
		glEnd();
	}
}

/*
   1. 다음과 같은 모양의 저항 심볼을 만든다
   -/\/\/\-
   2. 입력 받아야하는 변수는 시작점과 끝점, x,y축의 변화량(시작점과 끝점은 리드선을 기준으로 그린다)
   3. 첫 번째 대각선, 마지막 대각선의 y축 시작점은 y축 변화량의 절반지점
   4. 홀수번째 대각선의 끝점은 x축의 변화량의 절반 지점
   5. 짝수번째 대각선의 끝점은 x축, y축의 변화량 만큼
   6. 저항 심볼의 대각선 갯수는 6개 정점의 갯수는 7개가 된다
   7. 저항의 리드는 시작점과 끝점에 연결되며, x축 변화량과 절반의 y축 변화량을 가진다

   -> 테스트용으로 10,10지점을 기준으로 그린다
*/

void draw_sym_R()
{
	int i;

	glColor3f(0, 0, 1);
	//저항의 시작점 리드선을 그린다
	glBegin(GL_LINE_LOOP);
	glVertex3f(R->symbol.pVtx[0].x, R->symbol.pVtx[0].y, 0);
	glVertex3f(R->symbol.lead[0].vertex.x, R->symbol.lead[0].vertex.y, 0);
	glEnd();

	glColor3f(0, 0, 1);
	//저항의 끝점 리드선을 그린다
	glBegin(GL_LINE_LOOP);
	glVertex3f(R->symbol.lead[1].vertex.x, R->symbol.lead[1].vertex.y, 0);
	glVertex3f(R->symbol.pVtx[6].x, R->symbol.pVtx[6].y, 0);
	glEnd();

	for(i = 0; i < 6; i++)
	{
		glColor3f(1, 1, 0);
		glBegin(GL_LINE_LOOP);
		glVertex3f(R->symbol.pVtx[i+1].x, R->symbol.pVtx[i+1].y, 0); 
		glVertex3f(R->symbol.pVtx[i].x, R->symbol.pVtx[i].y, 0); 
		glEnd();
	}
}

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glColor3f(1, 0, 0);

	glBegin(GL_LINE_LOOP);
	glVertex3f(100.0, 0.0, 0.0);
    glVertex3f(-100.0, 0.0, 0.0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0, 100.0, 0.0);
    glVertex3f(0.0, -100.0, 0.0);
    glEnd();

	draw_2D_grid();
	draw_sym_R();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
    GLfloat n_range = 100.0f;

    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(w <= h)
        glOrtho(-n_range, n_range, -n_range * h / w, n_range * h / w, -n_range, n_range);
    else
        glOrtho(-n_range * w / h, n_range * w / h, -n_range, n_range, -n_range, n_range);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
	//심볼의 기저 좌표
	vertex2D basis = {10, 10};
	dx = 10, dy = 10;
	//저항 심볼의 좌표를 초기화 한다
	enqueue_symbol('R', &R, 1);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Digital Signal Processing");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}

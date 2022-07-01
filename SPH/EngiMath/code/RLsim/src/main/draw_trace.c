#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SLICE	360
#define Window_w 800
#define Window_h 800
#define World_w 200
#define World_h 200

bool clicked = false;
bool trace_mode = false;
int special_key;
//Window coordinate
int x, y;
//World coordinate
double click_x, click_y;
double mov_x, mov_y;

#if 0
	이와 같은 vertex 배열의 개수
	x, y, z를 표현할 수 있는 vertex 포인터(갯수가 가변이니까)
	어디랑 어디가 연결되는지에 대한 정보가 필요함(인덱스)
	- closed 되는 구성의 성분들끼리 모아놓고
	  opened 되는 구성의 성분들은 별도로 관리함

	- (시작점, 끝점) 방식의 관리
	  for루프 돌릴때 특별히 생각할 사항이 없어진다는 이점이 있음
	- glutPostRedisplay() 함수는 변경된 사항을 윈도우에 적용한다
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

void trace_draw_2D_line(void)
{
	printf("Trace Draw start!\n");
	glColor3f(1, 1, 0);

	glBegin(GL_LINE_LOOP);
	glVertex3f((double)mov_x, (double)mov_y, 0);
	glVertex3f((double)click_x, (double)click_y, 0);
	glEnd();
}

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glColor3f(1, 0, 0);
	glBegin(GL_LINE_LOOP); 
	glVertex3f((double)World_w/2.0, 0.0, 0.0); 
	glVertex3f(-(double)World_w/2.0, 0.0, 0.0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0, (double)World_h/2.0, 0.0);
    glVertex3f(0.0, -(double)World_h/2.0, 0.0);
    glEnd();

	draw_2D_grid();
	trace_draw_2D_line();
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

/*
 1. glutGet
 (1) 초당 그려지는 장면의 수 FPS를 구하는 함수
 (2) 함수원형 : glutGet(GLenum state)
 (3) 파라메터 : glutGet을 통해 알고싶은 값의 이름

 2. glReadPixel
 (1) 프레임 버퍼로 부터 픽셀 값을 읽는다
 (2) 함수원형 : glReadPixel(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *data)
 (3) 파라메터 : x, y -> x, y 첫번째 픽셀의 좌표이고,
                첫 번째 픽셀은 화면의 왼쪽 아래 코너
				width, height -> 픽셀의 높이, 넓이
				format -> 픽셀데이터의 포맷
				size -> buffer사이즈
				data -> 리턴되는 pixel데이터
*/
void on_mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON & state == GLUT_DOWN)
	{
		printf("Click - Left Down\n");
		//window 좌표계 -> 월드 좌표계로 변환
		click_x = ((double)x-(double)Window_w/2.0)*((double)World_w/(double)Window_w);
		click_y = ((double)y-(double)Window_h/2.0)*(-(double)World_h/(double)Window_h);

		//마우스를 눌렀다 뗐을 때
		if(clicked == true)
		{
			printf("clicked true\n");
			clicked = false;
			trace_mode = true;
		}
		//마우스를 눌렀을 때
		else
		{
			printf("clicked false\n"); 
			clicked = true;
			trace_mode = false;
		}
		printf("World Coordinate click_x = %lf, click_y = %lf\n", click_x, click_y);
		printf("Window Coordinate x = %d, y = %d\n", x, y);
	}

	if(button == GLUT_RIGHT_BUTTON & state == GLUT_DOWN)
	{
		printf("Click - Right Down\n");
		clicked = false;
		return;
	}
} 

void on_mouse_pointer(int x, int y) 
{
	if(trace_mode == true)
	{
		mov_x = ((double)x - (double)Window_w/2.0)*((double)World_w/(double)Window_w);
		mov_y = ((double)y - (double)Window_h/2.0)*(-(double)World_h/(double)Window_h);

		printf("Mouse Pointer x = %lf, y = %lf\n", mov_x, mov_y);
		glutPostRedisplay();
	}
}

/*
 1. glutMouseFunc
 (1) 마우스 클릭을 처리하기 위한 콜백함수
 (2) 함수 원형 : void glutMouseFunc(void (*func)(int button, int state, int x, int y))
 (3) 파라메터 : button -> 마우스의 우클릭,좌클릭,휠클릭으로 각각 GLUT_RIGHT_BUTTON, GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON
                state -> 마우스의 상태 GLUT_DOWN = ON, GLUT_UP = OFF
				x, y -> 마우스의 좌표값
 2. glutMotionFunc 
 (1) 마우스의 능동 모션처리를 하는 콜백함수 <-> 수동모션처리 glutPassiveMotionFunc
 (2) 함수 원형 : void glutMotionFunc(void (*func)(int x, int y))
 (3) 파라메터 : x, y -> 마우스의 좌표 값
 (4) 능동모션은 마우스 버튼을 누른채 마우스를 움직이는 것이고 수동모션은 마우스 버튼을 누르지 않은채 마우스를 움직이는 것
*/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(Window_w, Window_h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Draw Trace");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(on_mouse);
	glutPassiveMotionFunc(on_mouse_pointer);
	glutMainLoop();

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>
#include "../include/vector_3d.h"

#define SLICE	360

struct _Lcolor
{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
};

typedef struct _Lcolor Lcolor;

void draw_3d_vector(vec3 *arr, vec3 offset)
{
	//vector 1
#if 1
	glColor3f(1, 0, 0);
#endif
#if 0
	glColor3f(vcolor[0].red, vcolor[0].green, vcolor[0].blue);
#endif
	glBegin(GL_LINE_LOOP);
		glVertex3f(arr[0].x, arr[0].y, arr[0].z);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	//vector 2
#if 1
	glColor3f(0, 1, 0);
#endif
#if 0
	glColor3f(v2.red, v2.green, v2.blue);
#endif
	glBegin(GL_LINE_LOOP);
		glVertex3f(arr[1].x, arr[1].y, arr[1].z);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	//vector 3
#if 1
	glColor3f(0, 0, 1);
#endif
#if 0
	glColor3f(v3.red, v3.green, v3.blue);
#endif
	glBegin(GL_LINE_LOOP);
		glVertex3f(arr[2].x, arr[2].y, arr[2].z);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}

void draw_gramschmidt(vec3 *arr, vec3 offset)
{
	//Lcolor[3] = {};
	vec3 res[3]={};
	vec3 r = {0, 0, 0,
		      vec3_add, vec3_sub,
			  vec3_scale, vec3_dot, vec3_cross,
			  print_vec3, gramschmidt_normalization};

	glTranslatef(offset.x, offset.y, offset.z);
	draw_3d_vector(arr, offset);

	r.gramschmidt(arr, res, r);

	for(int i = 0; i < 3; i++)
		r.print(res[i]);

	draw_3d_vector(res, offset);
}

void display(void)
{
	//define vector set
	vec3 vec_set[3] = {{0.0, 40.0, 0.0},{20.0, 20.0, 10.0}, {10.0, 10.0, 10.0}}; 
	//define line color set
	//Lcolor color[3]={};
	vec3 offset = {3.0, 3.0, 3.0};

	glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	// 사용자가 바라보는 시각이 어디가 될지를 지정함
	// 요 부분은 추후 자료에 추가 및 설명을 보강할 예정
	// 카메라가 바라보는 관점? 마우스를 가지고 오브젝트의 관점을 이동할 때
#if 0
    gluLookAt(3.0, 3.0, 3.0-4.5, 0.0, 0.0,-4.5,0,1,0);
#endif
	//gluLookAt(eyeX, eyeY, eyeZ, AtX, AtY, AtZ, UpX, UpY, UpZ)
	//eye는 카메라의 위치, At은 카메라의 초점, Up은 카메라에 수직되는 벡터
#if 0
	gluLookAt(3.0, 3.0, 3.0, 0, 0, 0, 0, 0, 1);
#endif
#if 1
	gluLookAt(-1.098, 4.098, 3, 0, 0, 0, 0, 0, 1);
#endif
#if 0
	//z-axis 60deg -> x-axis 60deg
	gluLookAt(-1.098, 4.647, -2.048, 0, 0, 0, 0, 0, 1);
#endif

	//begin, end 작업시 
	//그래픽의 최소단위는 삼각형(프렉탈)제어시 꼬이는 상황을 방지하기 위해 사용한다
    glPushMatrix();

#if 0
	// 평행 이동 z축으로 -4.5만큼
    glTranslatef(0.0, 0.0, -4.5);
#endif
	glTranslatef(0.0, 0.0, 0.0);

	/*draw base coordinate*/
    glBegin(GL_LINES);
	//x-axis
    glColor3f (1.0, 0.0, 0.0);
    glVertex3f(100.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);

	//y-axis
    glColor3f (0.0, 1.0, 0.0);
    glVertex3f(0.0, 100.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);

	//z-axis
    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 100.0);
    glVertex3f(0.0, 0.0, 0.0);
    glEnd();

	//draw GramSchmidt result
	draw_gramschmidt(vec_set, offset);

    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    GLfloat n_range = 10.0f;

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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Digital Signal Processing");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}

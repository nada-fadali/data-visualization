#include <GL/glut.h>

using namespace std;


void init(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
}

void handleResize(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w/(double)h, 1.0, 1000.0);

}


void drawAxis()
{
	// x-axis
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(1.0,0.0,0.0);
		glVertex3f(-1.0,0.0,0.0);
	glEnd();
			// y-axis
	glBegin(GL_LINES);
		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,1.0,0.0);
		glVertex3f(0.0,-1.0,0.0);
	glEnd();
		// z-axis
	glBegin(GL_LINES);
		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,-1.0);
		glVertex3f(0.0,0.0,1.0);
	glEnd();

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	gluLookAt(1.0, 1.0, 1.0,
				0.0, 0.0, 0.0,
				0.0, 1.0, 0.0	);


	// draw
	drawAxis();

	glutSwapBuffers();
}

int main(int argc, char **argv)
{  

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(50,50);
	glutCreateWindow("Project");
	init();
	glutDisplayFunc(display);	
	glutReshapeFunc(handleResize);
	// glutSpecialFunc(specKeyFun);
	glutMainLoop();

	return 0;            
}

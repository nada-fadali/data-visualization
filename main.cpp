#include <GL/glut.h>	// opengl
#include <fstream>		// read txt file
#include <string>		// string
#include <iostream>		// cout
#include <sstream>		// istringstream
#include <vector>		// vector

using namespace std;


///////////////////////////////
//	CLASSES
///////////////////////////////

//=========> Node
class Node {
public:
    string name;
    int value;
    Node* parent;
    vector<Node*> children;
    vector<Node*> link;
    Node();
    Node(string, int);
    Node(string,int, Node*);
};

Node::Node(){ 
}

Node::Node(string n , int v){
	name = n;
	value = v;
}

Node::Node(string n, int v, Node* p){
	Node(n, v);
	parent = p;
}


//=========> HashNode



//////////////////////////////
//	OPENGL
/////////////////////////////
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


//////////////////////////////
//	MAIN
//////////////////////////////
int main(int argc, char **argv)
{
	ifstream myfile ("inputs.txt");
	if (myfile.is_open()){
		string line;
		bool firstLine = true;
		Node root = Node();
		while ( getline (myfile,line) ){
			istringstream ss(line);
			string sub;

			vector<string> subStrings;
			while(getline(ss, sub, ':')){
				subStrings.push_back(sub);
			}

			// get the root node
			if(firstLine){
				if (subStrings.size() > 3) {
					cout << "Error in input data at line 0" << endl;
					exit(0);
				}
				else {
					root.name = subStrings[0];
					root.value = atoi(subStrings[1].c_str());
					root.parent = NULL;

					firstLine = false;
				}
			}
			// get other node that is not the root 
			else {
				
			}
		}
		myfile.close();
    }
    else cout << "Unable to open file";


    // start opengl
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(50,50);
	//glutCreateWindow("Project");
	init();
	glutDisplayFunc(display);	
	glutReshapeFunc(handleResize);
	// glutSpecialFunc(specKeyFun);
	glutMainLoop();

	return 0;            
}

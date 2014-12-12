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

//=========> Point
class Point
{
public:
	float x, y, z;
	Point();
	Point(float, float, float);
};

Point::Point(){}

Point::Point(float xin, float yin, float zin)
{
	x = xin; y = yin; z = zin;
}

//=========> Node
class Node {
public:
    string name;
    int value;
    string parent;	// parent name
    vector<string> relation;
    Point position;
    int level;
    Node();
    Node(string, int);
};

Node::Node(){ 
}

Node::Node(string n , int v){
	name = n;
	value = v;
}


//==========> Graph
class Graph
{
public:
	Node root;
	vector<Node> nodeList;
	Graph();
};

Graph::Graph(){
}


//////////////////////////////
//	Globals
//////////////////////////////
Graph graph;
Point cam;


//////////////////////////////
//	Logic
/////////////////////////////


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

void specKeyFun(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_RIGHT:
			cam.x += 0.1;
			//cam.z += 0.1;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			cam.x -= 0.1;
			//cam.z += 0.1;
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:
			cam.y += 0.1;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			cam.y -= 0.1;
			glutPostRedisplay();
			break;		
		case GLUT_KEY_END:
			exit(0);
			break;
		default: break;
	}
}

void renderStrokeFontString(float x, float y, float z, float sx, float sy, float sz, char *string) {
	char *c;
	glPushMatrix();
		glScalef(sx, sy, sz);
		glRotatef(45, 0.0, 1.0, 0.0);
		glTranslatef(x, y, z);
		//glRotatef(-30, 1.0, 0.0, 0.0);
		for (c = string; *c != '\0'; c++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
		}
	glPopMatrix();
}

void drawLevelPlane()
{
	glBegin(GL_LINE_STRIP);
	glVertex3f(-0.5,0.5,-0.5);
	glVertex3f(0.5,0.5,-0.5);
	glVertex3f(0.5,0.5,0.5);
	glVertex3f(-0.5,0.5,0.5);
	glVertex3f(-0.5,0.5,-0.5);
	glEnd();
}

void drawAxis()
{
	// x-axis
	glBegin(GL_LINES);
		glColor3f(0.2,0.0,0.0);
		glVertex3f(1.0,0.0,0.0);
		glVertex3f(-1.0,0.0,0.0);
	glEnd();
	// y-axis
	glBegin(GL_LINES);
		glColor3f(0.0,0.2,0.0);
		glVertex3f(0.0,1.0,0.0);
		glVertex3f(0.0,-1.0,0.0);
	glEnd();
	// z-axis
	glBegin(GL_LINES);
		glColor3f(0.0,0.0,0.2);
		glVertex3f(0.0,0.0,-1.0);
		glVertex3f(0.0,0.0,1.0);
	glEnd();

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	gluLookAt(cam.x, cam.y, cam.z,
				0.0, 0.0, 0.0,
				0.0, 1.0, 0.0	);


	// draw axis
	drawAxis();

	// draw level
	// glPushMatrix();
	// glScalef(0.5,0.5,0.5);
	// glColor3f(0.5,0.5,0.0);
	// drawLevelPlane();
	// glPopMatrix();

	// for example
	// glColor3f(1.0,0.0,1.0);
	// glLineWidth(5.0);
	// renderStrokeFontString(0.0,0.0,-1.0, 0.0005,0.0005,0.0005, "N");

	glutSwapBuffers();
}

//////////////////////////////
//	MAIN
//////////////////////////////
int main(int argc, char **argv)
{
	ifstream myfile ("inputs.txt");
	if (myfile.is_open()){
		bool firstLine = true;
		vector<string> tobeParent;
		graph = Graph();

		string line;
		int level=0;
		while ( getline (myfile,line) ){
			istringstream ss(line);
			string sub;

			// split at ":"
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
					graph.root = Node(subStrings[0], atoi(subStrings[1].c_str()));

					graph.root.level = level++;
					tobeParent.push_back(graph.root.name);

					firstLine = false;
				}
			}
			// get other node that is not the root 
			else {
				Node n = Node(subStrings[0], atoi(subStrings[1].c_str()));
				if(subStrings.size() == 4){
					n.relation.push_back(subStrings[2]);
				}
				if(subStrings.back().compare(".")==0 || subStrings.back().compare("{")==0){
					n.parent = tobeParent.back();
					n.level = level;
					graph.nodeList.push_back(n);
				}

				if(subStrings.back().compare("{")==0){
					level++;
					tobeParent.push_back(n.name);
				} 
				else if(subStrings.back().compare("}")==0){
					level--;
					if(tobeParent.empty()){
						tobeParent.push_back(graph.root.name);
					} 
					else{
						tobeParent.pop_back();	
					}
					
				}
			}
		}
		myfile.close();
    }
    else cout << "Unable to open file";

    // DEBUGGING
    //<---->
    cout << "name value parent level relation"<<endl;
    cout << graph.root.name << " " << graph.root.value  << " " << graph.root.level << endl;
    for (unsigned i = 0; i < graph.nodeList.size(); i++)
    {
    	cout << graph.nodeList[i].name << " " << graph.nodeList[i].value << " " << graph.nodeList[i].parent << " " << graph.nodeList[i].level << " " ;
    	if(!graph.nodeList[i].relation.empty())
    		cout << graph.nodeList[i].relation[0];
    	cout << endl;
    }
    //<---->

    cam.x = 1.0;
	cam.y = 0.0;
	cam.z = 1.0;

    // start opengl
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(50,50);
	//glutCreateWindow("Project");
	init();
	glutDisplayFunc(display);	
	glutReshapeFunc(handleResize);
	glutSpecialFunc(specKeyFun);
	glutMainLoop();

	return 0;            
}

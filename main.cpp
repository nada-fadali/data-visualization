#include <GL/glut.h>	// opengl
#include <fstream>		// read txt file
#include <string>		// string
#include <iostream>		// cout
#include <sstream>		// istringstream
#include <vector>		// vector
#include <stdio.h>		// printf

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
	int getNumberOfLevels();
	vector<string> getNodesInLevel(int);
	void setNodeLocation(string, float, float, float);
	Point getNodePosition(string);
	string getParent(string);
};

Graph::Graph(){
}

int Graph::getNumberOfLevels(){
	int max = 0;
	for (unsigned i = 0; i < nodeList.size(); i++){
		if(nodeList[i].level > max)
			max = nodeList[i].level;
	}
	return max;
}

vector<string> Graph::getNodesInLevel(int level){
	vector<string> result;
	if(level == 0){
		result.push_back(root.name);
	}
	else {
		for (unsigned i = 0; i < nodeList.size(); i++){
		if(nodeList[i].level == level)
			result.push_back(nodeList[i].name);
		}
	}
	return result;
}

void Graph::setNodeLocation(string name, float x, float y, float z){
	if(name.compare(root.name) == 0){
		root.position = Point(x, y, z);
	}
	else{
		for (unsigned i = 0; i < nodeList.size(); i++){
			if(nodeList[i].name.compare(name) == 0){
				nodeList[i].position = Point(x, y, z);		
			}
		}
	}
}

Point Graph::getNodePosition(string nodeName){
	Point p;
	for(unsigned i = 0; i < nodeList.size(); i++) {
		if(nodeList[i].name.compare(nodeName) == 0){
			p = nodeList[i].position;
		}
	}
	if(root.name.compare(nodeName) == 0){
		p = root.position;
	}
	return p;
}

string Graph::getParent(string node){
	if(root.name.compare(node)==0)
		return "root";
	else{
		for(unsigned i = 0; i < nodeList.size(); i++){
			if(nodeList[i].name.compare(node)==0)
				return nodeList[i].parent;
		}
	}
	return "error";
}

//////////////////////////////
//	Globals
//////////////////////////////
Graph graph;
Point cam;


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
	glLineWidth(3.0);
	glPushMatrix();
		glRotatef(45, 0.0, 1.0, 0.0);
		glTranslatef(x, y, z);
		glScalef(sx, sy, sz);
		glRotatef(90, 0.0, 1.0, 0.0);
		for (c = string; *c != '\0'; c++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
		}
	glPopMatrix();
	glLineWidth(1.0);
}

void drawLevelPlane()
{
	glBegin(GL_LINE_STRIP);
	//glBegin(GL_QUADS);
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


	glLineWidth(1.0);
	// draw axis
	drawAxis();

	// draw each level
	int levels = graph.getNumberOfLevels();
	float factor;
	for (int i = 0; i < levels+1; i++){
		factor = (i*1.0 / levels*1.0);

		// draw base plane
		glPushMatrix();
			glColor3f(0.1,0.0,0.3);
			glTranslatef(0, -factor, 0);
			glScalef(factor+0.5, factor+0.5, factor+0.5);
			drawLevelPlane();
		glPopMatrix();

		// get nodes in this level
		vector<string> levelNodes = graph.getNodesInLevel(i);
		// draw nodes
		float sign = 0.1;
		for (unsigned j = 0; j < levelNodes.size(); j++){
			glColor3f(1.0,0.0,1.0);
			graph.setNodeLocation(levelNodes[j], j*0.1, -factor+0.5, j*sign);
			renderStrokeFontString(
				j*0.1,
			    -factor+0.5,
				j*sign,
				0.0008,0.0008,0.0008, 
				&levelNodes[j][0]
			);
			sign *= -1;
		}
	}

	// draw edges between nodes
	glLineWidth(2.0);
	glPushMatrix();
	glRotatef(45.0,0.0,1.0,0.0);
	glTranslatef(0.0, 0.1, 0.0);
	glColor3f(0.5, 0.5, 0.0);
	for (unsigned w = 0; w < graph.nodeList.size(); w++){
		Point p = graph.getNodePosition(graph.nodeList[w].parent);
		glBegin(GL_LINES);
			glVertex3f(p.x, p.y-0.1, p.z);
			glVertex3f(graph.nodeList[w].position.x, graph.nodeList[w].position.y, graph.nodeList[w].position.z);
		glEnd();
	}
	glPopMatrix();

	// draw relations between nodes and their parents
	glColor3f(0.8, 0.8, 0.8);
	for (unsigned z = 0; z < graph.nodeList.size(); z++){
		if(!graph.nodeList[z].relation.empty()){
			Point p1 = graph.nodeList[z].position;
			for(unsigned q = 0; q < graph.nodeList[z].relation.size(); q++){
				Point p2 = graph.getNodePosition(graph.nodeList[z].relation[q]);

				glPushMatrix();
				glRotatef(45.0,0.0,1.0,0.0);
				glTranslatef(0.0, 0.0, 0.0);
				glBegin(GL_LINES);
					glVertex3f(p1.x, p1.y, p1.z);
					glVertex3f(p2.x, p2.y, p2.z);
				glEnd();
				glPopMatrix();

				// draw relation between parents
				string parent1 = graph.nodeList[z].parent;
				string parent2 = graph.getParent(graph.nodeList[z].relation[q]);
				while(parent1.compare(parent2) != 0){

					Point pp1 = graph.getNodePosition(parent1);
					Point pp2 = graph.getNodePosition(parent2);

					glPushMatrix();
					glRotatef(45.0,0.0,1.0,0.0);
					glTranslatef(0.0, 0.0, 0.0);
					glBegin(GL_LINES);
						glVertex3f(pp1.x, pp1.y, pp1.z);
						glVertex3f(pp2.x, pp2.y, pp2.z);
					glEnd();
					glPopMatrix();


					parent1 = graph.getParent(parent1);
					parent2 = graph.getParent(parent2);
				}
			}
		}
	}

	glutSwapBuffers();
}

//////////////////////////////
//	MAIN
//////////////////////////////
int main(int argc, char **argv)
{	
	// process input
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

    int levels = graph.getNumberOfLevels();
    cam.x = 1.0;
	cam.y = 1.0;
	cam.z = -1*(levels);

    // start opengl
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(50,50);
	glutCreateWindow("Project");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(handleResize);
	glutSpecialFunc(specKeyFun);
	glutMainLoop();

	return 0;            
}

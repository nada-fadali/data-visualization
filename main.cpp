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
	//cout << "x: " << x << " y: " << y << " z: " << z <<endl;
	if(name.compare(root.name) == 0){
		root.position = Point(x, y, z);
		//cout << root.name  << " " <<  root.position.x <<  " " << root.position.y << " " << root.position.z << endl; 
	}
	else{
		for (unsigned i = 0; i < nodeList.size(); i++){
			if(nodeList[i].name.compare(name) == 0){
				nodeList[i].position = Point(x, y, z);
				//cout << nodeList[i].name << " " << nodeList[i].position.x << " " << nodeList[i].position.y << " " << nodeList[i].position.z << endl;			
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
	p = root.position;
	return p;
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
	glPushMatrix();
		glRotatef(45, 0.0, 1.0, 0.0);
		glTranslatef(x, y, z);
		glScalef(sx, sy, sz);
		//glRotatef(-30, 1.0, 0.0, 0.0);
		for (c = string; *c != '\0'; c++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
		}
	glPopMatrix();
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


	// draw axis
	//drawAxis();

	// draw each level
	int levels = graph.getNumberOfLevels();
	float factor;
	for (int i = 0; i < levels+1; i++){
		factor = (i*1.0 / levels*1.0);

		// draw base plane
		glPushMatrix();
			glColor3f(0.2,0.2,0.0);
			glTranslatef(0, -factor, 0);
			glScalef(factor+0.5, factor+0.5, factor+0.5);
			drawLevelPlane();
		glPopMatrix();

		// get nodes in this level
		vector<string> levelNodes = graph.getNodesInLevel(i);
		// draw nodes
		glPushMatrix();
		for (unsigned j = 0; j < levelNodes.size(); j++){
			glColor3f(1.0,0.0,1.0);
			graph.setNodeLocation(levelNodes[j], j*0.3, -factor+0.5, 0);
			renderStrokeFontString(
				j*0.3,
			    -factor+0.5,
				0,
				0.0008,0.0008,0.0008, 
				&levelNodes[j][0]
			);

			// cout << levelNodes[j] << "  "; // debugging
		}
		// cout << endl; //debugging
		glPopMatrix();
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

    //TODO delete this
    // DEBUGGING
    //<---->
    cout << "name value parent level relation"<<endl;
    cout << graph.root.name << " " << graph.root.value  << "  " << graph.root.level << endl;
    for (unsigned i = 0; i < graph.nodeList.size(); i++)
    {
    	cout << graph.nodeList[i].name << " " << graph.nodeList[i].value << " " << graph.nodeList[i].parent << " " << graph.nodeList[i].level << " " ;
    	if(!graph.nodeList[i].relation.empty())
    		cout << graph.nodeList[i].relation[0];
    	cout << endl;
    }
    cout << "levels: " << graph.getNumberOfLevels() << endl;

    // 	    cout << endl;
    // cout << "Positions" << endl;
    // printf("%s %f %f %f\n", graph.root.name.c_str(), graph.root.position.x, graph.root.position.y, graph.root.position.z);
    // for (unsigned i = 0; i < graph.nodeList.size(); i++)
    // {
    // 	printf("%s %f %f %f\n", graph.nodeList[i].name.c_str(), graph.nodeList[i].position.x, graph.nodeList[i].position.y,  graph.nodeList[i].position.z );
    // }
    //<---->

    int levels = graph.getNumberOfLevels();
    cam.x = levels;
	cam.y = 0;
	cam.z = levels+0.5;

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

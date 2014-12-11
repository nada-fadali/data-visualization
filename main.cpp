#include <GL/glut.h>	// opengl
#include <fstream>		// read txt file
#include <string>		// string
#include <iostream>		// cout
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
    Node* Parent;
    vector<Node*> children;
    Node();
    Node(string,int,vector<Node*>);

};

Node::Node(){
    name = "";
    value = 0;  
}

Node::Node(string n , int v, vector<Node*> c) {
    name = n;
    value = v;
    children = c;
}

//=========> HashNode
class HashNode { 
public:
    Node key;
    Node value;
    HashNode(Node,Node);
    HashNode();  
};

HashNode::HashNode(){ 
}

HashNode::HashNode(Node k, Node v) {
    key = k;
    value = v;
}

//=========> Graph
class Graph {
public:
    Node R1;
    Node R2;
    Node parentR1;
    Node parentR2;
    HashNode relationtable;
    Graph();
    Graph(Node,Node,HashNode);
};

Graph::Graph(){
}

Graph::Graph(Node k, Node v , HashNode rt){
    R1 = k;
    R2 = v;
    relationtable = rt;
}


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
		while ( getline (myfile,line) ){
			//split by ":" 
			vector <string> lineAttr;
			vector <string> roots;
			vector <Node> children;
			Node child;
			//vector <Node> relations;
  
			int counter;

	  		split( lineAttr, line, is_any_of( ":" ) );
			while (!EOF){
		  		for (int i=0 ; i< lineAttr.size(); i++){
			
					if (lineAttr[2]== "{") {
					//roots[counter] = lineAttr[0];
					counter++;
					roots.push_back(lineAttr[0]);
					//child(lineAttr[0],lineAttr[1],NULL) .. this should be for the next line .. needs some modifications
					children.push_back(child);
					Node node(lineAttr(0),lineAttr(1), children);
				} 
				else (lineAttr[2]== "."){
					Node node(lineAttr(0),lineAttr(1), null);
				}
			
				//else { //node
					//create relation
					//relations.push_back(lineAttr[2]);
				//	Node node (lineAttr(0),lineAttr(1), null);
				//}

				if (lineAttr[0] == "}"){
					counter--;
					// create instance 
			
					roots.pop_back();
				}
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
	glutCreateWindow("Project");
	init();
	glutDisplayFunc(display);	
	glutReshapeFunc(handleResize);
	// glutSpecialFunc(specKeyFun);
	glutMainLoop();

	return 0;            
}

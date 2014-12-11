//
//  main.cpp
//  Project
//
//  Created by Ethar Elhinamy on 12/10/14.
//  Copyright (c) 2014 Ethar Elhinamy. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>

using namespace std;
using namespace boost;

class Node {

public:
    string name;
    int value;
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



class HashNode {
    
public:
    Node key;
    Node value;
    HashNode(Node,Node);
    HashNode();
    Node getKey()  {
        return key;
    }
    
    void setKey(Node key) {
    
        HashNode::key = key;
    
    }
    
    Node getValue() {
        return value;
    }
    
    void setValue(Node value) {
        HashNode::value = value;
    }
  
 
    
};

HashNode::HashNode(){
   
    
    
}

HashNode::HashNode(Node k, Node v) {

    key = k;
    value = v;

}


class Graph {

public:
    
    Node R1;
    Node R2;
    Node parentR1;
    Node parentR2;
    HashNode relationtable;
    Graph();
    Graph(Node,Node,HashNode);
    

    
    Node getParentR1(){
    
    
        return parentR1;
    
    }
    
    
    
    Node getParentR2(){
    
    
        return parentR2;
    
    }
};


Graph::Graph(){

}

Graph::Graph(Node k, Node v , HashNode rt){

    R1 = k;
    R2 = v;
    relationtable = rt;



}



int main(int argc, const char * argv[]) {
    // insert code here...
  //read line by line from file
	string line;
  ifstream myfile ("inputs.txt");
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {

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
 


    }
    myfile.close();
  }

  else cout << "Unable to open file"; 
    return 0;
}

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
    vector<Node*> relation;
    vector<Node*> children;
    Node();
    Node(string,int,vector<Node*>,vector<Node*>);

};


Node::Node(){
    name = "";
    value = 0;
    
    
    
}

Node::Node(string n , int v,vector<Node*> r, vector<Node*> c) {



    name = n;
    value = v;
    relation = r;
    children = c;




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
	vector <Node> relations;
  
	int counter;

	  split( lineAttr, line, is_any_of( ":" ) );
		while (!EOF){
		  for (int i=0 ; i< lineAttr.size(); i++){
			
			if (lineAttr[2]== "{") {
				//roots[counter] = lineAttr[0];
				counter++;
				roots.push_back(lineAttr[0]);
				Node node(lineAttr(0),lineAttr(1), children, relations);
			} 
			else if (lineAttr[2]== "."){
				Node node(lineAttr(0),lineAttr(1), null, null);
			}
			
			else { //node

				//create relation
				relations.push_back(lineAttr[2]);
				Node node (lineAttr(0),lineAttr(1), null, relations);
			}

		if (lineAttr[0] == "}"){
			counter--;
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

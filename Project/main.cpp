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

using namespace std;

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
    std::cout << "Hello, World!\n";
    return 0;
}

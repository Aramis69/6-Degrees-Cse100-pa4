/*  File: ActorNode.hpp
 *  Author: Aramis Conde
 *  file defines Actor Node class and instance variables
 * */
#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP 

#include <iostream>
#include "ActorGraph.hpp"

using namespace std;

class MovieNode;
/*  Actor Node class with constructor and instance variables
 * */
class ActorNode{

public:
    //name of actor
    const string name;

    //movies the actor is in
    vector<MovieNode*> Movies;

    //BFS tools
    int dist;
    //reused in MST
    ActorNode* prev;
    //reused in MST
    MovieNode* prevMov;    
   
    //Dijistras tools
    bool done;
    
    //used in MST
    int size;
     
    //constructor
    ActorNode(string n) : name(n),size(1), dist(numeric_limits<int>::max()),
                            prev(nullptr), prevMov(nullptr), done(false) {} 
   
    ~ActorNode(){}
};

#endif 


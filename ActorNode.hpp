#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP 

#include <iostream>
#include "ActorGraph.hpp"

using namespace std;

class MovieNode;

class ActorNode{

public:
    //name of actor
    const string name;

    //movies the actor is in
    vector<MovieNode*> Movies;

    //BFS tools
    int dist;
    ActorNode* prev;
    MovieNode* prevMov;    
    
    //constructor
    ActorNode(string n) : name(n), dist(numeric_limits<int>::max()),
                            prev(nullptr), prevMov(nullptr) {} 
    
};

#endif 


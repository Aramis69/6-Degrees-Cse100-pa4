/*  File: MovieNode.hpp
 *  Author:Aramis Conde
 *  Defines Movie node class
 * */
#ifndef MOVIENODE_HPP
#define MOVIENODE_HPP 

#include <iostream>
#include "ActorGraph.hpp"

using namespace std;

class ActorNode;
/*defines movie node class with constructor and instance variables*/
class MovieNode{

public:
    //name of this movie
    const string name;

    //year this movie was made
    int year;

    //Actors in this movie
    vector<ActorNode*> Actors;

    //constructor 
    MovieNode(string n, int y) : name(n), year(y) {}

    ~MovieNode(){} 
};

#endif
 

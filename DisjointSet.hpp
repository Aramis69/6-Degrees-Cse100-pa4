/* File: DisjointSet.hpp
 * Author:Aramis Conde
 * Hold Disjoint set class definition
 * */
#ifndef DISJOINTSET_HPP
#define DISJOINTSET_HPP 

#include "ActorGraph.hpp"

using namespace std;
/*  DisjointSet class 
 *  defines constructors and functions for using this
 * */
class DisjointSet{

protected:
    
    friend class ActorGraph;

    //struct used when we sort the edge vector
    struct compare_minEdges{
        bool operator()(const pair<MovieNode*,pair<ActorNode*,ActorNode*>> & p1
                        ,const pair<MovieNode*,pair<ActorNode*,ActorNode*>> & p2)
        { 
            unsigned int p1Dist = (1 + (2019 - (p1.first->year)));
            unsigned int p2Dist = (1 + (2019 - (p2.first->year)));
           return p1Dist < p2Dist;
        }
    };

    //pq for actors pairs and movies connecting them 
    vector<pair<MovieNode*, pair<ActorNode*,ActorNode*>>> edges;
 
    //unordered map that holds actor nodes and their parents <node,parent>
    unordered_map<string,ActorNode*>* mstMap;
public:
    //constructor
    DisjointSet();
    //destructor that deletes map.
    ~DisjointSet();

    //method to check if a node what set a node belongs to
    ActorNode* find(ActorNode* n);
    
    //union method to connect disjoint sets
    bool uniteSets(ActorNode* one, ActorNode* two, MovieNode* movie);
    
    //populates priority que with all graph edges
    void makeEdges(unordered_map<string,ActorNode*>* map,
                    unordered_map<string,MovieNode*>* movieMap);

    //builds MST by popping ActorGraphs PQ which is made of edges
    void makeMST(unordered_map<string,ActorNode*>* map);

    //traverses the build MST and prints it out
    void outputMST(ofstream& out, unordered_map<string,ActorNode*>* map);

};

#endif

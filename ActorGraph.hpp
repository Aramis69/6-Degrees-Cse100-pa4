/*
 * ActorGraph.hpp
 * Author: Aramis Conde
 * Date:   6-2-19
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include "ActorNode.hpp"
#include "MovieNode.hpp"
#include "DisjointSet.hpp"


using namespace std;

/**
 * ActorGraph class which contains methods to build actor graph
 * as well as performing different types of searches
 * 
 * Contains contructors and destructors since our graph relies on two 
 * connected unordered maps. A priority que is also initialized
 * in order to use for movietraveler main 
 */
class ActorGraph {
protected:
    friend class DisjointSet;
  
    //holds actor objects
    unordered_map<string, ActorNode*>* map;   
    
    //holds movie objects
    unordered_map<string, MovieNode*>* movieMap;

public:

    //for priority que used in Dijkstras
    struct compare_minPair{
        bool operator()(const pair<ActorNode*,unsigned int> & p1,
                        const pair<ActorNode*,unsigned int> & p2){ 
           return p2.second < p1.second;
        }
    };

    /* Possible optimization i did not need
    *  used to sort movies by smallest year
    struct sortMovies{
        bool operator()(const MovieNode* & p1
                        ,const MovieNode* & p2)
        { 
            unsigned int p1Dist = (1 + (2019 - (p1->year)));
            unsigned int p2Dist = (1 + (2019 - (p2->year)));
           return p2Dist < p1Dist;
        }
    };
    */

    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);

    ~ActorGraph();
    
    void deleteAll();

    //loads pairs but calls djpath to take weight into consideration
    void weightedActorPair(const char* file, ofstream& out);

    //uses Dijkstras algorithm to find least weighted graph between actors
    string djPath(string one, string two);

    //for finding path in graph without using weights
    bool loadActorPair(const char* file, ofstream& out);

    //helper funciton to output a path from actor one to two
    string outputPath(string one, string two);    

    //function that handles movietraveler call
    void travelMovies(ofstream& out);

    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2019 - movie_year), 
     *                      otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     *stream(const std::basic_ofstream<char>&)' is implicitly deleted because the default definition would be ill-formed:
    */

    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
  
};


#endif // ACTORGRAPH_HPP

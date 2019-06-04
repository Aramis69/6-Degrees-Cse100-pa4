/*
 * ActorGraph.cpp
 * Author: Aramis Conde
 * Date:   5-26-19
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in imdb_2019.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.hpp"

using namespace std;

//minimum priority que

ActorGraph::~ActorGraph()
{
    deleteAll();
}

void ActorGraph::deleteAll(){

    //erase elements in actor map    
    for(auto it : *map){
        delete(it.second);
    }
    delete map;

    //erase elements in movie map
    for(auto it : *movieMap){
        delete(it.second);
    }
    delete movieMap;
}

/**
 * Constructor of the Actor graph
 */ 
ActorGraph::ActorGraph(void) {
    map = new unordered_map<string,ActorNode*>();
    movieMap = new unordered_map<string,MovieNode*>();
}

/*this is a helper function that is used by load actor pair*/
string ActorGraph::outputPath(string one, string two){

    unordered_map<string,int> distances;
    //que to keep nodes
    queue<ActorNode*> stack;
    //start of the path 
    ActorNode* start = (*map)[one];
    //Actor node of end to build
    ActorNode* end = (*map)[two];
    //to break loop
    bool reachEnd = false;
    //update distance for start
    start->dist = 0;
    //insert to que
    stack.push(start);
    while(!stack.empty() && reachEnd == false){
        //get actor
        ActorNode* next = stack.front();
        stack.pop(); 
        //iterate through this actors movies
        vector<MovieNode*>::iterator it = next->Movies.begin();
        //go through all movies for this actor
        for( ; it != next->Movies.end(); it++){
            //go through every actor in the movie    
            vector<ActorNode*>::iterator mov = (*it)->Actors.begin();
            for(; mov != (*it)->Actors.end();mov++){
                ActorNode* neighbor = *mov;
                //if the dist has not been set alter values 
                if(distances.find(neighbor->name) == distances.end()){
                    distances[neighbor->name] = 1;
                    neighbor->prevMov = *it;
                    neighbor->prev = next;
                    stack.push(neighbor);
                }
                //check to see if we have reached the end node 
                if(distances.find(end->name) != distances.end()){
                    reachEnd = true;
                    break;
                }
            }
            //break loop
            if(reachEnd == true){
                break;
            }
        }
    }

    //movie of last node
    MovieNode* buildMov = end->prevMov;
    ActorNode* build = end;
    //get the start node
    ActorNode* first = (*map)[one];
    string path;
    bool loop = true;
    //loop and build string
    while(loop){
        //make the path for this actor and move
        string string2 = "";
        string2 += string("--") + "[" + buildMov->name + "#@" + 
                        to_string(buildMov->year) + "]" + "-->" + "(" + 
                        build->name + ")";
        //add to the path 
        path.insert(0,string2);
        //move to the next previous node
        build = build->prev;
        buildMov = build->prevMov;
        //once you reach the start actor
        if(build->name.compare(first->name) == 0){
            //insert last part of path
            path.insert(0 ,"(" + one + ")");
            //break loop
            loop = false;
        }
    }

    return path;
}

/*loads actor pairs and finds unweighted paths between them*/
bool ActorGraph::loadActorPair(const char* in_filename, ofstream& out) {

    // Initialize the file stream
    ifstream infile(in_filename);
    
    out << "(actor)--[movie#@year]-->(actor)--..." << endl; 
    
    bool have_header = false;
    // keep reading lines until the end of file is reached
    while (infile) {
        //hold the line we about to get
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;
        
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        
        istringstream ss( s );
        //to hold different part of this line
        vector <string> record;

        //while on the same line
        while (ss) {
            string next;
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;
            //push this actor name
            record.push_back( next );
        }
    
        if (record.size() != 2) {
            // we should have exactly 3 columns
            continue;
        }
        //two actor names
        string actor1(record[0]);
        string actor2(record[1]);
        
        //if this actor is already in the Actor Graph
        if((map->find(actor1) != map->end()) && (map->find(actor2) != map->end())){
 
            string myReturn = outputPath(actor1, actor2);
        
            out << myReturn << endl;
        }
        else{
            
            out << endl;
        }
    }
    return true;
}

/* Path builder using dijkstas algorithm
 * Goes through actor map and finds shortest path between two actors
 * Returns: string containing path from one to two
 * */
string ActorGraph::djPath(string one,string two){

    //reset actor nodes variables that are used for dijkstras path
    for(auto it : *map){
        //grab actorNode from iterator
        ActorNode* set = it.second;
        //set feilds
        set->dist = numeric_limits<int>::max();
        set->prev = nullptr;
        set->prevMov = nullptr;
        set->done = false;
    }

    //pq for actor nodes and weights
    priority_queue<pair<ActorNode*,unsigned int>, vector<pair<ActorNode*,unsigned
                int>>, compare_minPair> nodes; 
    //get beggining node
    ActorNode* first = (*map)[one];
    //Actor node of end to check when we reach node
    ActorNode* end = (*map)[two];
    //set new distance
    first->dist = 0;
    //make pair for this node 
    pair<ActorNode*,unsigned int> firstPair = make_pair(first,0);
    nodes.push(firstPair);

    //while stuff in pq
    while(!nodes.empty()){
        //top and pop 
        pair<ActorNode*,unsigned int> curr = nodes.top(); 
        nodes.pop();
        
        //check to see if we have reached the end node 
        if((curr.first->name).compare(end->name) == 0){
                break;
        }
        
        //only do if this node has not been marked as done
        if(curr.first->done == false){
            curr.first->done = true;      

            //iterate through this actors movies
            vector<MovieNode*>::iterator it = curr.first->Movies.begin();
            //go through all movies for this actor
            for( ; it != curr.first->Movies.end(); it++){
                //go through every actor in the movie    
                vector<ActorNode*>::iterator mov = (*it)->Actors.begin();
                //get weight of this movie 
                unsigned int distance = curr.first->dist + (1 +(2019 - ((*it)->year)));
                //for all neighbors
                for(; mov != (*it)->Actors.end();mov++){
                    //currents neighbor
                    ActorNode* neighbor = *mov;
                    //if this neighbors distance is greater than this new path
                    if(distance < neighbor->dist){
                        //update feilds
                        neighbor->dist = distance;
                        neighbor->prev = curr.first;
                        neighbor->prevMov = *it;
                        //make new pair for this node
                        pair<ActorNode*,unsigned int> enQ = make_pair(neighbor,distance);
                        nodes.push(enQ);
                    }
                }
            }
        }
    }

    //empty pq
    while(!nodes.empty()){
        nodes.pop();
    }

    //movie of last node
    MovieNode* buildMov = end->prevMov;
    ActorNode* build = end;
    //build string path
    string path;
    bool loop = true;
    //loop and build string
    while(loop){
        //make the path for this actor and move
        string string2 = "";
        string2 += string("--") + "[" + buildMov->name + "#@" + 
                        to_string(buildMov->year) + "]" + "-->" + "(" + 
                        build->name + ")";
        //add to the path 
        path.insert(0,string2);
        //move to the next previous node
        build = build->prev;
        buildMov = build->prevMov;
        //once you reach the start actor
        if(build->name.compare(first->name) == 0){
            //insert last part of path
            path.insert(0 ,"(" + one + ")");
            //break loop
            loop = false;
        }
    }

    //return the path build
    return path;
}

/*  Function that gets pairs of actors and calls helper to return a string
 *  and output to the file
 *  No Return
 *  Outfile is expected to be changed.
 * */
void ActorGraph::weightedActorPair(const char* file,ofstream& out){

    // Initialize the file stream
    ifstream infile(file);
    
    out << "(actor)--[movie#@year]-->(actor)--..." << endl; 
    
    bool have_header = false;
    // keep reading lines until the end of file is reached
    while (infile) {
        //hold the line we about to get
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;
        
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        
        istringstream ss( s );
        //to hold different part of this line
        vector <string> record;

        //while on the same line
        while (ss) {
            string next;
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;
            //push this actor name
            record.push_back( next );
        }
    
        if (record.size() != 2) {
            // we should have exactly 3 columns
            continue;
        }
        //two actor names
        string actor1(record[0]);
        string actor2(record[1]);
        
        //if these actors are already in the Actor Graph
        if((map->find(actor1) != map->end()) && (map->find(actor2) != map->end())){
            //call function that will return path as string
            string myReturn = djPath(actor1, actor2);
            //output this string to outfile
            out << myReturn << endl;
        }
        else{
            
            out << endl;
        }
    }

}


/*  Function called by movietraveler after graph is built
 *  Will call functions to capture edges of the graph and build a 
 *  MST out of all the actor nodes
 *  This function will construct a Disjoint Set in order to call its functions
 *  which will build the MST and output the MST edges to the outFile
 *
 * */
void ActorGraph::travelMovies(ofstream& out){
    
    //Call function to populate priority que of all edges in actor map
    auto it = map->begin();
    //make disSet map
    DisjointSet* disSet = new DisjointSet();
    //start at whatever actor is first in the map
    disSet->makeEdges(map,movieMap);

    disSet->makeMST(map);
    
    disSet->outputMST(out,map);
    
    delete(disSet);
     
}

/** You can modify this method definition as you wish
 *
 * Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename - input filename
 * use_weighted_edges - if true, compute edge weights as 1 + (2019 - movie_year),
 *                      otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {

            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
    
        // TODO: we have an actor/movie relationship, now what?
        
        //if this actor is already in the Actor Graph
        if(map->find(actor_name) != map->end()){
                //if movie is already in movie map
                if(movieMap->find(movie_title + to_string(movie_year)) != 
                    movieMap->end()){

                    //check that they are connected
                    bool connect = false;
                    //load existing actor and movie from maps
                    ActorNode* myActor = (*map)[actor_name];
                    MovieNode* myMovie = (*movieMap)[movie_title + 
                                            to_string(movie_year)];
                    //go through actors movies
                    for(int i = 0; i < myActor->Movies.size(); i++){
                        //check that the movies name is same
                        int eq = (myMovie->name).compare(myActor->Movies[i]->name);
                        //make sure name and year are the same
                        if(eq == 0 && myMovie->year == myActor->Movies[i]->year){
                            //they are already connected
                            connect = true;
                        }
                    }
                    //connect the two 
                    if(connect == false){
                        myActor->Movies.push_back(myMovie);
                        myMovie->Actors.push_back(myActor);   
                    }
                }
                //if movie is new
                else{
                    //create new movie object
                    MovieNode* newMovie = new MovieNode(movie_title,movie_year);
                    //connect actor and movie
                    ActorNode* myActor = (*map)[actor_name];
                    myActor->Movies.push_back(newMovie);
                    newMovie->Actors.push_back(myActor);   
                    //insert to movie map    
                    (*movieMap)[movie_title + to_string(movie_year)] = newMovie;   
                }
        }    
        else{    
            //make a new actor
            ActorNode* myActor = new ActorNode(actor_name);
            //check if movie exists
            if(movieMap->find(movie_title + to_string(movie_year)) != 
                movieMap->end()){
                //get existing movie
                MovieNode* myMovie = (*movieMap)[movie_title + 
                                        to_string(movie_year)];
                //connect the movie and actor
                myActor->Movies.push_back(myMovie);
                myMovie->Actors.push_back(myActor);   
            }
            //if movie doesnt exist as well
            else{
                //create new movie object
                MovieNode* newMovie = new MovieNode(movie_title,movie_year);
                //connect actor and movie
                myActor->Movies.push_back(newMovie);
                newMovie->Actors.push_back(myActor);   
                //insert to movie map    
                (*movieMap)[movie_title + to_string(movie_year)] = newMovie;         
            }
            //insert newly created actor to map
            (*map)[actor_name] = myActor;
        }
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}


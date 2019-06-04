
#include "DisjointSet.hpp"

using namespace std;

DisjointSet::~DisjointSet()
{
        //delete map
        delete mstMap;
}

//constructor for disjoint set
DisjointSet::DisjointSet(){
        mstMap = new unordered_map<string, ActorNode*>();
}

//loops up to find sentinal node
ActorNode* DisjointSet::find(ActorNode* n){
        //get this nodes parent using MST map
        ActorNode* parent = (*mstMap)[n->name];
        //compare the name of this node with its parents name in the mstMap
        if(n->name.compare(parent->name) != 0){
                //if they are not the same then keep looking by moving to parent
                parent = find((*mstMap)[parent->name]);
                //set parent as returned sentinal for compression
                (*mstMap)[n->name] = parent;
        }
        //return node if we have reached sentinal node
        return parent;
}

/*Collects all the existing edges for a node*/
void DisjointSet::makeEdges(unordered_map<string, ActorNode*>* map, 
                unordered_map<string, MovieNode*>* movieMap){

        /* Idea i did not use but might revist
        //push all movies in map into a vector
        vector<MovieNode*> myMovies;
        for(auto it : *movieMap){
        myMovies.push_back(it.second);
        }
        //sort this vector of movies
        sort(myMovies.begin(),myMovies.end(), sortMovies);

        //keep count of edges added since we want #ofactors-1 for MST
        int actorsSize = map.size();
        int edgeCount = 0;
        */

        //for all movies in map
        for(auto it : *movieMap){
                //go through every actor in the movie    
                vector<ActorNode*>::iterator act = (it.second)->Actors.begin(); 

                //for all actors in this movie
                for(; act != (it.second)->Actors.end(); act++){

                        //get node the iterator is pointing to
                        ActorNode* curr = *act;
                        ///start at outer loop at some position
                        vector<ActorNode*>::iterator nestLoop = act;

                        //mark node in this loop as done 
                        curr->done = true;   


                        //connect with other actors in this movie
                        for(; nestLoop != (it.second)->Actors.end(); nestLoop++){

                                //get next up in vector
                                ActorNode* neighbor = *nestLoop;

                                //prevents duplicate edges and edge with itself
                                if(neighbor->done == true){
                                        continue;
                                }

                                //make a pair of two actor nodes
                                pair<ActorNode*,ActorNode*> actorPair = make_pair(curr,neighbor);
                                //make edge pair with Movie and actor combination
                                pair<MovieNode*,pair<ActorNode*,ActorNode*>> edgePair 
                                        = make_pair(it.second, actorPair);
                                //push to edge vector
                                edges.push_back(edgePair);
                        }
                }
                    
                //reset all done fields for this actor vector 
                //to move onto next movie 
                act = (it.second)->Actors.begin(); 
                for(; act != (it.second)->Actors.end(); act++){
                        //grab actorNode from iterator
                        ActorNode* set = *act;
                        //set feilds
                        set->done = false;
                }
        }
}


/*  Goes through vector of edges and attempts to make an MST 
 *  No return 
 *  
 * */
void DisjointSet::makeMST(unordered_map<string,ActorNode*>* map){

        //populate MST map with nodes initially pointing to themselves
        for(auto it : *map){
                //i think there is a simpler way to copy all info but i forgot
                //set parent to itself
                (*mstMap)[it.first] = it.second;
        }

        //sort the edges 
        sort(edges.begin(),edges.end(),compare_minEdges());        
        //get an iterator for the edge vector 
        vector<pair<MovieNode*,pair<ActorNode*, ActorNode*>>>::iterator
                it = edges.begin();

        //get number of actors;
        int nodeCount = map->size();
        //keep track of edges 
        int edgeCount = 0; 
        for(;it != edges.end(); it++){

                //get movie edge that contains actor pair and movie which connects them
                pair<MovieNode*,pair<ActorNode*, ActorNode*>> movieEdge = *it;

                //get actor pair from this movie edge
                pair<ActorNode*,ActorNode*> actors = movieEdge.second;

                //find the sentinal nodes of each actor 
                ActorNode* one = find(actors.first);       
                ActorNode* two = find(actors.second);       

                /*union section of code*/
                bool success = uniteSets(one,two, movieEdge.first);

                //if an edge has been created
                if(success){
                        //keep track of edges 
                        edgeCount++;
                }
                //early termination if MST has been completed
                if((nodeCount-1) == edgeCount){
                        break;
                }
        }
}


/*  Takes two actor nodes which should be sentinal nodes and unifies them 
 *  if they do not belong to the same set
 *  returns false if they cannot be united
 *  returns true if unification is success
 *
 * */
bool DisjointSet::uniteSets(ActorNode* one, ActorNode* two, MovieNode* movie){

        //if they are in the same set
        if(one->name.compare(two->name) == 0){
                //you would not join in this case
                //is there something that we should do??
                return false;
        }
        //if the actors are in diffrent sets then link the sentinal nodes
        else{
                //if actor TWO is in a BIGGER disjoint set
                if(one->size < two->size){
                        //set the edge that connected these two in child 
                        one->prevMov = movie;
                        //set parent in unordered map
                        (*mstMap)[one->name] = two;
                        //update the size of Actor two
                        two->size = one->size + two->size;
                }
                //if actor ONE is in a BIGGER disjoint set
                else if(two->size < one->size){
                        //set the edge that connected these two in child
                        two->prevMov = movie;
                        //set parent in unordered map
                        (*mstMap)[two->name] = one;
                        //update the size of Actor one
                        one->size = two->size + one->size;
                }
                //if the disjoing sets are of same size
                //do same as if ONE was bigger and make it parent
                else{
                        //set the edge that connected these two in child
                        two->prevMov = movie;
                        //make one parent in map
                        (*mstMap)[two->name] = one;
                        //update the size of Actor one
                        one->size = two->size + one->size;
                }
        }
}

void DisjointSet::outputMST(ofstream& out,unordered_map<string,ActorNode*>* map){

    int edgeWeights = 0;
    
    out << "(actor)<--[movie#@year]-->(actor)" << endl;    
    //go through MST nodes and their parents
    for(auto it : *mstMap){

        //go through MST map nodes
        string nodeName = it.first;
        //get parent. Either sentinal because compression or other
        ActorNode* parent = it.second;
        //if at sentinal node
        if(nodeName.compare(parent->name) == 0){
            //skip onto next
            continue;
        }
        //get nodes actorNode
        ActorNode* node = (*map)[nodeName];
        //acess movie that was set for this child
        MovieNode* movie = node->prevMov;

        //make string and output it
        out << string("(") << nodeName << ")<--[" << movie->name << "#@" 
                << to_string(movie->year) << "]-->(" << parent->name << ")" 
                <<endl;
        
        //add to weight
        edgeWeights = edgeWeights + (1+ (2019 - movie->year));   
    }
    out << "#NODE CONNECTED: " << mstMap->size() << endl;
    
    out << "#EDGE CHOSEN: " << (mstMap->size()-1) << endl;
 
    out << "TOTAL EDGE WEIGHTS: " << edgeWeights << endl;
}




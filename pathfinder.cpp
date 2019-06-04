/*  File: pathfinder.cpp
 *  Author:Aramis Conde
 *  Runs pathfinder program which varies based on user input
 *  Gets all function calls from ActorGraph
 * */

#include "ActorGraph.hpp"

using namespace std;
/* main function which will be called  by user using 4 differnt arguments
 * 1st user argument will be actor movie relations such as 
 * Actor<tab>Movie<tab>MovieYear
 * 2nd will be a u or w flag to determine what kind of search to perform
 * 3rd will be actor pairs like Actor<tab>Actor
 * 4th will be the file that the paths will be written to
 * */
int main(int argc, char** argv) {

    //check if our call to the function has the correct number of arguments
    if(argc != 5){
        cout << argv[0] << " called with incorrect arguments." << endl;
        cout << "Usage: " << argv[0] << " movie_cast_tsv_file" << " u/w" 
            << " pairs_tsv_file" << " output_paths_tsv_file" << endl;
        return -1;
    }

    //to set weights or not
    bool weight = false;
    char* argument = argv[2];
    //change if weight flag is set
    if(argument[0] == 'w'){
        weight = true;
    }

    //make new actor graph
    ActorGraph* myGraph = new ActorGraph();
    //call method to load actor movie relationships 
    myGraph->loadFromFile(argv[1],weight);

    //make file to write to
    ofstream outFile;
    outFile.open(argv[4], ios::out);
    
    //check for weight flag
    if(weight == false){
        //loads pairs and outputs paths based on BFS search
        myGraph->loadActorPair(argv[3], outFile); 
    }
    else{
        //loads pairs and outputs paths with minimun weight
        myGraph->weightedActorPair(argv[3], outFile);
    }

    outFile.close();
    //delete to save memory
    delete(myGraph);

    return 1;      
 
}


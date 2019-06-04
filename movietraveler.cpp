/* File: movietraveler.cpp
 * Author:Aramis Conde
 *
 * file used to call functions which buiild a minimum spanning tree
 * and finds a path from one actor to another using the most recent movies
 * possible and least amount of actors
 * Outputs movies and actors to a file
 *
 * */
#include "ActorGraph.hpp"

using namespace std;

int main(int argc, char** argv) {

    if(argc != 3){
        cout << argv[0] << " called with incorrect arguments." << endl;
        cout << "Usage: " << argv[0] << " movie_cast_tsv_file" << 
                " output_paths_tsv_file" << endl;
        return -1;
    }

    //make new actor graph
    ActorGraph* myGraph = new ActorGraph();
    //call method to load actor movie relationships 
    myGraph->loadFromFile(argv[1],false);

    //make file to write to
    ofstream outFile;
    outFile.open(argv[2], ios::out);
    
    //call function to build MST and output it to file
    myGraph->travelMovies(outFile);
    //delete graph
    delete(myGraph);

    return 1;      
 
}


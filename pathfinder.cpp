
#include "ActorGraph.hpp"

using namespace std;

int main(int argc, char** argv) {

    if(argc != 5){
        cout << argv[0] << " called with incorrect arguments." << endl;
        cout << "Usage: " << argv[0] << " movie_cast_tsv_file" << " u/w" 
            << " pairs_tsv_file" << " output_paths_tsv_file" << endl;
        return -1;
    }

    //to set weights or not
    bool weight = false;
    char* argument = argv[2];
    if(argument[0] == 'W'){
        weight = true;
    }

    //make new actor graph
    ActorGraph* myGraph = new ActorGraph();
    //call method to load actor movie relationships 
    myGraph->loadFromFile(argv[1],weight);

    //make file to write to
    ofstream outFile;
    outFile.open(argv[4], ios::out);
    
    myGraph->loadActorPair(argv[3], outFile); 

    delete(myGraph);

    return 1;      
 
}


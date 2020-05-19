#include "annealing_core.h"

int main() {

    // Random function setup
    srand(time(NULL));
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // Shuffle seed
    
    // Main variables
    int nodes, edges;
    std::vector<std::pair<int, int> > edge;
    std::vector<int> f;

    readInput(edge, f, nodes, edges);

    // Setting initial value as current best and defining max number of Annealing calls
    globalOptimum = std::make_pair(cutValue(edge,f,nodes), f);
    int calls = SA_ITERATIONS;

    while(calls--){

        simulatedAnnealing(edge,f,nodes);
        std::shuffle(f.begin(), f.end(), std::default_random_engine(seed));

    }

    print_result();

}

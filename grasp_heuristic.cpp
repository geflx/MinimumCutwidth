#include "core.h"
#include "grasp_functions.h"

int main() {

	// Execution parameters defined in "core.h" file.

    srand(time(NULL));
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); // Shuffle seed

    // Main variables.
    int nodes, edges;
    std::vector<std::pair<int, int> > edge;
    std::vector<int> f;

    readInput(edge, f, nodes, edges);

    // Generates initial solution and call GRASP m-heuristic.
    std::pair<std::vector<int>, int> initial = initialSolution(INITIAL_GREEDY_TRIES, nodes, f, edge);
    std::vector<int> S_best = GRASP(nodes, initial.first, initial.second, edge, GRASP_ITERATIONS, RANDOM_ALPHA); 
  
    printSolution(nodes, S_best, edge);
    
}
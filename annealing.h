#ifndef ANNEALING_H
#define ANNEALING_H

std::vector<int> nextNeighbor(const std::vector <std::pair<int,int> > &edge, std::vector <int>&f, int nodes, int T) { 
    
    std::vector <int> newConfig = f;

    // Dinamically controlling neighborhood, making fewers disturbances in lower temperatures
    double tempFactor = T/(nodes*nodes*4.0); 
    int neighborhoodSize = (int) ceil(tempFactor*nodes) + 2;

    for(int i=0; i<neighborhoodSize/2; i++){
        
        int a = rand() % (nodes-1);
        int b = rand() % (nodes-1);
        
        std::swap(newConfig[a], newConfig[b]);
    }

    return newConfig;
}

void simulatedAnnealing(const std::vector <std::pair<int, int> > &edge, std::vector<int> &f, int nodes) {
    
    // Setting initial parameters
    double T = nodes;
    double Tmin = 0.001;
    
    std::vector <int> solution = f;
    int best = cutValue(edge,f,nodes);
    
    while(T>Tmin){
    
        double trials = 0.0;
        double changes = 0.0;
        double equilibrium = 0.0;
        
        while(trials < SA_MAX_TRIALS) {
                
            trials++;

            solution = nextNeighbor(edge, f, nodes, T);
            int cost = cutValue(edge, solution, nodes);
            int delta = cost - best;
            
            if(cost < globalOptimum.first) {
                
                globalOptimum.first = cost;
                globalOptimum.second = solution;
            
            }

            if(delta <= EPSILON) {
                
                changes++;
                best = cost;
                f = solution;
            
            }
            else {
                // Accepts a bad move with a given probability
        
                double e = 1.0/exp(delta/T);
                double R = (rand()%100 + 1)/100.0;
                if(e>R)	{

                    changes++;
                    best = cost;
                    f = solution; 

                }

            }
        }
        
        // Cooling schedule
        equilibrium = (changes*1.0)/(trials*1.0);
        double k = ( (equilibrium>1)?equilibrium/100.0:equilibrium ) * 0.25;
        T *= 0.60 + k; 

    } 
}

#endif

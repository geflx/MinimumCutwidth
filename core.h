#ifndef CORE_H
#define CORE_H

// Containers & Data types
#include <string>
#include <utility>
#include <vector>

// Auxiliar functions
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>

#define SA_MAX_TRIALS 10000
#define SA_ITERATIONS 10
#define EPSILON 1E-12

std::pair<int,std::vector<int> > globalOptimum;

int cutValue(const std::vector< std::pair<int,int> > &edge, const std::vector<int> &f, int nodes) {

    int maxCut = -1;
    std::vector <int> cutwidth(nodes - 1, 0);

    #pragma omp parallel for
    for(int i=0; i<nodes; i++) {
        for (int j=0; j<edge.size(); j++) {
            
            int fa = std::min(f[edge[j].first], f[edge[j].second]);
            int fb = std::max(f[edge[j].first], f[edge[j].second]);
            
            if (fa <= f[i] && f[i] < fb)
                ++cutwidth[f[i] - 1];

        }
    }

    for(int i=0; i<cutwidth.size(); i++) maxCut = cutwidth[i] > maxCut ? cutwidth[i] : maxCut;
    
    return maxCut;
}

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

std::vector<int> initialGreedy(int nodes, const std::vector<std::pair<int,int> > &edge, const std::vector<std::pair<int,int> > &distance){
    
    std::vector<int> newConfig(nodes);
    std::vector<bool> visited(nodes,false);
    int pos = 1;
    
    for(int i=0; i<distance.size(); i++){

        // From edge e, starting from u and going to v
        int e = distance[i].second;
        int u = edge[e].first;
        int v = edge[e].second;

        if(!visited[u]){ newConfig[u] = pos++; visited[u] = true; }
        if(!visited[v]){ newConfig[v] = pos++; visited[v] = true; }
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

std::vector<std::pair<int,int> > getDistances(const std::vector<std::pair<int,int> > &edge, std::vector<int> &f){

    std::vector<std::pair<int,int> > distance(edge.size());

    for(int i=0;i<edge.size();i++){

        int u = f[edge[i].first];
        int v = f[edge[i].second];

        int d = abs(u-v);
        distance[i] = std::make_pair(d, i);
    }

    std::sort(distance.begin(),distance.end(), std::greater<std::pair<int,int> >());
    
    return distance;
}

std::vector<std::pair<int,int> > readGrid(int &columns){
  
    std::vector<std::pair<int,int> > edge;
    std::string line;
    
    int current_line = 0;
    columns = 0;

    while( std::getline(std::cin, line) ){

        if(current_line == 0) columns=line.size()/2;
        
        for(int i=0; i<line.size(); i++){

            if(line[i] == '1'){

                bool repeating_edge = false;
                for(int j=0; j<edge.size(); j++)
                    if(edge[j].first==(i/2) && edge[j].second==current_line)
                        repeating_edge=true;

                if(!repeating_edge) 
                    edge.push_back(std::make_pair(current_line, i/2));
            }

        }

        current_line++;
    }

    return edge;
}

std::vector<std::pair<int,int> > readGraph(int &nodes, int &edges, std::vector<int> &f){
    
    std::vector<std::pair<int,int> > distance(edges);
    std::vector<std::pair<int,int> > edge(edges);

    for(int i=0; i<edges; i++){
        
        int u,v;
        std::cin >> u >> v;
        
        u--;
        v--;
        
        int d = abs(f[u]-f[v]);
        distance[i] = std::make_pair(d,i);
        edge[i] = std::make_pair(u,v);
    
    }
    
    std::sort(distance.begin(),distance.end());
    std::vector<int> greedy(f);

    greedy = initialGreedy(nodes, edge, distance);
    
    int greedyCost = cutValue(edge, greedy, nodes);
    int initialCost = cutValue(edge, f, nodes);  
    
    if( greedyCost < initialCost ) f = greedy;

    return edge;
}

void readInput(std::vector<std::pair<int,int> > &edge, std::vector<int> &f, int &nodes, int &edges) {

    std::string line;
    std::getline(std::cin, line);

    // Non-grid instances - Harwell and small
    if(line.find("Grid") == std::string::npos) {
    
        // Reading the number of vertices (it's repeated on the file)
        std::cin >> nodes >> nodes >> edges; 
        
         //Initializing positions vector {1,2,3,...,n}
        f.resize(nodes);
        for (int i=0; i<nodes; i++) f[i] = i + 1;

        edge = readGraph(nodes, edges, f);
    }
    else {

        nodes = 0;
        edge = readGrid(nodes);

        f.resize(nodes);
        for(int i=0; i<nodes; i++) f[i] = i + 1; 
        
        std::vector<std::pair<int,int> > distance = getDistances(edge, f);
        std::vector<int> greedy = initialGreedy(nodes,edge,distance);

        int greedyCost = cutValue(edge, greedy, nodes); 
        int initialCost = cutValue(edge, f, nodes);

        if( greedyCost < initialCost ) f = greedy;

    }

}

void print_result() {

    std::cout << "{ ";
    for(int i=0; i<globalOptimum.second.size(); i++) 
        std::cout << (globalOptimum.second)[i] << " ";
    std::cout << "} Minimum Maximum Cut: " << globalOptimum.first << "\n";

}

#endif
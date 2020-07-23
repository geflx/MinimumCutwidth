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

// Simulated Annealing parameters
#define SA_MAX_TRIALS 10000
#define SA_ITERATIONS 10
#define EPSILON 1E-12

// Grasp heuristic parameters
#define INITIAL_GREEDY_TRIES 10000
#define GRASP_ITERATIONS 5000
#define RANDOM_ALPHA 4

std::pair<int,std::vector<int> > globalOptimum;

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

    std::cout << "Input file: "; 
    std::string line;
    std::getline(std::cin, line);

    // Non-grid instances - Harwell and small
    if(line.find("Grid") == std::string::npos) {
        
        std::cout << "Input nodes, nodes (again) and edges: "; 
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
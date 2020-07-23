#ifndef GRASP_FUNCTIONS_H
#define GRASP_FUNCTIONS_H

std::vector<int> nextNeighbor(const std::vector <std::pair<int,int> > &edge, std::vector <int>&f, int nodes) {

    std::vector <int> newConfig = f;

    int neighborhoodSize = (int) ceil(0.1*nodes);
    std::vector<bool> randomHistory(nodes,false);

    for(int i=0; i<neighborhoodSize/2; i++){
        
        int a = rand() % (nodes-1);
        int b = rand() % (nodes-1);
        
        while(randomHistory[a]) a = rand() % (nodes-1);
        randomHistory[a] = true;
        while(randomHistory[b]) b = rand() % (nodes-1);
        randomHistory[b] = true;
        
        randomHistory[a] = true;
        randomHistory[b] = true;
        
        std::swap(newConfig[a], newConfig[b]);
    }

    return newConfig;
}

std::vector<int> auxiliarFunction(int nodes, const std::vector<std::vector<int>> &adjList, const std::vector<int> &rclOriginal, int randomParameter){

    std::vector<int> greedyRandom(nodes,-1);
    int upperMiddle = nodes/2;
    int lowerMiddle = (nodes/2)-1 >= 0 ? (nodes/2 - 1) : -1;
    int insertedNodes = 0;

    std::vector<int> rcl = rclOriginal;
    std::vector<bool> visited (nodes, false);

    while(insertedNodes < nodes){  

        // Forbide wrong position access.
        if(lowerMiddle < 0 && upperMiddle >= nodes){ 
            break;
        }

        // Inserting an aleatory node at each iteration.
        int chosenNode = rand()%nodes;

        while( visited[chosenNode] )
            chosenNode = rand()%nodes;

        visited[chosenNode] = true;
        insertedNodes++;

        // Choose Left or Right position to insert node.
        if(upperMiddle < nodes) 
            greedyRandom[chosenNode] = upperMiddle + 1;
        else
            greedyRandom[chosenNode] = lowerMiddle + 1;
    

        if(greedyRandom[chosenNode] == upperMiddle + 1)
            upperMiddle++;
        else
            lowerMiddle--;

        // Indicates which position insert into (Left - false/Right - true).
        bool clock = false; 
        
        // Refreshing RCL List to valorize adjacent nodes of previously inserted one.
        for(int i: adjList[chosenNode])
            rcl[i] += 2;
        
        for(int i=0; i < adjList[chosenNode].size(); i++){

            int maxi =- 1;

            for(int j=0; j < nodes; j++)
                if(!visited[j] && rcl[j] >= maxi) // Pick max. value node of RCL.
                    maxi = j;
               
            // Inserting max. value node.
            int bestNode = maxi;

            if(insertedNodes >= nodes) // Extra position check up.
                return greedyRandom;

            if(!visited[bestNode]){

                visited[bestNode] = true;
                insertedNodes++;

                if(!clock){ // Insert node in Left position.

                    if(lowerMiddle >= 0){

                        greedyRandom[bestNode] = lowerMiddle + 1;
                        lowerMiddle--;

                    }else{ 

                        greedyRandom[bestNode] = upperMiddle + 1;
                        upperMiddle++;
                        
                    }
                    clock = true;

                }else{ // Insert ndoe in Right position.

                    if(upperMiddle<nodes){

                        greedyRandom[bestNode] = upperMiddle + 1;
                        upperMiddle++;

                    }else{

                        greedyRandom[bestNode] = lowerMiddle + 1;
                        lowerMiddle--;

                    }
                    clock=false;
                }
            }
        }
    }
    return greedyRandom; // Return solution.
}

std::vector<int> GRASP(int nodes,std::vector<int> &iniSolution, int iniCutValue, const std::vector<std::pair<int,int> > &edge, int itnumber, int randomParameter){
   
    std::vector<std::vector<int>> adjList(nodes);
    std::vector<std::pair<int,int>> edgesOnIt(nodes);

    std::vector<int> rcl(nodes, 0); //Restricted candidate list (RCL).

    // Create ADJ List.
    for(int i=0; i< edge.size(); i++){

        adjList[ edge[i].first ].push_back( edge[i].second );
        adjList[ edge[i].second ].push_back( edge[i].first );

    }
    // Main variables.
    for(int i = 0;i < nodes; i++){
        edgesOnIt[i].second = i;
        edgesOnIt[i].first = 0; // Number of adjacent edges.
    }

    for(int i = 0;i < edge.size(); i++){
        edgesOnIt[ edge[i].first ].first++;
        edgesOnIt[ edge[i].second ].first++;
    }
    sort(edgesOnIt.begin(), edgesOnIt.end(), std::greater<std::pair<int,int> >());;

    // Sum "1" of weight in 40% nodes with most edges in RCL List.
    for(int i = 0; i<= 0.5 * nodes; i++)
        rcl[i] += 2;
    
    // Global best values/solutions.
    int bestCutValue = iniCutValue;
    std::vector<int> bestConfig = iniSolution;

    int currCutValue;
    std::vector<int> currSolution;

    currSolution = auxiliarFunction(nodes, adjList, rcl, randomParameter); // Local solution.
    currCutValue = cutValue(edge, currSolution, nodes);

    if(bestCutValue >= currCutValue){ // Best solution check-up.
    
        bestConfig = currSolution;
        bestCutValue = currCutValue;

    }
    while(itnumber--){ // Do local search on local solution.

        int trocas = 10000;
        int trocasInicial  = trocas;
        int stuck = 0;
        while (trocas--){
            if(stuck > ceil(0.1 * trocasInicial)){  // LS will stop in case of 1000 consecutive iterations without any improvement.
                break;
            }
            std::vector<int> tempConfig = nextNeighbor(edge, currSolution, nodes);
            int neighborCutValue = cutValue(edge, tempConfig, nodes);
            if(neighborCutValue <= currCutValue){
                currSolution = tempConfig;
                currCutValue = neighborCutValue;
                stuck = 0; 
            }else{
                ++stuck;
            }
        } 
        if(currCutValue < bestCutValue){ // Best solution check-up.
            bestConfig = currSolution;
            bestCutValue = currCutValue;
        }
        currSolution = auxiliarFunction(nodes, adjList, rcl, randomParameter); // Local solution.
        currCutValue = cutValue(edge, currSolution, nodes);
    }    

    return bestConfig; // Return best solution.
}

std::pair<std::vector<int>, int> initialSolution(int tries, int nodes, std::vector<int> &f, const std::vector<std::pair<int, int>> &edge){
    
    int totalTries = tries;
    int stuck = 0;
    int obj = cutValue(edge, f, nodes);

    while (tries--){

        if(stuck> ceil(0.1 * totalTries)){ // Finish if got stuck in 10 percent of tries.
            break;
        }

        std::vector<int> ngbor = nextNeighbor(edge, f, nodes);
        int ngbor_obj = cutValue(edge,ngbor,nodes);

        if(ngbor_obj < obj){

            f = ngbor;
            obj = ngbor_obj;
            stuck = 0; 

        }else{
            ++stuck;
        }
    }
    return {f, obj};
}

void printSolution(int nodes, const std::vector<int> &S, const std::vector<std::pair<int, int>> &edge){

    std::cout<<"Solution: ";

    for(int i=0; i < S.size(); i++)
        std::cout << S[i] << " ";
    
    std::cout << "\n" << "Obj F. Value: " << cutValue(edge, S, nodes) << "\n";
}

#endif
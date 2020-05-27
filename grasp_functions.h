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

    while(insertedNodes < nodes){  //Inserir todos os vertices na solucao.

        if(lowerMiddle < 0 && upperMiddle >= nodes){ //Imperir acesso de posicao invalida.
            break;
        }
        //A cada iteracao um vertice aleatorio e' inserido
        int chosenNode = rand()%nodes;

        while( visited[chosenNode] )
            chosenNode = rand()%nodes;

        visited[chosenNode] = true;
        insertedNodes++;

        if(upperMiddle < nodes) //Escolhendo posicao para inserir (esquerda ou direita)
            greedyRandom[chosenNode] = upperMiddle + 1;
        else
            greedyRandom[chosenNode] = lowerMiddle + 1;
    

        if(greedyRandom[chosenNode] == upperMiddle + 1)
            upperMiddle++;
        else
            lowerMiddle--;

        bool clock = false; //Variavel para indicar se e' para inserir na esquerda (false) ou direita (true)
        
        //Atualizando a lista RCL para valorizar vertices adjacentes ao inserido
        for(int i: adjList[chosenNode])
            rcl[i] += 2;
        
        for(int i=0; i < adjList[chosenNode].size(); i++){

            int maxi =- 1;

            for(int j=0; j < nodes; j++)
                if(!visited[j] && rcl[j] >= maxi) //Escolhendo o vertice de maior valor da RCL List
                    maxi = j;
               
            
            //Inseriremos agora o maior da candidate list
            int bestNode = maxi;

            if(insertedNodes >= nodes) //Extra Stop condition
                return greedyRandom;

            if(!visited[bestNode]){

                visited[bestNode] = true;
                insertedNodes++;

                if(!clock){ //Inserir na esquerda: clock=false

                    if(lowerMiddle >= 0){

                        greedyRandom[bestNode] = lowerMiddle + 1;
                        lowerMiddle--;

                    }else{

                        greedyRandom[bestNode] = upperMiddle + 1;
                        upperMiddle++;
                        
                    }
                    clock = true;

                }else{//Inserir na direita: clock=true

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
    return greedyRandom; //retorna solucao.
}

std::vector<int> GRASP(int nodes,std::vector<int> &iniSolution, int iniCutValue, const std::vector<std::pair<int,int> > &edge, int itnumber, int randomParameter){
   
    std::vector<std::vector<int>> adjList(nodes);
    std::vector<std::pair<int,int>> edgesOnIt(nodes);

    std::vector<int> rcl(nodes, 0); //Restricted candidate list

    //Criando a lista de adjacencia
    for(int i=0; i< edge.size(); i++){

        adjList[ edge[i].first ].push_back( edge[i].second );
        adjList[ edge[i].second ].push_back( edge[i].first );

    }
    //Inicializando std::vector de structs que serao usados
    for(int i = 0;i < nodes; i++){
        edgesOnIt[i].second = i;
        edgesOnIt[i].first = 0; //Numero de arestas adjacentes
    }

    for(int i = 0;i < edge.size(); i++){
        edgesOnIt[ edge[i].first ].first++;
        edgesOnIt[ edge[i].second ].first++;
    }
    sort(edgesOnIt.begin(), edgesOnIt.end(), std::greater<std::pair<int,int> >());;

    //Aumentando 1 no peso dos 40 por cento vertices com mais arestas na RCL.
    for(int i = 0; i<= 0.5 * nodes; i++)
        rcl[i] += 2;
    
    //Otimos gerais
    int bestCutValue = iniCutValue;
    std::vector<int> bestConfig = iniSolution;

    int currCutValue;
    std::vector<int> currSolution;

    currSolution = auxiliarFunction(nodes, adjList, rcl, randomParameter); //Criando uma solucao para comparar com a inicial
    currCutValue = cutValue(edge, currSolution, nodes);

    if(bestCutValue >= currCutValue){ //Atualizando os melhores resultados
    
        bestConfig = currSolution;
        bestCutValue = currCutValue;

    }
    while(itnumber--){ //Busca local em cima da solucao obtida.

        int trocas = 10000;
        int trocasInicial  = trocas;
        int stuck = 0;
        while (trocas--){
            if(stuck > ceil(0.1 * trocasInicial)){  //A busca local devera' terminar caso nao melhore em 1000 iteracoes seguidas.
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
        if(currCutValue < bestCutValue){ //Atualizando os melhores resultados gerais.
            bestConfig = currSolution;
            bestCutValue = currCutValue;
        }
        currSolution = auxiliarFunction(nodes, adjList, rcl, randomParameter); //Gerando nova solucao
        currCutValue = cutValue(edge, currSolution, nodes);
    }    

    return bestConfig; //Retorna melhor configuracao.
}

std::pair<std::vector<int>, int> initialSolution(int tries, int nodes, std::vector<int> &f, const std::vector<std::pair<int, int>> &edge){
    
    int totalTries = tries;
    int stuck = 0;
    int obj = cutValue(edge, f, nodes);

    while (tries--){

        if(stuck> ceil(0.1 * totalTries)){ //Finishes if 10% of total tries run out
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
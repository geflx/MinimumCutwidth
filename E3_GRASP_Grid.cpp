#include <bits/stdc++.h>
#define DEBUG_MODE
using namespace std;

// Acha o valor do corte máximo.
int cutValue(const vector <pair<int,int> > &edge,
    const vector<int>&f, int numVertices) {

    int maxCut = -1;
    vector <int> cutwidth(numVertices - 1, 0);

    // Identificando cortes
    for (int i = 0; i < edge.size(); i++) {

        int fa = min(f[edge[i].first], f[edge[i].second]);
        int fb = max(f[edge[i].first], f[edge[i].second]);

        for (int j = 0; j < numVertices; j++) {
            if (fa <= f[j] && f[j] < fb)
                ++cutwidth[f[j] - 1];
        }
    }

    for (int i = 0; i < cutwidth.size(); i++) maxCut = cutwidth[i] > maxCut ? cutwidth[i] : maxCut;
    return maxCut;
}

vector<pair<int,int>> readGrid(int &numColunas){
  vector<pair<int,int> > edge;
  string line;
  int contadorLinha=0;
  numColunas=0;
  while(getline(cin,line)){
    if(contadorLinha==0){
      numColunas=line.size()/2;
    }
    for(int i=0;i<line.size();i++){
      if(line[i]=='1'){
      	bool arestaRepetida=false;
        for(int j=0;j<edge.size();j++){
        	if(edge[j].first==(i/2) && edge[j].second==contadorLinha){
        		arestaRepetida=true;
        	}
        }
        if(!arestaRepetida){
        	edge.push_back(make_pair(contadorLinha,i/2));
        }

      }
    }
    ++contadorLinha;
  }
  return edge;
}

vector < int > localSearch(const vector <pair<int,int> > &edge, vector <int>&f, int numVertices) {

    int currCut = cutValue(edge, f, numVertices);
    vector < int > newConfig = f;

    int a, b;
    a = rand() % (numVertices - 1);
    b = rand() % (numVertices - 1);
    while (a == b) a = rand() % (numVertices - 1);

    swap(newConfig[a], newConfig[b]);

    int newCut = cutValue(edge, newConfig, numVertices);

    return newCut < currCut ? newConfig : f;
}


vector<int> initialGreedy(int numVertices, const vector<pair<int,int> > &edge,const vector<pair<int,int> > &dist){
    vector<int> novaConfig(numVertices);
    int cont=1;
    vector<bool> visitado(numVertices,false);
    for(int i=0;i<dist.size();i++){

        int position=dist[i].second;
        int firstGuy=edge[position].first;
        int secondGuy=edge[position].second;

        if(!visitado[firstGuy]){
            novaConfig[firstGuy]=cont++; 
            visitado[firstGuy]=true;

        }
        if(!visitado[secondGuy]){
            novaConfig[secondGuy]=cont++;
            visitado[secondGuy]=true;
        }
    }
    return novaConfig;
}



vector <int> nextNeighbor(const vector <pair<int,int> > &edge, vector <int>&f, int numVertices) {

    vector <int> newConfig = f;

    int neighborhoodSize = (int) ceil(0.1*numVertices);
    vector<bool> randomHistory(numVertices,false);
    for(int i=0; i<neighborhoodSize/2; i++){
        
        int a = rand() % (numVertices-1);
        int b = rand() % (numVertices-1);
        
        while(randomHistory[a]) a = rand() % (numVertices-1);
        randomHistory[a] = true;
        while(randomHistory[b]) b = rand() % (numVertices-1);
        randomHistory[b] = true;
        
        randomHistory[a] = true;
        randomHistory[b] = true;
        
        swap(newConfig[a], newConfig[b]);
    }

    return newConfig;
}
vector<int> auxGraspGreedy(int numVertices, const vector<vector<int> > &adjList, const vector<int> &rclOriginal, int randomParameter){
    vector<int> greedyRandom(numVertices,-1);
    int upperMiddle=((numVertices/2));
    int lowerMiddle=(numVertices/2)-1>=0?((numVertices/2)-1):-1;
    int alreadyFixed=0;

    vector<int> rcl=rclOriginal;
    vector<bool> visited(numVertices,false);
    while(alreadyFixed<numVertices){
        if(lowerMiddle<0 && upperMiddle>=numVertices){
            break;
        }
        //A cada iteracao um vertice aleatorio e' inserido
        int randomNumber= rand()%numVertices;
        while(visited[randomNumber])
            randomNumber = rand()%numVertices;
        int whoInsert = randomNumber;

        visited[whoInsert]=true;
        alreadyFixed++;
        if(upperMiddle<numVertices){ //Escolhendo posicao para inserir (esquerda ou direita)
            greedyRandom[whoInsert]=upperMiddle+1;
        }else{
            greedyRandom[whoInsert]=lowerMiddle+1;
        }
        if(greedyRandom[whoInsert]==upperMiddle+1)
            upperMiddle++;
        else
            lowerMiddle--;

        bool clock=false;
        //Atualizando a lista RCL para valorizar vertices adjacentes ao inserido
        for(int i:adjList[whoInsert]){
            rcl[i]+=2;
        }   
        for(int i=0;i<adjList[whoInsert].size();i++){
            int max=-1;
            for(int j=0;j<numVertices;j++){
                if(!visited[j] && rcl[j]>=max){
                    max=j;
                }
            }
            //Inseriremos agora o maior da candidate list
            int nowInsert=max;
            if(alreadyFixed>=numVertices) //Extra Stop condition
                return greedyRandom;
            if(!visited[nowInsert]){
                visited[nowInsert]=true;
                alreadyFixed++;
                if(!clock){ //Inserir na direita: clock=false
                    if(lowerMiddle>=0){
                        greedyRandom[nowInsert]=lowerMiddle+1;
                        lowerMiddle--;
                    }else{
                        greedyRandom[nowInsert]=upperMiddle+1;
                        upperMiddle++;
                    }
                    clock=true;
                }else{//Inserir na direita: clock=true
                    if(upperMiddle<numVertices){
                        greedyRandom[nowInsert]=upperMiddle+1;
                        upperMiddle++;
                    }else{
                        greedyRandom[nowInsert]=lowerMiddle+1;
                        lowerMiddle--;
                    }
                    clock=false;
                }
            }
        }
    }
    return greedyRandom;
}

vector<int> GraspGreedy(int numVertices,vector<int> &iniSolution, int iniCutValue, const vector<pair<int,int> > &edge, int itnumber,int randomParameter){
    vector<vector<int> > adjList(numVertices);
    vector<pair<int,int >> edgesOnIt(numVertices);

    vector<int> rcl(numVertices,0); //Restricted candidate list

    //Criando a lista de adjacencia
    for(int i=0;i<edge.size();i++){
        adjList[edge[i].first].push_back(edge[i].second);
        adjList[edge[i].second].push_back(edge[i].first);
    }
    //Inicializando vector de structs que serao usados
    for(int i=0;i<numVertices;i++){
        edgesOnIt[i].second=i;
        edgesOnIt[i].first=0; //Numero de arestas adjacentes
    }

    for(int i=0;i<edge.size();i++){
        edgesOnIt[edge[i].first].first++;
        edgesOnIt[edge[i].second].first++;
    }
    sort(edgesOnIt.begin(),edgesOnIt.end(),greater<pair<int,int> >());;

    //Aumentando 1 no peso dos 40 por cento vertices com mais arestas em RCL.
    for(int i=0;i<=0.5*numVertices;i++){
        rcl[i]+=2;
    }

    //Otimos gerais
    int bestCutValue=iniCutValue;
    vector<int> bestConfig=iniSolution;

    int currCutValue;
    vector<int> currSolution;

    currSolution=auxGraspGreedy(numVertices,adjList,rcl,randomParameter);
    currCutValue=cutValue(edge,currSolution,numVertices);
    if(bestCutValue>=currCutValue){
        bestConfig=currSolution;
        bestCutValue=currCutValue;
    }
    while(itnumber--){
        int trocas = 1000;
        int trocasInicial= trocas;
        int naoMelhora=0;
        while (trocas--){
            if(naoMelhora> ceil(0.1*trocasInicial)){
                break;
            }
            vector<int> tempConfig= nextNeighbor(edge, currSolution, numVertices);
            int neighborCutValue=cutValue(edge,tempConfig,numVertices);
            if(neighborCutValue<=currCutValue){
                currSolution=tempConfig;
                currCutValue=neighborCutValue;
                naoMelhora=0; //Melhorou! Reset.
            }else{
                ++naoMelhora;
            }
        }
        if(currCutValue<bestCutValue){
            bestConfig=currSolution;
            bestCutValue=currCutValue;
        }
        currSolution=auxGraspGreedy(numVertices,adjList,rcl,randomParameter);
        currCutValue=cutValue(edge,currSolution,numVertices);
    }    

    return bestConfig;
}



vector<pair<int,int> > readGraph(int numVertices,int numArestas, vector<int> &f){

  vector<pair<int,int> > dist(numArestas);
  vector<pair<int,int> > edge(numArestas);
  int temp1,temp2;

  for(int i=0;i<numArestas;i++){
    cin >> temp1 >> temp2;
    //Subtraindo para forma de 0..n
    --temp1;
    --temp2;
    int distVertices=abs(f[temp1]-f[temp2]);
    dist[i]=(make_pair(distVertices,i));
    edge[i]=make_pair(temp1,temp2);
  }
  sort(dist.begin(),dist.end());
  vector<int> temp = initialGreedy(numVertices,edge,dist);
  if(cutValue(edge,temp,numVertices) < cutValue(edge,f,numVertices))
  	f=temp;
  return edge;
}

vector<pair<int,int> > calculaDist(const vector<pair<int,int> > &edge, const vector<int> &f){
    vector<pair<int,int> > dist(edge.size());
    for(int i=0;i<edge.size();i++){ //Calculando a distancia entre os vertices de cara aresta.
        int distVertices=abs(f[edge[i].first]-f[edge[i].second]);
        dist[i]=(make_pair(distVertices,i));
    }
    sort(dist.begin(),dist.end(),greater<pair<int,int> >());
    return dist;
}


int main() {
    srand(time(NULL));
    string line;
    getline(cin, line); //Ignorar a primeira linha. Nome do problema.

    int numVertices=0;
    int numArestas;

    bool isSmallHarwell=false;
    vector<int> f;
    vector<pair<int, int> > edge;
    vector<pair<int,int> > dist;

    if(isSmallHarwell){
	    cin >> numVertices >> numVertices; //2a linha: extra + numVertices (ou vice-versa pois esta repetido
	    cin >> numArestas;
    	for (int i = 0; i < numVertices; i++)
    		f.push_back(i+1); //Iniciando vetor da forma {1,2,3,...,n}
    	edge = readGraph(numVertices, numArestas,f);
    
    }else{ //Entao e' grid
    	edge = readGrid(numVertices);
    	for (int i = 0; i < numVertices; i++)
    		f.push_back(i+1); //Iniciando vetor da forma {1,2,3,...,n}
    	dist= calculaDist(edge,f); //O calcula dist da versao grid nao e' embutido no readGrid
    	vector<int> iniConfig=initialGreedy(numVertices,edge,dist);
    	if(cutValue(edge,iniConfig,numVertices)<cutValue(edge,f,numVertices)){
    		f=iniConfig;
    	}
    }

    if(isSmallHarwell){
    	dist=calculaDist(edge,f);
    }

    for(int i=0;i<1;i++){

	 	int trocas = 10000;
	    int trocasInicial= trocas;
	    int naoMelhora=0;
		int currCutValue=cutValue(edge,f,numVertices);
	    while (trocas--){
	        if(naoMelhora> ceil(0.1*trocasInicial)){ //A busca local devera' terminar
	            break;
	        }
	    	vector<int> tempConfig= nextNeighbor(edge, f, numVertices);
	    	int neighborCutValue=cutValue(edge,tempConfig,numVertices);
	    	if(neighborCutValue<currCutValue){
	    		f=tempConfig;
	    		currCutValue=neighborCutValue;
	            naoMelhora=0; //Melhorou! Reset.
	    	}else{
	            ++naoMelhora;
	        }
	    }
	    vector<int> grasp=GraspGreedy(numVertices,f,currCutValue,edge,2000,4); //de 500 (testes feitos) pra 2000
        cout<<"Configuração obtida: ";
        for(int i=0;i<grasp.size();i++){
            cout<<grasp[i]<<" ";
        }cout<<endl;
	    cout<<cutValue(edge,grasp,numVertices)<<endl;
    }
}
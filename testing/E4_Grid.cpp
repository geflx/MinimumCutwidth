
/*  Trabalho 1 - INF 492
    Gabriel / Lucas
    Para usar processadores em paralelo, compilar com:
    g++ E4_Grid.cpp -O3 -fopenmp
*/

#include <bits/stdc++.h>

using namespace std;
pair<int, vector<int> > globalOptimum;

// Acha o valor do corte máximo.
int cutValue(const vector <pair<int,int> > &edge, const vector<int>&f, int numVertices) {

    int maxCut = -1;
    vector <int> cutwidth(numVertices - 1, 0);

    // Identificando cortes
    #pragma omp parallel for
    for(int i=0; i<numVertices; i++) {

        for (int j = 0; j < edge.size(); j++) {
            
            int fa = min(f[edge[j].first], f[edge[j].second]);
            int fb = max(f[edge[j].first], f[edge[j].second]);
            
            if (fa <= f[i] && f[i] < fb)
                ++cutwidth[f[i] - 1];
        }
    }

    for(int i=0; i<cutwidth.size(); i++) maxCut = cutwidth[i] > maxCut ? cutwidth[i] : maxCut;
    return maxCut;
}

vector <int> nextNeighbor(const vector <pair<int,int> > &edge, vector <int>&f, int numVertices, int T) { // Less Random when temp is decreasing
    
    vector <int> newConfig = f;    
    int neighborhoodSize = (int) ceil(0.1*numVertices+1);
    
    // double tempFactor = T/(numVertices*numVertices*1.0); // Less Random when temp is decreasing
    // int neighborhoodSize = (int) ceil((0.1*tempFactor)*numVertices) + 1;
    
    for(int i=0; i<neighborhoodSize/2; i++){
        
        int a = rand() % (numVertices-1);
        int b = rand() % (numVertices-1);
        
        swap(newConfig[a], newConfig[b]);
    }

    return newConfig;
}

vector<int> initialGreedy(int numVertices, const vector<pair<int,int> > &edge, const vector<pair<int,int> > &dist){
    
    vector<int> novaConfig(numVertices);
    vector<bool> visitado(numVertices,false);
    int cont=1;
    
    for(int i=0;i<dist.size();i++){

        int position = dist[i].second;
        int firstGuy = edge[position].first;
        int secondGuy = edge[position].second;

        if(!visitado[firstGuy]){
            novaConfig[firstGuy] = cont++; 
            visitado[firstGuy] = true;
        }
        if(!visitado[secondGuy]){
            novaConfig[secondGuy] = cont++;
            visitado[secondGuy] = true;
        }
    }
    
    return novaConfig;
}


vector<pair<int,int> > calculaDist(const vector<pair<int,int> > &edge, vector<int> &f){

    vector<pair<int,int> > dist(edge.size());

    for(int i=0;i<edge.size();i++){

        int u = f[edge[i].first];
        int v = f[edge[i].second];

        int distVertices = abs(u-v);
        dist[i] = {distVertices,i};
    }
    sort(dist.begin(),dist.end(), greater<pair<int,int> >());
    
    return dist;
}

vector<pair<int,int>> readGrid(int &numColunas){
  
  vector<pair<int,int> > edge;
  string line;
  
  int contadorLinha = 0;
  numColunas = 0;
  
  while(getline(cin,line)){

    if(contadorLinha == 0)
      numColunas=line.size()/2;
    
    for(int i=0;i<line.size();i++){
      if(line[i] == '1'){
        
        bool arestaRepetida=false;
        for(int j=0;j<edge.size();j++)
            if(edge[j].first==(i/2) && edge[j].second==contadorLinha)
                arestaRepetida=true;

        if(!arestaRepetida)
            edge.push_back(make_pair(contadorLinha,i/2));
      }
    } // External for
    
    ++contadorLinha;
  }
  return edge;
}

void simulatedAnnealing(const vector <pair<int, int> > &edge, vector<int> &f, int numVertices) {

	
    // Setting initial parameters
	vector <int> s = f;
	double T = numVertices*numVertices;
	double Tmin = 0.001;
	int maxTrials = numVertices;
	int currBest = cutValue(edge,f,numVertices);
	
	while(T>Tmin){
	
		double trials = 0.0;
		double changes = 0.0;
		double equilibrium = 0.0;
		
		while(trials < maxTrials) {
				
			trials++;
			s = nextNeighbor(edge, f, numVertices, T);

			int currSol = cutValue(edge, s, numVertices);
			int delta = currSol - currBest;
			
		    if(currSol < globalOptimum.first) {
                globalOptimum.first = currSol;
                globalOptimum.second = s;
		    }

		    if(delta <= 0) {
                changes++;
                currBest = currSol;
                f = s;
		    } else {
                
                double e = 1.0/exp(delta/T);
                double R = (rand()%100 + 1)/100.0;
                
                // Accepts a bad move with a given probability
                if(e>R)	{
                    changes++;
                    currBest = currSol;
                    f = s; 
                }
            }		
		} // Internal Loop

        // Cooling schedule
		equilibrium = (changes*1.0)/(trials*1.0);
		double k = ((equilibrium>1)?equilibrium/100.0:equilibrium)*0.25;
		T *= 0.60 + k; 
	
    } // Temperature loop
}

int main() {

    srand(time(NULL)); // Random seed
    unsigned seed = chrono::system_clock::now().time_since_epoch().count(); // Shuffle seed

    string line;
    getline(cin, line); // Reads problem name

    int numVertices = 0;
    vector<pair <int, int> > dist;    
    vector<pair <int, int> > edge = readGrid(numVertices);
    vector<int> f(numVertices, 1);
    vector<int> reset;

    for (int i=0; i<numVertices; i++) f[i] = i + 1; //Iniciando vetor da forma {1,2,3,...,n}
    dist = calculaDist(edge,f);
    reset = f;
    
    vector<int> greedy = initialGreedy(numVertices,edge,dist);
    int greedyCost = cutValue(edge,greedy,numVertices); 
    int initialCost = cutValue(edge,f,numVertices);
    if(greedyCost<initialCost) f = greedy;

    globalOptimum = {cutValue(edge,f,numVertices), f};

    int calls = 5;
    
    while(calls--){

        simulatedAnnealing(edge,f,numVertices);
        f = reset;
        shuffle(f.begin(), f.end(), default_random_engine(seed));
    }
    
     // Output the best configuration found    
    // for(int i=0; i<globalOptimum.second.size(); i++) {
    //     if(!i) cout << "{" << i;
    //     else cout << "," << i;
    // }
    cout << line;
    cout << "} maxCut: " << globalOptimum.first << endl;

}

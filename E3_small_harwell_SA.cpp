#include <bits/stdc++.h>
using namespace std;

pair<int, vector<int> > globalOptimum;
// Acha o valor do corte máximo.
int cutValue(const vector <pair<int,int> > &edge, const vector<int>&f, int numVertices) {

    int maxCut = -1;
    vector <int> cutwidth(numVertices - 1, 0);

    // Identificando cortes
    for(int i=0; i<edge.size(); i++) {

        int fa = min(f[edge[i].first], f[edge[i].second]);
        int fb = max(f[edge[i].first], f[edge[i].second]);

        for (int j = 0; j < numVertices; j++) {
            if (fa <= f[j] && f[j] < fb)
                ++cutwidth[f[j] - 1];
        }
    }

    for(int i=0; i<cutwidth.size(); i++) maxCut = cutwidth[i] > maxCut ? cutwidth[i] : maxCut;
    return maxCut;
}

vector <int> nextNeighbor(const vector <pair<int,int> > &edge, vector <int>&f, int numVertices) {

    vector <int> newConfig = f;

    int neighborhoodSize = (int) ceil(0.1*numVertices);
    bool randomHistory[numVertices] = {false};
    
    for(int i=0; i<neighborhoodSize/2; i++){
        
        int a = rand() % (numVertices-1);
        int b = rand() % (numVertices-1);
        
        while(randomHistory[a]) a = rand() % (numVertices-1);
        randomHistory[a] = true;
        while(randomHistory[b]) b = rand() % (numVertices-1);
        randomHistory[b] = true;
        //while(a==b || randomHistory[a]) a = rand() % (numVertices-1);
        //while(b==a || randomHistory[b]) b = rand() % (numVertices-1);
        
        randomHistory[a] = true;
        randomHistory[b] = true;
        
        swap(newConfig[a], newConfig[b]);
    }

    return newConfig;
}

//Sugiro mudar o cabeçalho para ficar coerente com as outras funcoes... edge vem prmeiro, vector dist depois e por ultimo numVertices
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

vector<pair<int,int> > readGraph(int numVertices,int numArestas, vector<int> &f){

  vector<pair<int,int> > dist(numArestas);
  vector<pair<int,int> > edge(numArestas);
  int temp1,temp2;

  for(int i=0;i<numArestas;i++){
    
    cin >> temp1 >> temp2;
    --temp1;
    --temp2;
    
    int distVertices = abs(f[temp1]-f[temp2]);
    
    dist[i] = make_pair(distVertices,i);
    edge[i] = make_pair(temp1,temp2);
  
  }
  
  sort(dist.begin(),dist.end());

  vector<int> temp(f);
  temp = initialGreedy(numVertices,edge,dist);
  //cout << "Guloso: " << cutValue(edge, temp, numVertices) << endl;
  if(cutValue(edge,temp,numVertices) < cutValue(edge,f,numVertices)) f=temp;

  return edge;
}

void simulatedAnnealing(const vector <pair<int, int> > &edge, vector<int> &f, int numVertices) {

	
	vector <int> s = f;
	//double T = initialTemp(parametros);
	double T = 90.0;
	double Tmin = 5.0;
	int factorA = numVertices*T;
	int factorB = numVertices;

	int currBest = cutValue(edge,f,numVertices);

	while(T>Tmin){
	
		int numTrials = 0;
		int changes = 0;
	
		while(changes < factorA && numTrials < factorB) {
			
			s = nextNeighbor(edge, f, numVertices);
			numTrials++;
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
				double R = (rand()%100 + 1)/10.0;
			       
				if(e>R)	{

					changes++;
					currBest = currSol;
					f = s; // Pode trocar diretao assim a melhor conhecida?
				}
			
			}

			//for(int v: f) cout << v << ",";
			//cout << "cut " << currSol;
			//cout << endl;
					
		} // Internal Loop
	
		//T = updateTemp(currBest);
		//cout << "Atualizando temp " << T << endl;
		//cout << "Changes e Trials" << changes << " " << numTrials << endl;
		T = (T*0.6);
		//T *= changes?(changes*changes/10.0):0.1;
	}
}

int main() {

    srand(time(NULL));
    string line;
    getline(cin, line); //Ignorar a primeira linha. Nome do problema.

    int numVertices, numArestas;
    cin >> numVertices >> numVertices; //2a linha: extra + numVertices (ou vice-versa pois esta repetido
    cin >> numArestas;

    vector <int> f(numVertices, 1);
    for (int i=0; i<numVertices; i++) f[i] = i + 1; //Iniciando vetor da forma {1,2,3,...,n}

    vector< pair<int, int> > edge = readGraph(numVertices, numArestas,f);
    globalOptimum = {cutValue(edge,f,numVertices), f};

    //cout << "Ordem normal ou Guloso: " <<  cutValue(edge, f, numVertices) << endl;

    simulatedAnnealing(edge,f,numVertices);
    //cout << "Rodei o simulated" << endl;
    
    //for(int i: globalOptimum.second) cout << i << ",";
    cout << globalOptimum.first << endl;
}

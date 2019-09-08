#include <bits/stdc++.h>

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



vector<int> initialGreedy(int numVertices, const vector<pair<int,int> > &edge){
    vector<int> novaConfig(numVertices);
    vector<pair<int,int> > qtdLigacoes(numVertices);
    for(int i=0;i<numVertices;i++){
    	qtdLigacoes[i].second=i;
    }
    for(int i=0;i<edge.size();i++){
    	qtdLigacoes[edge[i].first].first++;
    	qtdLigacoes[edge[i].second].first++;

    }
    sort(qtdLigacoes.begin(),qtdLigacoes.end());
    int preenchidos=0;
    int posiFrente=0;
    int posiFinal=numVertices-1;
    bool preencherFrente=true;
    while(preenchidos<numVertices){
    	if(preencherFrente){
    		novaConfig[qtdLigacoes[preenchidos].second]=posiFrente+1;
    		preencherFrente=false;
    		posiFrente++;
    	}else{
    		novaConfig[qtdLigacoes[preenchidos].second]=posiFinal+1;
    		posiFinal--;
    		preencherFrente=true;
    	}
    	++preenchidos;
    }
    return novaConfig;
}

vector<pair<int,int> > readGraph(int numVertices,int numArestas){

  vector<pair<int,int> > dist(numArestas);
  vector<pair<int,int> > edge(numArestas);
  int temp1,temp2;

  for(int i=0;i<numArestas;i++){
    cin >> temp1 >> temp2;
    //Subtraindo para forma de 0..n
    --temp1;
    --temp2;
    edge[i]=make_pair(temp1,temp2);
  }
  return edge;
}


int main() {

    srand(time(NULL));
    string line;
    getline(cin, line); //Ignorar a primeira linha. Nome do problema.

    int numVertices, numArestas;
    cin >> numVertices >> numVertices; //2a linha: extra + numVertices (ou vice-versa pois esta repetido
    cin >> numArestas;
    
    vector < int > f(numVertices, 1);
    for (int i = 0; i < numVertices; i++) f[i] = i + 1; //Iniciando vetor da forma {1,2,3,...,n}

    vector< pair<int, int> > edge = readGraph(numVertices, numArestas);
	// f=initialGreedy(numVertices,edge);
	
    cout << cutValue(edge, f, numVertices) << endl;

    // //Declaracao do vetor f (1,2,...n) -> especificado no artigo

    // int trocas = 10000000;
    // while (trocas--) {

    //     f = localSearch(edge, f, numVertices);

    // }

    // //cout << "{ ";
    // //for(int i:f) cout << i << ",";
    // //cout << "}" << endl;
    // cout << cutValue(edge, f, numVertices) << endl;
}
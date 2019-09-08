#include <bits/stdc++.h>

using namespace std;

// Acha o valor do corte máximo.
int cutValue(const vector <pair<int,int> > &edge,
    const vector<int>&f, int numVertices) {

    int maxCut = -1;
    vector <int> cutwidth(numVertices, 0);

    // Identificando cortes
    for (int i = 0; i < edge.size(); i++) {

        int fa = min(f[edge[i].first], f[edge[i].second]);
        int fb = max(f[edge[i].first], f[edge[i].second]);

        for (int j = 0; j < numVertices; j++) {
            if (fa <= f[j] && f[j] < fb)
                ++cutwidth[f[j]];
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

vector<pair<int,int> > calculaDist(const vector<pair<int,int> > &edge, vector<int> &f){
    vector<pair<int,int> > dist(edge.size());
    for(int i=0;i<edge.size();i++){
        int distVertices=abs(f[edge[i].first]-f[edge[i].second]);
        dist[i]=(make_pair(distVertices,i));
    }
    sort(dist.begin(),dist.end(),greater<pair<int,int> >());
    return dist;
}


vector<int> initialGreedy(int numVertices, const vector<pair<int,int> > &edge, vector<int> &f){
    vector<int> novaConfig(numVertices,0);
    vector<pair<int,int> > dist = calculaDist(edge,f);
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
        edge.push_back(make_pair(contadorLinha,i/2));
      }
    }
    ++contadorLinha;
  }
  return edge;
}

int main() {

    srand(time(NULL));
    string line;
    getline(cin, line); //Ignorar a primeira linha. Nome do problema.


    int numVertices=0;
    vector< pair<int, int> > edge = readGrid(numVertices);
    vector<int> f(numVertices,0);
    for(int i=0;i<numVertices;i++) f[i]=i+1;

    vector<int> iniConfig;

    iniConfig=initialGreedy(numVertices,edge,f);
    cout << cutValue(edge, iniConfig, numVertices) << endl;

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
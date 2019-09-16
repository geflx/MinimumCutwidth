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
    // cout << cutValue(edge, iniConfig, numVertices) << endl;

    // //Declaracao do vetor f (1,2,...n) -> especificado no artigo

    int trocas = 1000000;
    int trocasInicial= trocas;
    int currCutValue=cutValue(edge,iniConfig,numVertices);

    int naoMelhora=0;
    while (trocas--){
        if(naoMelhora>ceil(0.1*trocasInicial)){
            break;
        }
        vector<int> tempConfig= nextNeighbor(edge, iniConfig, numVertices);
        int neighborCutValue=cutValue(edge,tempConfig,numVertices);
        if(neighborCutValue<currCutValue){
            iniConfig=tempConfig;
            currCutValue=neighborCutValue;
            naoMelhora=0;
        }else{
            ++naoMelhora;
        }
    }
    cout << cutValue(edge, iniConfig,numVertices) << endl;
}

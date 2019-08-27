#include <bits/stdc++.h>
using namespace std;

// Acha o valor do corte máximo. Retorna o corte máximo e a largura de corte de uma dada ordem
pair<int,int> cutValue(const vector<pair<int, int> > &edge,const vector<int> &f, int numVertices){
	
  	int cutwidthSum = 0;
    int maxCut = -1;
	vector<int> cutwidth(numVertices,0);
	
    // Identificando cortes
  	for(int i=0;i<edge.size();i++){
	    for(int j=0;j<numVertices;j++){
            
		    int a = edge[i].first;
			int b = edge[i].second;
            
            //Condicao para calculo da largura do corte, especificada no artigo.
			if(f[a]<=f[j] && f[j]<f[b]){ 
                ++cutwidth[j];
                ++cutwidthSum;
			}
		}
	}
  
  for(int i=0; i<cutwidth.size(); i++) maxCut = cutwidth[i]>maxCut?cutwidth[i]:maxCut;
  return make_pair(maxCut,cutwidthSum);  
}


vector<int> localSearch(vector<pair<int,int> > &edge, vector<int> &f, int numVertices) {
	
      vector<int> currConfig = f;
  	  vector<int> leastCutSum = f; //at the moment
    
  	  pair <int,int> currSolution = cutValue(edge,f,numVertices);
  	  int currSum = currSolution.second;
      int currCut = currSolution.first;

      
      for(int i=0;i<f.size()-1;i++){
        vector<int> newConfig=f;
        //vector<pair<int,int> > newEdge=edge;
        swap(newConfig[i], newConfig[i+1]); 

        pair<int,int> newResults= cutValue(edge,newConfig,numVertices);
        int newCut= newResults.first; //Captura o novo corte minimo
        int newSum= newResults.second; //Captura o somatorio dos cortes

        if(newCut<currCut){
          return newConfig; //Copiando o novo vetor. OTIMIZAVEL. Daria pra trocar so os dois swapados
        }
        if(newSum < currSum) {
          leastCutSum = newConfig;
          currSum = newSum;
        }
    }
  
  	//Zona de empate
   return leastCutSum;
}

vector<pair<int,int> > readGraph(int numVertices,int numArestas){
  vector<pair<int,int> > edge(numArestas);
  int temp1,temp2;
  for(int i=0;i<numArestas;i++){
    cin >> temp1 >> temp2;
    //Subtraindo para forma de 0..n
    --temp1;
    --temp2; 
    edge.push_back(make_pair(temp2,temp1)); // Acho que aqui deveriamos so fazer o for ao contrário mesmo...
    edge[i]=make_pair(temp1,temp2);
  }
  return edge;
}

 
int main(){
    
	string line;
	getline(cin,line); //Ignorar a primeira linha. Nome do problema.

	int numVertices, numArestas;
  	cin>>numVertices>>numVertices; //2a linha: extra + numVertices (ou vice-versa pois esta repetido
	cin>>numArestas;
    
  	vector< pair<int, int> > edge = readGraph(numVertices, numArestas);
	
	//Declaracao do vetor f (1,2,...n) -> especificado no artigo
	vector<int> f(numVertices,1);
	for(int i=0;i<numVertices;i++) f[i]=i+1; //Iniciando vetor da forma {1,2,3,...,n}

	int trocas = 100;
	while(trocas--){
			
      f=localSearch(edge,f,numVertices);

	}
    cout<<cutValue(edge,f,numVertices).first<<endl;
}
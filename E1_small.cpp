#include <bits/stdc++.h>
using namespace std;

pair<int,int> cutValue(const vector<pair<int, int>> &edge,const vector<int> &f, int numVertices){
  int numArestas=edge.size();
  int cutwidthSum=0;
    // Identificando cortes
	vector<int> cutwidth(numVertices,0);
  	for(int i=0;i<numArestas;i++){
			for(int j=0;j<numVertices;j++){
              	//Condicao para calculo da largura do corte, especificada no artigo.
				if(f[edge[i].first]<=f[j] && f[j]<f[edge[i].second]){ 
					++cutwidth[j];
          ++cutwidthSum;
				}
			}
		}
  
  // Pegando maior corte da solução atual
  int maxCut = -1;
  for(int i=0; i<cutwidth.size(); i++)
    maxCut = cutwidth[i]>maxCut?cutwidth[i]:maxCut;
  return make_pair(maxCut,cutwidthSum);  
}


vector<int> bestChoice(vector<pair<int,int> > &edge, vector<int> &f, int numVertices) {
	
      vector<int> currConfig = f;
  	  vector<int> leastCutSum = f; //at the moment
      
  	  pair <int,int> currSolution = cutValue(edge,f,numVertices);
      int currCut = currSolution.first;
  	  int currSum = currSolution.second;

      for(int i=0;i<f.size()-1;i++){
        vector<int> newConfig=f;
        vector<pair<int,int> > newEdge=edge;
        swap(newConfig[i], newConfig[i+1]); 
        // cout<<"newConfig eh:    ";
        // for(int j=0;j<newConfig.size();j++){
        //   cout<<newConfig[j]<<" ";
        // }cout<<endl;
        //Invertendo as arestas que sao do tipo i ou i+1 (que acabamos de inverter) 
        // for(int j=0;j<newEdge.size();j++){ // aqui!
        //   if(newEdge[j].first==i || newEdge[j].second==i || newEdge[j].first==(i+1) ||newEdge[j].second==(i+1)){
        //       swap(newEdge[j].first,newEdge[j].second);
        //   }
        // }
        pair<int,int> newResults= cutValue(newEdge,newConfig,numVertices);
        int newCut= newResults.first; //Captura o novo corte minimo
        int newSum= newResults.second; //Captura o somatorio dos cortes

        // cout<<"currSum eh:";
        // cout<<newSum<<endl;
        // cout<<"newCut: "<<newCut<<endl;
        // cout<<"currCut: "<<currCut<<endl;
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

vector<pair<int,int>> readGraph(int numVertices,int numArestas){
  vector<pair<int,int> > edge(numArestas);
  int temp1,temp2;
  for(int i=0;i<numArestas;i++){
    cin>>temp1>>temp2;
    --temp1;--temp2; //Subtraindo para forma de 0..n
  	// if(temp1>temp2)
   //    swap(temp1,temp2);
    edge.push_back(make_pair(temp2,temp1));
    edge[i]=make_pair(temp1,temp2);
  }
  return edge;
}

 
int main(){
	string line;
	getline(cin,line); //Ignorar a primeira linha. Nome do problema.

	int numVertices,extra;
  	cin>>numVertices>>extra; //2a linha: extra + numVertices (ou vice-versa pois esta repetido
	int numArestas;          // + numero de Arestas
	cin>>numArestas;
  	vector< pair<int, int>> edge = readGraph(numVertices, numArestas);
	
	//Declaracao do vetor f (1,2,...n) -> especificado no artigo
	vector<int> f(numVertices,1);
	for(int i=0;i<numVertices;i++) f[i]=i+1; //Iniciando vetor da forma {1,2,3,...,n}
    
  // cout<<"No antigo: "<<cutValue(edge,f,numVertices).first<<endl;      
	int trocas = 10;
	while(trocas--){
			
      f=bestChoice(edge,f,numVertices);

      // for(int i=0; i<f.size(); i++) cout << f[i] << " ";
      // cout << endl;
      // cout << cutValue(edge,f,numVertices).first << " " << cutValue(edge,f,numVertices).second << endl;     
      // cout<<"SELECIONEI! ACIMA.;"
	}
     //  cout<<"f: ";
     //  for(int i=0; i<f.size(); i++) cout << f[i] << " ";
     //  cout << endl;
     //  cout <<"cutValue: "<< cutValue(edge,f,numVertices).first << endl;
	    // cout<<"sumCut: "<<cutValue(edge,f,numVertices).second<<endl;
    cout<<cutValue(edge,f,numVertices).first<<endl;
}
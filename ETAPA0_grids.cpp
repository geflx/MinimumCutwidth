#include <bits/stdc++.h>
using namespace std;

int main(){
	string line;
	getline(cin,line); //Ignorar a primeira linha. Nome do problema.
	int contadorLinha=0;
	int numColunas=0;
	vector<pair<int,int> > edges;

	/*Lendo a matriz de adjacencia e transformando numa Lista de Adjacencia!

	*/
	while(getline(cin,line)){
		if(contadorLinha==0){
			numColunas=line.size()/2;
		}
		for(int i=0;i<line.size();i++){
			if(line[i]=='1'){
				edges.push_back(make_pair(contadorLinha,i/2));
			}
		}
		++contadorLinha;
	}

	//Declaracao do vetor f (1,2,...n) -> especificado no artigo
	vector<int> f(numColunas,1);
	for(int i=1;i<numColunas;i++) 
		f[i]=f[i-1]+1;

	//Declaracao corte minimo
	vector<int> cutwidth(numColunas,0);

	//Contando largura do corte a partir da lista de adjacencia
	for(int k=0;k<numColunas;k++){
		for(int i=0;i<edges.size();i++){
			if(f[edges[i].first]<=f[k] && f[k]<f[edges[i].second]){
					++cutwidth[k];
				
			}
		}
	}
	int maximumCut=-1; //Verificando qual e' o corte maximo!
	for(int j=0;j<numColunas;j++){
		if(cutwidth[j]>maximumCut)
			maximumCut=cutwidth[j];
	}

	cout<<maximumCut<<endl; //Imprime!

	return 0;
}
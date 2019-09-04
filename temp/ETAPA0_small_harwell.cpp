#include <bits/stdc++.h>
using namespace std;
int main(){
	string line;
	getline(cin,line); //Ignorar a primeira linha. Nome do problema.

	int numVertices,extra;
	cin>>numVertices>>extra; //2a linha: extra + numVertices (ou vice-versa pois esta repetido
	int numArestas;          // + numero de Arestas
	cin>>numArestas;

	//Declaracao do vetor f (1,2,...n) -> especificado no artigo
	vector<int> f(numVertices,1);
	for(int i=1;i<numVertices;i++) 
		f[i]=f[i-1]+1;

	//Declaracao corte minimo
	vector<int> cutwidth(numVertices,0);

	for(int i=0;i<numArestas;i++){
		//Lendo a aresta
		int in1,in2;
		cin>>in1>>in2;
		/*Ordenando para que fique da forma: verticeI --->(liga) verticeJ, com i<j
		Isto serve para que ordenemos o arquivo de entrada, pois esta' desorganizado. */
		if(in1>in2){
			int temp=in1;
			in1=in2;
			in2=temp;

		}
		for(int j=0;j<numVertices;j++){
			if(f[in1-1]<=f[j] && f[j]<f[in2-1]){ //Condicao para calculo da largura do corte, especificada no artigo.
				++cutwidth[j];
			}
		}
	}
	int maximumCut=-1; //Verificando qual e' o corte maximo!
	for(int i=0;i<numVertices;i++){
		if(cutwidth[i]>maximumCut)
			maximumCut=cutwidth[i];
	}
	cout<<maximumCut<<endl; //Imprime!
}

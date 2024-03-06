#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;


//o certo eh passar por const, mas estamos na maratona :)
void print(vector<int> &v) {
	for(int i= 0;i<v.size();i++) 
		cout << v[i] << " ";
	cout << "\n";
}



struct S {
	int x;
	string nome;
};

int main() {
	pair<int,string> p;
	p.first = 4;
	p.second = "abc";
	vector<pair<int,string> > v;
	v.push_back({5,"abc"});





	//criar iterador

	//end, begin

	//avancar, voltar, avancar n unidades, ...

	//subtrair iteradores (apenas de acesso aleatorio!)

	//comparar iteradores (apenas de acesso aleatorio!)

	//const iterator

	//iteradores sao tipo "referencia" 
	//--> qual a diferenca entre it++ e (*it)++ ?

	//reverse...
	/*
	vector<int>::iterator itr = v.end();
	itr--;
	while(itr >= v.begin()) {
		cout << *itr << " ";
		itr--;
	}*/

}


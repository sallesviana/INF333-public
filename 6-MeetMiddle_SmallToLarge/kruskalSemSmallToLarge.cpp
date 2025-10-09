#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;


class DisjointSets {
  private:
	vector<int> cores; //no inicio, cada vertice tera uma cor. Vertices do mesmo CC sao pintados da mesma cor
	vector<int> sizeCor; //tamanho do CC de cada cor..
	vector<vector<int> > adj;

  public:
	DisjointSets(int size) : cores(size), adj(size),sizeCor(size,1) {
		for (int i = 0; i < size; i++) { cores[i] = i; }
	}

	int find(int x) { return cores[x]; }

	bool unite(int x, int y) { //une os dois sets (retorna false se ja estavam unidos...)
		if(connected(x,y)) return false;

		//Exercicio: adapte aqui para que a estrutura funcione com "small to large merging"


		//sizeCor indica quantos vertices estao atualmente marcados com cada cor. Usaremos isso no small to large
		sizeCor[cores[y]] = sizeCor[cores[y]] + sizeCor[cores[x]]; //a cor de Y será usada para marcar todo o CC de x --> atualiza o numero de elementos com essa cor!

		marca(x, cores[y]); // pinta o CC do vertice x com a cor do cc de y

		//conecta o CC de X com o de y nos conjuntos
		adj[x].push_back(y);
		adj[y].push_back(x);		

		return true;
	}

	void marca(int v, int cor) {
		if(cores[v]==cor) return;
		cores[v] = cor;
		for(int w:adj[v]) marca(w,cor);
	}

	bool connected(int x, int y) { return find(x) == find(y); }
};


//parte principal do codigo compiado do USACO: https://usaco.guide/gold/mst?lang=cpp
//solucao para o problema: https://cses.fi/problemset/task/1675
int main() {
	int city_num;
	int road_num;
	std::cin >> city_num >> road_num;

	struct Road {
		int c1, c2;
		int cost;
	};
	vector<Road> roads(road_num);
	for (Road &r : roads) {
		std::cin >> r.c1 >> r.c2 >> r.cost;
		r.c1--;
		r.c2--;
	}
	std::sort(roads.begin(), roads.end(),
	          [&](const Road &e1, const Road &e2) { return e1.cost < e2.cost; });

	DisjointSets cities(city_num);
	long long min_cost = 0;
	int added = 0;
	for (Road &r : roads) {
		bool status = cities.unite(r.c1, r.c2);
		min_cost += status * r.cost;
		added += status;
	}

	if (added != city_num - 1) {
		cout << "IMPOSSIBLE" << endl;
	} else {
		cout << min_cost << endl;
	}
}
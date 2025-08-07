#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include <bits/stdc++.h>
using namespace std;

//Fonte: código adaptado de: https://usaco.guide/plat/sqrt?lang=cpp

//Complexidade será O( N sqrt N ) * complexidade de add,remove,valorConsulta
//adapte essa classe!
class JanelaMo {
public:
	JanelaMo(vector<int> &v_): v(v_) { //tamanho do array
		soma = 0;
		n = 0;
	}
	double valorConsulta() { //adaptar: retorna o valor da consulta da janela atual
		return ((double)soma)/n;
	}
	void add(int idx) { //adaptar:adiciona o elemento da posicao idx a janela		
		soma += v[idx];  
		n++;
	}
	void remove(int idx) {//adaptar:remove o elemento da posicao idx da janela
		soma -= v[idx];
		n--;
	}

private:
	vector<int> &v; //array de entrada (estático!) . Adaptar para outros tipos...
	int soma;
	int n;
};

struct Query {
	int l, r, idx;
};

int main() {
	int n;
	cin >> n;
	vector<int> v(n);
	for (int i = 0; i < n; i++) { cin >> v[i]; }

	int q;
	cin >> q;
	vector<Query> queries;
	for (int i = 0; i < q; i++) {
		int x, y;
		cin >> x >> y;
		queries.push_back({--x, --y, i});
	}

	int block_size = (int)sqrt(n);
	auto mo_cmp = [&](Query a, Query b) {
		int block_a = a.l / block_size;
		int block_b = b.l / block_size;
		if (block_a == block_b) { return a.r < b.r; }
		return block_a < block_b;
	};
	sort(queries.begin(), queries.end(), mo_cmp);

	
	JanelaMo janela(v);

	int mo_left = -1;
	int mo_right = -1;
	vector<double> ans(q); //respostas das consultas (adaptar)
	for (int i = 0; i < q; i++) {
		int left = queries[i].l;
		int right = queries[i].r;

		if(mo_left==-1) { //primeira consulta...
			for(int i=left;i<=right;i++) janela.add(i);
			mo_left = left;
			mo_right = right;
		} else {
			while (mo_left < left) { janela.remove(mo_left++); } 
			while (mo_left > left) { janela.add(--mo_left); }
			while (mo_right < right) { janela.add(++mo_right); }
			while (mo_right > right) { janela.remove(mo_right--); }
		}

		ans[queries[i].idx] =  janela.valorConsulta()  ;   
	}

	for (int i = 0; i < q; i++) { cout << ans[i] << '\n'; }
}
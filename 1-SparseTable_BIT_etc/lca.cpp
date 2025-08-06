#include <bits/stdc++.h>
using namespace std;

template <typename T> class SparseTable {
  private:
	int n, log2dist;
	vector<vector<T>> st;

  public:
	SparseTable(const vector<T> &v) {
		n = (int)v.size();
		log2dist = 1 + (int)log2(n);
		st.resize(log2dist);
		st[0] = v;
		for (int i = 1; i < log2dist; i++) {
			st[i].resize(n - (1 << i) + 1);
			for (int j = 0; j + (1 << i) <= n; j++) {
				st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
			}
		}
	}

	/** @return minimum on the range [l, r] */
	T query(int l, int r) {
		int i = (int)log2(r - l + 1);
		return min(st[i][l], st[i][r - (1 << i) + 1]);
	}
};


//LCA usando euler tour
//fonte: https://usaco.guide/gold/tree-euler?lang=cpp
//assume que 0 é a raiz...
class LCA {
  private:
	const int n;
	const vector<vector<int>> &adj;
	SparseTable<pair<int, int>> rmq;
	vector<int> tin, et, dep;
	int timer = 0;

	/** prepares tin, et, dep arrays */
	void dfs(int u, int p) {
		tin[u] = timer; //tempo de entrada do vertice
		et[timer++] = u; //adiciona os vertices na ordem do euler tour
		for (int v : adj[u]) {
			if (v == p) { continue; }
			dep[v] = dep[u] + 1;
			dfs(v, u);
			et[timer++] = u;
		}
	}

  public:
	// make sure the adjacency list is 0 indexed
	LCA(vector<vector<int>> &_adj)
	    : n((int)_adj.size()), adj(_adj), tin(n), et(2 * n), dep(n),
	      rmq(vector<pair<int, int>>(1)) {
		dfs(0, -1);
		vector<pair<int, int>> arr(2 * n);
		for (int i = 0; i < 2 * n; i++) { arr[i] = {dep[et[i]], et[i]}; }
		rmq = SparseTable<pair<int, int>>(arr);
	}

	/** @return LCA of nodes a and b */
	int query(int a, int b) {
		if (tin[a] > tin[b]) { swap(a, b); }
		return rmq.query(tin[a], tin[b]).second;
	}

	void debug() {
		for(int i=0;i<n;i++) cout << i << " "; cout << endl;
		for(int i=0;i<n;i++) cout << tin[i] << " "; cout << endl;
		for(int i=0;i<n;i++) cout << dep[i] << " "; cout << endl;

		cout << endl;
		for(int i=0;i<et.size();i++) cout << i << " "; cout << endl;
		for(int i=0;i<et.size();i++) cout << dep[et[i]] << " "; cout << endl;
		for(int i=0;i<et.size();i++) cout << et[i] << " "; cout << endl;

	}
};

/*
Arvore usada na aula:
9
-1 5 6 0 0 0 3 6 1*/


int main() {
	int n;
	cin >> n;

	vector<vector<int> > adj(n);
	for(int i=0;i<n;i++) { //vetor de pais...
		int p;
		cin >> p;
		if(p==-1) continue;
		//p--; //contar a partir de 0
		//pai do i é o p
		adj[i].push_back(p);
		adj[p].push_back(i);
	}
	LCA e(adj);
	e.debug(); //para entender a estrutura...
}
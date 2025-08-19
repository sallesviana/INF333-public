
//Fonte: https://medium.com/carpanese/an-illustrated-introduction-to-centroid-decomposition-8c1989d53308
//Ha apenas log(n) niveis no CD --> complexidade de construir fica O(n log n)
struct CentroidDecomposition {
	vector<set<int>> adj; // it's not vector<vector<int>>!
	vector<int> dad;
	vector<int> sub;

	CentroidDecomposition(vector<set<int>> &adj) : adj(adj) {
		int n = adj.size();
		dad.resize(n);
		sub.resize(n);
		build(0, -1);
	}

	void build(int u, int p) {
		int n = dfs(u, p); // find the size of each subtree
		int centroid = dfs(u, p, n); // find the centroid
		if (p == -1) p = centroid; // dad of root is the root itself
		dad[centroid] = p;

		vector<int> neighbors(adj[centroid].begin(), adj[centroid].end());
		
		// for each tree resulting from the removal of the centroid
		for (auto v : adj[centroid]) {
			adj[v].erase(centroid);
			build(v, centroid);
		}
		adj[centroid].clear();
	}

	int dfs(int u, int p) {
		sub[u] = 1;

		for (auto v : adj[u])
			if (v != p) sub[u] += dfs(v, u);

		return sub[u];
	}

	int dfs(int u, int p, int n) {
		for (auto v : adj[u])
			if (v != p and sub[v] > n/2) return dfs(v, u, n);

		return u;
	}

	int operator[](int i) {
		return dad[i];
	}
};
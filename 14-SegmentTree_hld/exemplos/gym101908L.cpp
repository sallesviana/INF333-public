#include <iostream>
#include <vector>
using namespace std;

//Fontes: https://cp-algorithms.com/graph/hld.html
//https://github.com/brunomaletta/Biblioteca/blob/master/Codigo/Grafos/LCA-HLD/hldVertice.cpp

//exemplo: soma dos valores em um caminho
//ATUALIZAR: usa uma segtree que suporta soma de intervalos e updates que trocam valores...

//para colocar valor nas arestas em vez de nos vertices --> muda pouca coisa 
//basicamente o valor de cada vertice (na solucao abaixo) representaria o valor da aresta chegando nele (na ordem da DFS)
//assim, if(head[u]==head[v]) return st.query(pos[v], pos[u]);  (POR EXEMPLO )
//viraria: if(head[u]==head[v]) return st.query(pos[v]+1, pos[u]); (pois no caminho de v até u nao queremos somar o valor de v (que representa a aresta chegando em v...))

//segtree que permite somar em intervalos
//adaptado daqui: https://cp-algorithms.com/data_structures/segment_tree.html#addition-on-segments
typedef int T;
class SegTree {
public:
	SegTree(int n) {
		mxPos = n-1;
		t.resize(4*n,0);
		lazy.resize(4*n,0);
	}

	void build(vector<int> &a, int v, int tl, int tr) {
	    if (tl == tr) {
	        t[v] = a[tl];
	    } else {
	        int tm = (tl + tr) / 2;
	        build(a, v*2, tl, tm);
	        build(a, v*2+1, tm+1, tr);
	        t[v] = (t[v*2] + t[v*2 + 1]);
	    }
	}

	void build(vector<int> &a) {
		build(a,1,0,mxPos);
	}


	void push(int v, int tl, int tr) {
	    if (lazy[v] != 0) {
	        int tm = (tl + tr) / 2;
	        t[v*2]     += lazy[v] * (tm - tl + 1);
	        lazy[v*2]  += lazy[v];
	        t[v*2+1]   += lazy[v] * (tr - tm);
	        lazy[v*2+1] += lazy[v];
	        lazy[v] = 0;
	    }
	}

	void update(int v, int tl, int tr, int l, int r, int addend) {
	    if (l > r) return;
	    if (l == tl && tr == r) {
	        t[v] += addend * (tr - tl + 1);
	        lazy[v] += addend;
	    } else {
	        push(v, tl, tr);
	        int tm = (tl + tr) / 2;
	        update(v*2, tl, tm, l, min(r, tm), addend);
	        update(v*2+1, tm+1, tr, max(l, tm+1), r, addend);
	        t[v] = t[v*2] + t[v*2+1];
	    }
	}


	void update(int l, int r, T add) {
		update(1,0,mxPos,l,r,add);
	}

	int query(int v, int tl, int tr, int l, int r) {
	    if (l > r) return 0;
	    if (l == tl && r == tr) return t[v];
	    push(v, tl, tr);
	    int tm = (tl + tr) / 2;
	    return query(v*2, tl, tm, l, min(r, tm)) +  
	           query(v*2+1, tm+1, tr, max(l, tm+1), r);
	}



	T query(int l, int r) {
		return query(1,0,mxPos,l,r);
	}
		
	
private:
	vector<T> t, lazy;
	int mxPos;
};


//podemos trabalhar com int, long long ,etc.
template<class T>
class Hld {
public:
	//lista de adjacencia (nao direcionada), valor em cada vertice, qual será a raiz da árvore
	//
	Hld(vector<vector<int> > &adj, vector<T> &value_, int root=0): st(adj.size()) {

		ct = 0; //marca os vertices com codigo 0,1,2 ... com base na posicao em que vao ficar na seg tree
		int n = adj.size();
		pos = sz = parent = head = depth = vector<int>(n);
		value = value_;

		vector<int> v(n); //vetor com pesos dos vertices na seg tree..
		depth[root] = 0; //vamos considerar que a raiz esta na profundidade 0 (opcional)
		dfs(adj,root,-1);
		build(adj,v,ct,root,-1); //segunda DFS, após colocar as arestas pesadas como (u, adj[u][0]) 
		st.build(v); //cria a Seg Tree
	}
	//as consultas nas chains sao sempre de cima para baixo
	//note que se o pai de head de v estiver no meio da chain de u --> nao gera problema
	//pois o pos de u será menor que v e trocariamos os dois..
	//ou seja, sempre teremos  pos[u] >= pos[v]
	//assim, a consulta é sempre do pos maior para o menor...
	T query(int u, int v) { //consulta nos nodos do caminho entre vertices u e v
		if(pos[u] < pos[v]) swap(u,v); //posicoes sao de cima para baixo...
		if(head[u]==head[v]) return st.query(pos[v], pos[u]); //estao na mesma chain

		return  st.query(pos[head[u]],pos[u]) + query(parent[head[u]], v) ; //ATUALIZAR (exemplo: minimo de caminho, maximo de caminho, etc)
	}
 	void updatePath(int u, int v, T valor) {
		if(pos[u] < pos[v]) swap(u,v); //posicoes sao de cima para baixo...
		if(head[u]==head[v]) st.update(pos[v], pos[u], valor); //estao na mesma chain
		else {
		  st.update(pos[head[u]],pos[u], valor); 
		  updatePath(parent[head[u]], v,valor) ; 
		}	
 	}
 	//!extra!

 	//Na dfs, ao processar o nodo u os (sz[u]-1) proximos elementos todos de sua subarvore estarao logo apos ele no vetor v de valores da seg tree
 	T querySubtree(int u) { //consulta na subarvore inteira do nodo u
 		return st.query(pos[u], pos[u] + sz[u] - 1);
 	}
 	void updateSubtree(int u, T valor) {
 		st.update(pos[u], pos[u] + sz[u] - 1,valor);
 	}
 	int lca(int u, int v) {
 		if(pos[u] < pos[v]) swap(u,v);

 		if(head[u]==head[v]) //estao na mesma chain...
 			return v;
 		return lca(parent[head[u]], v); 
 	}

private:
	void dfs(vector<vector<int> > &adj,int root, int prev) {
		sz[root] = 1;
		for(int &w:adj[root]) if(w!=prev) {
			depth[w] = depth[root] + 1; //opcional
			dfs(adj,w,root);
			sz[root] += sz[w];
			if(sz[w] > sz[ adj[root][0] ] || adj[root][0]==prev) 
				swap(adj[root][0],w); //o primeiro filho de cada vértice será sempre o maior (ou seja, terá um heavy edge entre eles)
		}
	}
	void build(vector<vector<int> > &adj,vector<T> &v, int &ct, int root, int prev)  {
		pos[root] = ct;
		v[ct] = value[root];
		ct++;
		for(int &w:adj[root]) if(w!=prev) { //vizinhos de root (cuidado para nao voltar)
			parent[w] = root;
			head[w] = (w==adj[root][0]?head[root]:w); //cada vertice é cabeca da sua chain. Depois arrumamos isso para os vertices que estiverem no heavy path
			build(adj,v,ct,w,root);			
		}

	}

	vector<T> value; //valor armazenado em cada vertice

	int ct;

	vector<int> pos; //posicao do vertice na seg tree 
	vector<int> sz; //sz[v] é o tamanho da subarvore com raiz em v
	vector<int> parent; //pai de cada vertice
	vector<int> head; //head (começo) de cada chain
	vector<int> depth; //nivel de cada nodo (OPCIONAL, mas pode ser util...)

	SegTree st;
};


//Time limit muito baixo!!! so passou com scanf
int main() {
	int n, q;
	cin >> n >> q;
	vector<vector<int> > adj(n);
	vector<int> v(n,0);

	for(int i=0;i<n-1;i++) {
		int a,b;
		//cin >> a >> b;
		scanf("%d %d",&a,&b);
		a--; b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	Hld<int> hld(adj, v,0);
	for(int i=0;i<q;i++) {
		int a,b,c,d;
		scanf("%d %d %d %d",&a,&b,&c,&d);
		a--; b--; c--; d--;
		hld.updatePath(a,b,1);
		printf("%d\n",hld.query(c,d));
		hld.updatePath(a,b,-1);
	}
}
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


//podemos trabalhar com int, long long ,etc.

typedef int T

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
			build(v,ct,w,root);			
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
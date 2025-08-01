#include <iostream>
#include <vector>
#include "segTreeSumInterval.cpp"
using namespace std;

//Fontes: https://cp-algorithms.com/graph/hld.html
//https://github.com/brunomaletta/Biblioteca/blob/master/Codigo/Grafos/LCA-HLD/hldVertice.cpp

//exemplo: soma dos valores em um caminho
//ATUALIZAR: usa uma segtree que suporta soma de intervalos e updates que trocam valores...

//para colocar valor nas arestas em vez de nos vertices --> muda pouca coisa 
//basicamente o valor de cada vertice (na solucao abaixo) representaria o valor da aresta chegando nele (na ordem da DFS)
//assim, if(head[u]==head[v]) return st.query(pos[v], pos[u]);  (POR EXEMPLO )
//viraria: if(head[u]==head[v]) return st.query(pos[v]+1, pos[u]); (pois no caminho de v até u nao queremos somar o valor de v (que representa a aresta chegando em v...))

//Permite atualizar e consultar a subárvore de um vértice
//Permite calcular LCA

//Construcao: O(n)
//Query: O(log^2 n), Query subtree: O(log n)
//Update: O(log^2 n), Update subtree: O(log n)

//podemos trabalhar com int, long long ,etc.

//typedef int T;

class Hld {
public:
	//lista de adjacencia (nao direcionada)
	//adj[a] --> arestas incidentes em a (pares do tipo vertice, peso)
	Hld(vector<vector<pair<int,int> > > &adj, int root=0): st(adj.size()) {

		ct = 0; //marca os vertices com codigo 0,1,2 ... com base na posicao em que vao ficar na seg tree
		int n = adj.size();
		pos = sz = parent = head = depth = vector<int>(n);

		vector<T> v(n); //vetor com pesos dos vertices na seg tree..
		head[root] = root;
		depth[root] = 0; //vamos considerar que a raiz esta na profundidade 0 (opcional)
		dfs(adj,root,-1);
		build(adj,v,ct,root,-1,-1); //segunda DFS, após colocar as arestas pesadas como (u, adj[u][0]) 
		st.build(v); //cria a Seg Tree
	}
	//as consultas nas chains sao sempre de cima para baixo
	//note que se o pai de head de v estiver no meio da chain de u --> nao gera problema
	//pois o pos de u será menor que v e trocariamos os dois..
	//ou seja, sempre teremos  pos[u] >= pos[v]
	//assim, a consulta é sempre do pos maior para o menor...
	T query(int u, int v) { //consulta nos nodos do caminho entre vertices u e v
		if(pos[u] < pos[v]) swap(u,v); //posicoes sao de cima para baixo...
		if(head[u]==head[v]) return st.query(pos[v]+1, pos[u]); //estao na mesma chain

		//ATUALIZAR (exemplo: minimo de caminho, maximo de caminho, etc)
		//implementacao atual: soma
		return  st.query(pos[head[u]],pos[u]) + query(parent[head[u]], v) ; 
	}
	//soma valor ao caminho..
 	void updatePath(int u, int v, T valor) {
 		if(u==v) return; //se o caminho tem apenas 1 vertice --> nao ha aresta para atualizar...
		if(pos[u] < pos[v]) swap(u,v); //posicoes sao de cima para baixo...
		if(head[u]==head[v]) st.update(pos[v]+1, pos[u], valor); //estao na mesma chain
		else {
		  st.update(pos[head[u]],pos[u], valor); 
		  updatePath(parent[head[u]], v,valor) ; 
		}	
 	}
 	
 	//!extra!

 	//Na dfs, ao processar o nodo u os (sz[u]-1) proximos elementos todos de sua subarvore estarao logo apos ele no vetor v de valores da seg tree
 	T querySubtree(int u) { //consulta na subarvore inteira do nodo u
 		if(sz[u]==1) return 0; //árvore com 1 vertice nao tem aresta --> peso 0
 		return st.query(pos[u]+1, pos[u] + sz[u] - 1);
 	}
 	//soma valor a subarvore..
 	void updateSubtree(int u, T valor) {
 		if(sz[u]==1) return;
 		st.update(pos[u]+1, pos[u] + sz[u] - 1,valor);
 	}
 	int lca(int u, int v) {
 		if(pos[u] < pos[v]) swap(u,v);

 		if(head[u]==head[v]) //estao na mesma chain...
 			return v;
 		return lca(parent[head[u]], v); 
 	}

private:
	void dfs(vector<vector<pair<int,int>> > &adj,int root, int prev) {
		sz[root] = 1;
		for(auto &vizinho:adj[root]) if(vizinho.first!=prev) {
			auto &[w,peso] = vizinho;
			depth[w] = depth[root] + 1; //opcional
			dfs(adj,w,root);
			sz[root] += sz[w];
			if(sz[w] > sz[ adj[root][0].first ] || adj[root][0].first ==prev) 
				swap(adj[root][0],vizinho); //o primeiro filho de cada vértice será sempre o maior (ou seja, terá um heavy edge entre eles)
		}
	}
	//ct = ordem de visitacao dos vertices (comeca de 0)
	//value = peso das arestas, que ficara na seg tree
	void build(vector<vector<pair<int,int>> > &adj, vector<T> &v,
		       int &ct, int root, int valorAresta, int prev)  {
		pos[root] = ct;
		v[ct] = valorAresta; 
		ct++;
		for(auto &[w,peso]:adj[root]) if(w!=prev) { //vizinhos de root 			
			parent[w] = root;
			//cada vertice é cabeca da sua chain. 
			//Depois arrumamos isso para os vertices que estiverem no heavy path
			head[w] = (w==adj[root][0].first?head[root]:w); 
			build(adj, v,ct,w,peso,root);			
		}
	}

	

	int ct;

	vector<int> pos; //posicao do vertice na seg tree 
	vector<int> sz; //sz[v] é o tamanho da subarvore com raiz em v
	vector<int> parent; //pai de cada vertice
	vector<int> head; //head (começo) de cada chain
	vector<int> depth; //nivel de cada nodo (OPCIONAL, mas pode ser util...)

	SegTree st;
};



int main() {
	vector<vector<pair<int,int> > > adj(6);
	adj[0].push_back(make_pair(1,1)); 
	adj[0].push_back(make_pair(4,7) );
	adj[1].push_back(make_pair(0,1)); 
	adj[1].push_back(make_pair(3,3)); 
	adj[1].push_back(make_pair(2,4)); 
	adj[2].push_back(make_pair(1,4) ); 
	adj[3].push_back(make_pair(1,3) ); 
	adj[4].push_back(make_pair(5,6)); 
	adj[4].push_back(make_pair(0,7) );
	adj[5].push_back(make_pair(4,6) ); 
	Hld hl(adj);
	cout << hl.query(0,5) << endl;
	cout << hl.query(4,5) << endl;
	cout << hl.query(5,1) << endl;
	hl.updatePath(2,0,2);
	cout << hl.query(0,5) << endl;
	cout << hl.query(4,5) << endl;
	cout << hl.query(5,1) << endl;
}
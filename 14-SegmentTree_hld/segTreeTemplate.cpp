#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

#define INF 1000000000

struct TipoNodo {
	...
};



typedef TipoNodo T;
class SegTree {
public:
	SegTree(int n) {
		mxPos = n-1;
		t.resize(4*n);
	}

	T combine(T a, T b) {
	    T d;
		....

		return d;
	}

	T make(int x) {
		T d;
		....
		return d;
	}
	T makeVazio() {
		T d;
		....
		return d;
	}
	//constroi a segTree com os elementos de a
	//inicialmente, tl=0, tr=n-1
	void build(vector<int> &a, int tl, int tr, int v) {
	    if (tl == tr) { //folha
	        t[v] = make(a[tl]); //adaptar
	    } else {
	        int tm = (tl + tr) / 2; //meio do segmento
	        //constroi nodos da esquerda ([tl,tm])
	        build(a, tl, tm, v*2); 
	        //constroi nodos da direita ([tm+1,tr])
	        build(a, tm+1, tr, v*2+1); 
	        //junta o resultado dos segmentos da esquerda e direita
	        //em geral, aqui é lugar mais "adaptado"
	        t[v] = combine(t[v*2] , t[v*2+1]); 
	    }
	}
	void build(vector<int> &a) {
		build(a,0,mxPos,1);
	}

	//consulta para encontrar a soma do intervalo [l,r]
	//procura no vértice v, representando os intervalos
	//[tl,tr] (na árvore)
	T query(int v, int tl, int tr, int l, int r) {
	    if (l > r) 
	        return makeVazio(); //adaptar, caso outro tipo de consulta
	    if (l == tl && r == tr) {
	        return t[v];
	    }
	    int tm = (tl + tr) / 2;

	    //chama SEMPRE para os dois lados
	    //mesmo se desnecessário --> ok (primeiro if)
	    //adaptar, caso outro tipo de consulta
	    return combine(query(v*2, tl, tm, l, min(r, tm)),
	            query(v*2+1, tm+1, tr, max(l, tm+1), r) );
	}
	T query(int l, int r) {
		return query(1,0,mxPos,l,r);
	}

	//nesse caso, new_val é inteiro
	void update(int v, int tl, int tr, int pos, int new_val) {
	    if (tl == tr) {
	        t[v] = make(new_val);
	    } else {
	        int tm = (tl + tr) / 2;
	        if (pos <= tm) //nodo está na esquerda?
	            update(v*2, tl, tm, pos, new_val);
	        else //nodo está na direita?
	            update(v*2+1, tm+1, tr, pos, new_val);

	        //atualiza raiz atual
	        //adaptar, caso outro tipo de consulta
	        t[v] = combine(t[v*2] , t[v*2+1]);
	    }
	}
	void update(int pos, int new_val) {
		update(1,0,mxPos,pos,new_val);
	}
private:
	vector<T> t;
	int mxPos;


};


int main() {
	
}

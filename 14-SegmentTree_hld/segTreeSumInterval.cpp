#include <iostream>
#include <vector>
using namespace std;

//consulta do tipo soma de intervalo
//permite somar valores a um intervalo
//usa lazy propagation para permitir as atualizacoes eficientes

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


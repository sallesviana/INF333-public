#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
typedef unsigned long long ull;

class Sqrt {
public:
	Sqrt(int tamBloco_,const vector<int> &v_): tamBloco(tamBloco_), v(v_) {
		somaBlocos = vector<ull>(2 + (int)ceil(((double)v.size())/tamBloco_) ,0); //somo 2 por nao confiar rsrsrs
		for(int i=0;i<v.size();i++) {
			somaBlocos[i/tamBloco] += v[i];
		}
	}
	void update(int i, int val) { //i no intervalo [0,n)
		somaBlocos[i/tamBloco] -= v[i];
		v[i] = val;
		somaBlocos[i/tamBloco] += v[i];
	}
	ull prefix(int i) { //soma do intervalo [0,i)
		ull soma = 0;
		int j = 0;
		while(j<i) {
			if(j/tamBloco<i/tamBloco) { //o bloco do j está antes do bloco do i? se sim, some o bloco todo e pule
				soma += somaBlocos[j/tamBloco];
				j += tamBloco;
			} else { //anda dentro do bloco de i...
				soma += v[j];
				j++;
			}
		}
		return soma;
	}
	ull query(int a, int b) { //soma do intervalo [a,b), contando a partir de 0
		return prefix(b) - prefix(a);
	}

private:
	int tamBloco;
	vector<int> v;
	vector<ull> somaBlocos;
};

int main() {
	int n, q;
	cin >> n >> q;

	vector<int> v(n);
	for(auto&el:v) cin >> el;
	Sqrt sq((int)ceil(sqrt(n)), v);

	for (int i = 0; i < q; i++) {
		int t, l, r;
		cin >> t >> l >> r;
		if (t == 1) {
			sq.update(l - 1, r);
		} else {
			cout << sq.query(l-1, r) << "\n"; //uso r aqui, porque conta a partir de 1 e query considera intervalo semiaberto
		}
	}
}
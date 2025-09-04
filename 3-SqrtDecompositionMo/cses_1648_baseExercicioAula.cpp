#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
typedef unsigned long long ull;


int main() {
	int n, q;
	cin >> n >> q;

	vector<int> v(n);
	vector<long long> prefix(n+1);
	for(auto&el:v) cin >> el;

	for(int i=1;i<=n;i++) { //prefixo(a) = soma(0...a-1)
		prefix[i] = prefix[i-1] + v[i-1];
	}	





	vector<pair<int,long long> > atualizacoes;
	for (int i = 0; i < q; i++) {
		int t, l, r;
		cin >> t >> l >> r;
		if (t == 1) {
			//sq.update(l - 1, r);
			atualizacoes.push_back( make_pair(l-1,r-v[l-1]) ); //tenho que somar r-v[i] para compensar a mudanca...
			v[l-1] = r;
		} else { //calcular soma de l-1 até r-1
			long long soma = prefix[r] - prefix[l-1];
			//varre as atualizacoes corrigindo soma...
			for(auto &[pos, valor]:atualizacoes) {
				if( l-1 <= pos && pos <= r-1)
					soma += valor;
			}

			cout << soma << "\n"; //uso r aqui, porque conta a partir de 1 e query considera intervalo semiaberto
		}





		
	}
}
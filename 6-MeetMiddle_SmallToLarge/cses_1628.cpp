#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
using namespace std;

/*
4 5
1 2 3 2*/


vector<long long> subconjuntos(const vector<int> &v) {
	vector<long long> ans;
	int n = v.size();
	for(int b=0;b< (1<<n); b++) { //gera mascaras de subconjuntos
		long long s = 0;
		for(int i=0;i<n;i++) 
			if( b&(1<<i) ) s+= v[i]; //bit i ligado?
		ans.push_back(s);
	}
	sort(ans.begin(),ans.end());
	return ans;
}

int main() {
	ios_base::sync_with_stdio(false);cin.tie(NULL); 

	int n, m;
	cin >> n >> m;
	vector<int> v(n);
	for(int i=0;i<n;i++) cin >> v[i];

	//divide o array em 2
	vector<int> v1(v.begin(),v.begin()+n/2);
	vector<int> v2(v.begin()+n/2,v.end());

	//acha todos os numeros obtiveis como subconjuntos de v1 e v2
	vector<long long> s1 = subconjuntos(v1);
	vector<long long> s2 = subconjuntos(v2);


	//agora temos que ver quantos conjuntos formados por numeros de s1 com numeros de s2 valem o m
	long long ans = 0;
	for( long long a:s1) {
		//temos que ver quantas vezes m-a aparece em s2...
		auto &&[it1,it2] = equal_range( s2.begin(), s2.end(), m-a); //rara vez em que uso && ! 
		ans += it2-it1;
	}
	cout << ans << endl;
}
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
typedef unsigned long long ull;


/*
5
1 2 3 1 1*/

int main() {
	int n;
	cin >> n;
	vector<int> v(n);
	for(int &el:v) cin >> el;

	vector<ull> dp(n,1);
	for(int i=n-1;i>=0;i--) {
		//de quantas formas consigo pintar a partir de i, supondo que o quadrado i está preto (a peça está em i?)
		//e que já calculei em dp as respostas para todos quadrados da frente?
		int elem = v[i]; // elemento que está na posicao i

		ull ans = 1;

		
		for(int x=1;i + x*elem < n;x++) { //salta de elem em elem a partir de i
			ans = (ans + dp[i + x*elem])%998244353;
		}


		dp[i] = ans;
	}

	cout << dp[0] << endl;
}


#include <iostream>
#include <vector>
using namespace std;


//o certo eh passar por const, mas estamos na maratona :)
void print(vector<int> &v) {
	for(int i= 0;i<v.size();i++) 
		cout << v[i] << " ";
	cout << "\n";
}


int main() {
	vector<int> v;
	vector<int> v2;
	//criar vector (construtores),  etc...


	//size, empty, front, back
	v.push_back(1);
	v.push_back(2);
	print(v);
	cout << v.back() << endl;
	v.pop_back();
	print(v);

	//pop_back


	//[], resize(), reserve(), push_back, performance...
	v.resize(4,3);
	v.resize(2);
	v.resize(5,7);
	v = vector<int>();
	v.resize(10);
	print(v);

	//copiar vector para vector

	//criar matriz, como inicializar (duas formas...)

	//dado um vector ja com elementos, como reinicializar tudo com 0 ?

}
#include <vector>
#include <iostream>
using namespace std;

int main() {
	vector<bool> v{true,false,true,true,false};
	bool &bref = v[0]; //não é possível criar uma referência para um elemento de um vector<bool> (pois os elementos são armazenados em bits)
	bref = false;
	for(bool b:v) cout << b << endl;
}
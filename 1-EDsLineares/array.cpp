#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

int main() {
	int v[10];
	int v2[10];

	for( ... ) {
		memset(v,1,n*sizeof(int));
		memcpy(v,v2,n*sizeof(int));
	}

}
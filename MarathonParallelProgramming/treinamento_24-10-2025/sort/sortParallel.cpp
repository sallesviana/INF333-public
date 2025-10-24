/**********************************************************************
 * Enumeration sort
 * Based on https://github.com/yohannteston/Parallel-course-Lab3/blob/master/labOpenMP/enumsort.c
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <cstdlib>
using namespace std;


#define LENGTH 8

FILE *fin, *fout;

char *strings;
long int N;

void openfiles() {
	fin = fopen("sort.in", "r+");
	if (fin == NULL ) {
		perror("fopen fin");
		exit(EXIT_FAILURE);
	}

	fout = fopen("sort.out", "w");
	if (fout == NULL ) {
		perror("fopen fout");
		exit(EXIT_FAILURE);
	}
}

void closefiles(void) {
	fclose(fin);
	fclose(fout);
}

#include <iostream>

void enun_sort(char *&a, int length, long int size) {

	long int i, j, rank;
	char *tmp = (char *) malloc(length);

	char *ans = (char *) malloc(length*size);
	vector<bool> usei(size,false);

	//todo: usar thrust + unordered set marcando as strings ja usadas
	//thrust para contar quantos menores e quantos iguais existem...

	for (j = 0; j < size; j++) {
		if(usei[j]) continue;

		int menor = 0;
		int igual = 0;

		#pragma omp parallel for reduction( + : igual, menor)
		for (i = 0; i < size; i++) {
			int cmp = strcmp(a + (i * length), a + (j * length));
			if(cmp<0) menor++;
			if(cmp==0) {
				usei[i] = true;
				igual++;
			}
		}
		#pragma omp parallel for
		for(int i=0;i<igual;i++) {
			strcpy(ans + ((menor + i) * length), a + (j * length));
		}

	}


	free(tmp);

	swap(ans, a);
}

int main(int argc, char* argv[]) {

	long int i;

	openfiles();

	fscanf(fin, "%ld", &N);
	strings = (char*) calloc(N, LENGTH);
	if (strings == NULL ) {
		perror("malloc strings");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < N; i++)
		fscanf(fin, "%s", strings + (i * LENGTH));

	enun_sort(strings, LENGTH, N);

	for (i = 0; i < N; i++)
		fprintf(fout, "%s\n", strings + (i * LENGTH));

	free(strings);
	closefiles();

	return EXIT_SUCCESS;
}


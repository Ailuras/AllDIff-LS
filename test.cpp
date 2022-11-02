#include <bits/stdc++.h>

using namespace std;

void main() {
    int order;
	int is_sat;
	int** vertex;
	
	cin >> order;
	cin >> is_sat;
	int vertex_size = order*order;
	vertex = (int **) malloc(sizeof(int *) * (vertex_size));
	for(int i = 0; i < vertex_size; i++) vertex[i] = (int *) malloc(sizeof(int) * (vertex_size));
	for(int i=0; i<vertex_size; i++) {
		for (int j=0; j<vertex_size; j++) {
			cin >> vertex[i][j];
		}
	}

	for(int i=0; i<vertex_size; i++) {
		for (int j=0; j<vertex_size; j++) {
			cout << vertex[i][j];
		}
		cout << endl;
	}

}
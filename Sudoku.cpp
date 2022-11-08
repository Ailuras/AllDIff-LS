/*================================================================
*   Copyright (C) 2021 ******** All rights reserved.
*   
*   File Name     ：FastLSC.cpp
*   Creator       ：******
*   Creation Date ：2021/02/11
*   Describe      ：
*
================================================================*/

#include <bits/stdc++.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <libgen.h>

using namespace std;

struct Edge{
	int x;
	int y;
	bool h;
	bool operator < (const Edge& kami) const {
		if(kami.x < x) return true;
		else if(kami.x == x) {
			if(kami.y < y) return true;
		}
		return false;
	}
};

int pppp_cnt;

struct St{
	int id;
	int c;
	long long score;
	bool operator < ( const St& other) const {
		if(id < other.id) return true;
		else if(id == other.id) {
			if(c < other.c) return true;
			else return false;
		}
		else return false;
	}
};

struct BClash{
	int e_id;
	int color;
	bool operator < (const BClash& kami) const {
		if(kami.e_id < e_id) return true;
		else return false;
	}
};

St* tmp_st;
St* tmp_tabu_st;
int* over_color;
St* move_vertex;
St* bms_array;
int* move_vertex_array;
int* move_vertex_position;
int** move_vertex_id;
int move_vertex_size;

const int bool_flag1 = true;		
const int tabuStep = 10;
bool flag_TabuCC;
const int CC = 0;
const int population_size = 20;
const int max_iter = 100000;
const double alpha = 0.6;
int total_Iters;
int Iter;

FILE *fp;

Edge* edge;
int** G;

int seed;
long long steps;
long long no_improve_steps;
int min_Clash;
int** tabu;
bool** cc;
bool** tabuC;
int*** tabuCV;
int** tabuCVlen;
int* tabuE;
int* tabuEC;
int tabuELen;

tms start;
tms finish;
int start_time;
double total_time;
double best_time;
double time_limit;

string filename;
string outPut;

int order;
int group_size;
int is_sat;
int vertex_size;    		
int edge_size;  		
int square_length;			
int** Neighbour;			
int* Neighbour_length;		
int** vertex_color;			
int* vertex_color_length;	
int** color_vertex_pos;  	
int* mVertexes;				
int* mVertexesPosition;		
int* CL_start_pos;       	
int vertex_use_length;		
int vertex_use_length_tmp;
int edge_use_length;
int** mColors;				
int* mColorsLength;			
int** vertex_color_pos;		
int* mEdges;				
int* mEdgesPosition;		
int edge_clash_length;		
int* mVertexesColor;		
int* mVertexesColorTmp;
int* mVertexesColorBest;
long long* score;			
int** mClash;				
int*** color_move;			
int** color_move_length;	
int*** color_move_pos;
bool** vertex_can_move;     
int** color_vertex;         
int* color_vertex_length;   
int ** color_vp;
int** freq;                 
int* total_freq;            
int* color_clash;           
int*** mCol;
int*** mCol_pos;
int** mCol_length;
int*** mRow;
int*** mRow_pos;
int** mRow_length;
int*** mGroup;
int*** mGroup_pos;
int** mGroup_length;
int** color_weight;
int** mPopulation;
int** mPopulationClash;
int** mPopulationClashColor;
long long** mPopulationVertexOld;
int* mPopulationOld;
int mPopulation_id;
int population_cnt;
int mPopulationClashLen;
long long* mPopulationMaxVOld;
bool mPopulationFlag;
int* ClashVertexes;
int* ClashVertexesPos;

int maxPC;
int minPC;
int maxDist;
int minDist;
int total_clash;
int edge_clash;
int vertex_clash_length;
int* tmp_array;
bool* tmp_bool;
Edge* edge_array;
int* ans;
int last_move;
int* init_best_order_array;
int* tmp_iboa;
int* tmp_vcl;
int* best_array;
int best_clash;
int* square_array;
int* square_cnt;
bool* t_array;
int t_cnt;
int local_min_clash;
int** tmp_clash;
BClash* best_clash_array;
int* tmp_p_array;
int* color_max_size;
long long** vertex_old;
int update_Iter;
int total_zero;
int total_all;

int to_index(int index1, int index2, bool type) {
	// row, col
	if(type) return (index1-1)*group_size + index2;
	// group, point
	int row_low = int((index1-1)/order)*order + int((index2-1)/order);
	int col = ((index1-1)%order)*order + (index2-1)%order + 1;
	return row_low*group_size + col;
}

void set_Variable_size() {
	square_length = group_size;
	G = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) G[i] = (int *) malloc(sizeof(int) * (vertex_size + 2));
	edge = (Edge *) malloc(sizeof(Edge) * (edge_size + 1));
	Neighbour = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) Neighbour[i] = (int *) malloc(sizeof(int) * (3 * square_length));
	Neighbour_length = (int *) malloc(sizeof(int) * (vertex_size + 2));
	vertex_color = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) vertex_color[i] = (int *) malloc(sizeof(int) * (square_length + 1));
	vertex_color_length = (int *) malloc(sizeof(int) * (vertex_size + 2));
	color_vertex_pos = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) color_vertex_pos[i] = (int *) malloc(sizeof(int) * (square_length + 2));
	mVertexes = (int *) malloc(sizeof(int) * (vertex_size + 2));
	mVertexesPosition = (int *) malloc(sizeof(int) * (vertex_size + 2));
	CL_start_pos = (int *) malloc(sizeof(int) * (square_length + 2));
	mColors = (int **) malloc(sizeof(int *) * (square_length + 2));
	for(int i = 1; i <= square_length; i++) mColors[i] = (int *) malloc(sizeof(int) * (vertex_size + 2));
	mColorsLength = (int *) malloc(sizeof(int) * (square_length + 2));
	vertex_color_pos = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) vertex_color_pos[i] = (int *) malloc(sizeof(int) * (square_length + 2));
	mEdges = (int *) malloc(sizeof(int) * (edge_size + 2));
	mEdgesPosition = (int *) malloc(sizeof(int) * (edge_size + 2));
	mVertexesColor = (int *) malloc(sizeof(int) * (vertex_size + 2));
	mVertexesColorTmp = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mVertexesColorBest = (int *) malloc(sizeof(int) * (vertex_size + 1));
	score = (long long *) malloc(sizeof(long long) * (edge_size + 2));
	mClash = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) mClash[i] = (int *) malloc(sizeof(int) * (square_length + 2));
	tmp_array = (int *) malloc(sizeof(int) * (vertex_size + 2));
	ans = (int *) malloc(sizeof(int) * (vertex_size + 2));
	tabu = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) tabu[i] = (int *) malloc(sizeof(long long) * (square_length + 2));
	cc = (bool **) malloc(sizeof(bool *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) cc[i] = (bool * ) malloc(sizeof(bool) * (square_length + 1));
	tabuC = (bool **) malloc(sizeof(bool *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) tabuC[i] = (bool *) malloc(sizeof(bool) * (square_length + 1));
	tabuCV = (int ***) malloc(sizeof(int **) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) {
		tabuCV[i] = (int **) malloc(sizeof(int *) * (square_length + 1));
		for(int j = 1; j <= square_length; j++) tabuCV[i][j] = (int *) malloc(sizeof(int) * vertex_size);
	}
	tabuCVlen = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) tabuCVlen[i] = (int *) malloc(sizeof(int) * (square_length + 1));
	tabuE = (int *) malloc(sizeof(int) * edge_size);
	tabuEC = (int *) malloc(sizeof(int) * edge_size);
	edge_array = (Edge *) malloc(sizeof(Edge) * (edge_size + 2)); 
	vertex_can_move = (bool **) malloc(sizeof(bool *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) vertex_can_move[i] = (bool *) malloc(sizeof(bool) * (square_length + 2));
	color_vertex = (int **) malloc(sizeof(int *) * (square_length + 2));
	for(int i = 1; i <= square_length; i++) color_vertex[i] = (int *) malloc(sizeof(int) * (vertex_size + 2));
	color_vp = (int **) malloc(sizeof(int *) * (square_length + 2));
	for(int i = 1; i <= square_length; i++) color_vp[i] = (int *) malloc(sizeof(int) * (vertex_size + 2));
	color_vertex_length = (int *) malloc(sizeof(int) * (square_length + 2));
	color_clash = (int *) malloc(sizeof(int) * (square_length + 2));
	init_best_order_array = (int *) malloc(sizeof(int) * (vertex_size + 2));
	tmp_bool = (bool *) malloc(sizeof(bool) * (vertex_size + 2));
	tmp_iboa = (int *) malloc(sizeof(int) * (vertex_size + 2));
	tmp_vcl = (int *) malloc(sizeof(int) * (vertex_size + 2));
	best_array = (int *) malloc(sizeof(int) * (vertex_size + 2));
	square_array = (int *) malloc(sizeof(int) * (square_length + 2));
	square_cnt = (int *) malloc(sizeof(int) * (square_length + 2));
	tmp_st = (St *) malloc(sizeof(St) * (vertex_size * square_length));
	tmp_tabu_st = (St *) malloc(sizeof(St) * (vertex_size * square_length));
	over_color = (int *) malloc(sizeof(int) * (square_length + 3));
	mCol = (int ***) malloc(sizeof(int **) * (square_length + 3));
	for(int i = 1; i <= square_length; i++) {
		mCol[i] = (int **) malloc(sizeof(int *) * (square_length + 3));
		for(int j = 1; j <= square_length; j++) mCol[i][j] = (int *) malloc(sizeof(int) * (square_length + 3));
	}
	mCol_pos = (int ***) malloc(sizeof(int **) * (square_length + 3));
	for(int i = 1; i <= square_length; i++) {
		mCol_pos[i] = (int **) malloc(sizeof(int *) * (square_length + 3));
		for(int j = 1; j <= square_length; j++) mCol_pos[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 3));
	}
	mCol_length = (int **) malloc(sizeof(int *) * (square_length + 3));
	for(int i = 1; i <= square_length; i++) mCol_length[i] = (int *) malloc(sizeof(int) * (square_length + 3));
	mRow = (int ***) malloc(sizeof(int **) * (square_length + 3));
	for(int i = 1; i <= square_length; i++) {
		mRow[i] = (int **) malloc(sizeof(int *) * (square_length + 3));
		for(int j = 1; j <= square_length; j++) mRow[i][j] = (int *) malloc(sizeof(int) * (square_length + 3));
	}
	mRow_pos = (int ***) malloc(sizeof(int **) * (square_length + 3));
	for(int i = 1; i <= square_length; i++) {
		mRow_pos[i] = (int **) malloc(sizeof(int *) * (square_length + 3));
		for(int j = 1; j <= square_length; j++) mRow_pos[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 3));
	}
	mRow_length = (int **) malloc(sizeof(int *) * (square_length + 3));
	for(int i = 1; i <= square_length; i++) mRow_length[i] = (int *) malloc(sizeof(int) * (square_length + 3));
	mGroup = (int ***) malloc(sizeof(int **) * (square_length + 3));
	for(int i = 1; i <= square_length; i++) {
		mGroup[i] = (int **) malloc(sizeof(int *) * (square_length + 3));
		for(int j = 1; j <= square_length; j++) mGroup[i][j] = (int *) malloc(sizeof(int) * (square_length + 3));
	}
	mGroup_pos = (int ***) malloc(sizeof(int **) * (square_length + 3));
	for(int i = 1; i <= square_length; i++) {
		mGroup_pos[i] = (int **) malloc(sizeof(int *) * (square_length + 3));
		for(int j = 1; j <= square_length; j++) mGroup_pos[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 3));
	}
	mGroup_length = (int **) malloc(sizeof(int *) * (square_length + 3));
	for(int i = 1; i <= square_length; i++) mGroup_length[i] = (int *) malloc(sizeof(int) * (square_length + 3));
	t_array = (bool *) malloc(sizeof(bool) * (vertex_size + 2));
	mPopulation = (int **) malloc(sizeof(int *) * population_size);
	for(int i = 0; i < population_size; i++) {
		mPopulation[i] = (int *) malloc(sizeof(int) * (1 + vertex_size));
	}
	mPopulationClash = (int **) malloc(sizeof(int *) * population_size);
	for(int i = 0; i < population_size; i++) mPopulationClash[i] = (int *) malloc(sizeof(int) * (edge_size + 1));
	mPopulationClashColor = (int **) malloc(sizeof(int *) * population_size);
	for(int i = 0; i < population_size; i++) mPopulationClashColor[i] = (int *) malloc(sizeof(int) * (edge_size + 1));
	mPopulationVertexOld = (long long **) malloc(sizeof(long long *) * population_size);
	for(int i = 0; i < population_size; i++) mPopulationVertexOld[i] = (long long *) malloc(sizeof(long long) * (vertex_size + 1));
	mPopulationOld = (int *) malloc(sizeof(int) * population_size);
	mPopulationMaxVOld = (long long *) malloc(sizeof(long long) * population_size);
	ClashVertexes = (int *) malloc(sizeof(int) * (vertex_size + 1));
	ClashVertexesPos = (int *) malloc(sizeof(int) * (vertex_size + 1));
	tmp_clash = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) tmp_clash[i] = (int *) malloc(sizeof(int) * (square_length + 2));
	best_clash_array = (BClash *) malloc(sizeof(BClash) * (edge_size + 1));
	tmp_p_array = (int *) malloc(sizeof(int) * population_size);
	color_max_size = (int *) malloc(sizeof(int) * (square_length + 1));
	vertex_old = (long long **) malloc(sizeof(long long *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) vertex_old[i] = (long long *) malloc(sizeof(long long) * 2);
	move_vertex = (St *) malloc(sizeof(St) * (square_length * vertex_size + 1));
	bms_array = (St *) malloc(sizeof(St) * (square_length * vertex_size + 1));
	move_vertex_array = (int *) malloc(sizeof(int) * (square_length * vertex_size + 1));
	move_vertex_position = (int *) malloc(sizeof(int) * (square_length * vertex_size + 1));
	move_vertex_id = (int **) malloc(sizeof(int * ) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) move_vertex_id[i] = (int *) malloc(sizeof(int) * (square_length + 1));
	return;
}

void init() {
	for(int i = 1; i <= vertex_size; i++) for(int j = 1; j <= vertex_size; j++) G[i][j] = -1;
	for(int i = 1; i <= vertex_size; i++) Neighbour_length[i] = 0;
	for(int i = 1; i <= vertex_size; i++) vertex_color_length[i] = 0;
	for(int i = 1; i <= vertex_size; i++) for(int j = 1; j <= square_length; j++) color_vertex_pos[i][j] = square_length + 1;
	//for(int i = 1; i <= vertex_size; i++) mVertexes[i] = i;
	for(int i = 1; i <= vertex_size; i++) {
		mVertexesPosition[i] = -1;
	}
	CL_start_pos[1] = 1;
	vertex_use_length = vertex_size;
	for(int i = 1; i <= vertex_size; i++) {
		for(int j = 1; j <= square_length; j++) vertex_color_pos[i][j] = 0;
	}
	for(int i = 1; i <= edge_size; i++) {
		mEdges[i] = i;
		mEdgesPosition[i] = i;
	}
	for(int i = 1; i <= vertex_size; i++) mVertexesColor[i] = -1;
	for(int i = 1; i <= vertex_size; i++) for(int j = 1; j <= square_length; j++) mClash[i][j] = 0;
	total_clash = 0;
	edge_clash = 0;
	for(int i = 1; i <= vertex_size; i++) {
		for(int j = 1; j <= square_length; j++) tabuC[i][j] = false;
	}
	tabuELen = 0;
	//for(int i = 1; i <= vertex_size; i++) tabu[i] = INT_MIN;
	for(int i = 1; i <= vertex_size; i++) {
		for(int j = 1; j <= square_length; j++) vertex_can_move[i][j] = false;
	}
	for(int i = 1; i <= square_length; i++) color_clash[i] = 0;
	best_clash = INT_MAX;
	for(int i = 1; i <= square_length; i++) {
		for(int j = 1; j <= square_length; j++) {
			for(int p = 1; p <= square_length; p++) {
				mCol[i][j][p] = square_length * (i - 1) + p;
				mCol_pos[i][j][square_length * (i - 1) + p] = p;
			}
		}
	}
	for(int i = 1; i <= square_length; i++) for(int j = 1; j <= square_length; j++) mCol_length[i][j] = square_length;
	for(int i = 1; i <= square_length; i++) {
		for(int j = 1; j <= square_length; j++) {
			for(int p = 1; p <= square_length; p++) {
				mRow[i][j][p] = square_length * (p - 1) + i;
				mRow_pos[i][j][square_length * (p - 1) + i] = p;
			}
		}
	}
	for(int i = 1; i <= square_length; i++) for(int j = 1; j <= square_length; j++) mRow_length[i][j] = square_length;
	for(int i = 1; i <= square_length; i++) {
		for(int j = 1; j <= square_length; j++) {
			for(int p = 1; p <= square_length; p++) {
				mGroup[i][j][p] = to_index(i, p, false);
				mGroup_pos[i][j][to_index(i, p, false)] = p;
				// cout<<"index: "<<index<<endl;
				// cout<<"group: "<<i<<endl;
				// cout<<"point: "<<p<<endl;
				// cout<<"mGroup: "<<index<<endl;
				// cout<<"mGroup_pos: "<<p<<endl;
			}
		}
	}
	for(int i = 1; i <= square_length; i++) for(int j = 1; j <= square_length; j++) mGroup_length[i][j] = square_length;
	for(int i = 1; i <= square_length; i++) {
		for(int j = 0; j < vertex_size; j++) {
			color_vertex[i][j] = j + 1;
			color_vp[i][j + 1] = j;
		}
	}
	for(int i = 1; i <= square_length; i++) color_vertex_length[i] = vertex_size;
	for(int i = 1; i <= vertex_size; i++) t_array[i] = false;
	t_cnt = 0;
	for(int i = 1; i <= vertex_size; i++) {
		for(int j = 1; j <= square_length; j++) {
			tmp_clash[i][j] = 0;
		}
	}
	for(int i = 1; i <= square_length; i++) color_max_size[i] = square_length;
	mPopulationClashLen = INT_MAX;
	return;
}

void swap_vertex(int id_a, int pos_b) {
	int id_b = mVertexes[pos_b];
	int pos_a = mVertexesPosition[id_a];
	mVertexes[pos_a] = id_b;
	mVertexesPosition[id_a] = pos_b;
	mVertexes[pos_b] = id_a;
	mVertexesPosition[id_b] = pos_a;
	return;
}

void swap_edge(int id_a, int pos_b) {
	int id_b = mEdges[pos_b];
	int pos_a = mEdgesPosition[id_a];
	mEdges[pos_a] = id_b;
	mEdgesPosition[id_a] = pos_b;
	mEdges[pos_b] = id_a;
	mEdgesPosition[id_b] = pos_a;
	return;
}

void swap_vertex_color(int id, int c_a, int p_b) {
	int c_b = vertex_color[id][p_b];
	int p_a = color_vertex_pos[id][c_a];
	swap(vertex_color[id][p_a], vertex_color[id][p_b]); 
	color_vertex_pos[id][c_a] = p_b;
	color_vertex_pos[id][c_b] = p_a;
	return;
}

void swap_vertex_with_color(int id, int from, int to) {
	int id_v = mColors[from][mColorsLength[from] - 1];
	mColors[from][vertex_color_pos[id][from]] = mColors[from][--mColorsLength[from]];
	vertex_color_pos[id_v][from] = vertex_color_pos[id][from];
	mColors[to][mColorsLength[to]++] = id;
	vertex_color_pos[id][to] = mColorsLength[to] - 1;
	mVertexesColor[id] = to;
	return;
}

void swap_col(int col_id, int c_id, int id_a, int pos_b) {
	int id_b = mCol[col_id][c_id][pos_b];
	int pos_a = mCol_pos[col_id][c_id][id_a];
	mCol[col_id][c_id][pos_a] = id_b;
	mCol[col_id][c_id][pos_b] = id_a;
	mCol_pos[col_id][c_id][id_a] = pos_b;
	mCol_pos[col_id][c_id][id_b] = pos_a;
	return;
}

void swap_row(int row_id, int c_id, int id_a, int pos_b) {
	int id_b = mRow[row_id][c_id][pos_b];
	int pos_a = mRow_pos[row_id][c_id][id_a];
	mRow[row_id][c_id][pos_a] = id_b;
	mRow[row_id][c_id][pos_b] = id_a;
	mRow_pos[row_id][c_id][id_a] = pos_b;
	mRow_pos[row_id][c_id][id_b] = pos_a;
	return;
}

void swap_group(int group_id, int c_id, int id_a, int pos_b) {
	int id_b = mGroup[group_id][c_id][pos_b];
	int pos_a = mGroup_pos[group_id][c_id][id_a];
	mGroup[group_id][c_id][pos_a] = id_b;
	mGroup[group_id][c_id][pos_b] = id_a;
	mGroup_pos[group_id][c_id][id_a] = pos_b;
	mGroup_pos[group_id][c_id][id_b] = pos_a;
	return;
}

void swap_color_vertex(int c_id, int id_a, int pos_b) {
	int pos_a = color_vp[c_id][id_a];
	int id_b = color_vertex[c_id][pos_b];
	color_vertex[c_id][pos_a] = id_b;
	color_vertex[c_id][pos_b] = id_a;
	color_vp[c_id][id_a] = pos_b;
	color_vp[c_id][id_b] = pos_a;
	return;
}

void swap_color_move(int from, int to, int id) {
	for(int i = 0; i < vertex_color_length[id]; i++) {
		int tmp_c = vertex_color[id][i];
		if(tmp_c != from) {
			int pos = color_move_pos[from][tmp_c][id];
			int len = color_move_length[from][tmp_c];
			int l_id = color_move[from][tmp_c][len - 1];
			color_move_pos[from][tmp_c][l_id] = pos;
			color_move[from][tmp_c][pos] = l_id;
			color_move_length[from][tmp_c]--;
		}
		if(tmp_c != to) {
			int len = color_move_length[to][tmp_c];
			color_move_pos[to][tmp_c][id] = len;
			color_move[to][tmp_c][len] = id;
			color_move_length[to][tmp_c]++;
		}
	}
	return;
}

void swap_move_vertex(int id_a, int pos_b) {
	int id_b = move_vertex_array[pos_b];
	int pos_a = move_vertex_position[id_a];
	move_vertex_array[pos_a] = id_b;
	move_vertex_array[pos_b] = id_a;
	move_vertex_position[id_a] = pos_b;
	move_vertex_position[id_b] = pos_a;
	return;
}

int cmp_CL(const void * elem1, const void * elem2) {
	int * p1 = (int *) elem1;
	int * p2 = (int *) elem2;
	return vertex_color_length[*p1] - vertex_color_length[*p2];
}

void check_move_vertex() noexcept {
	bool flag = true;
	long long cnt = 0;
	set<St> test_st;
	St h_st;
	for(int i = 0; i < vertex_clash_length; i++) {
		int id = ClashVertexes[i];
		for(int j = 0; j < vertex_color_length[id]; j++) {
			int tmp_c = vertex_color[id][j];
			if(tmp_c == mVertexesColor[id]) continue;
			//if(mClash[id][tmp_c] <= mClash[id][mVertexesColor[id]]) {
			cnt++;
			if(move_vertex_position[move_vertex_id[id][tmp_c]] >= move_vertex_size) {
				cout << "check move_vertex wrong: " << id << " " << tmp_c << " " << mVertexesColor[id] << endl;
				flag = false;
			}
			h_st.id = id;
			h_st.c = tmp_c;
			test_st.insert(h_st);
			//}
		}
	}
	for(int i = 0; i < move_vertex_size; i++) {
		int move_id = move_vertex_array[i];
		int id = move_vertex[move_id].id;
		int c = move_vertex[move_id].c;
		h_st.id = id;
		h_st.c = c;
		/*if(test_st.find(h_st) == test_st.end()) {
		  cout << "check move_vertex wrong: " << id << " " << c << " " << mVertexesColor[id] << " " << ClashVertexesPos[id] << " " << mClash[id][c] << " " << mClash[id][mVertexesColor[id]] << endl;
		  flag = false;
		  }*/
		if(ClashVertexesPos[id] == -1) {
			cout << "check move_vertex wrong: " << id << " " << c << " " << mVertexesColor[id] << " " << ClashVertexesPos[id] << endl;
			flag = false;
		}
		/*if(mClash[id][c] > mClash[id][mVertexesColor[id]]) {
		  cout << "check move_vertex wrong: " << id << " " << c << " " << mVertexesColor[id] << endl;
		  flag = false;
		  }*/
		if(c == mVertexesColor[id]) {
			cout << "check move_vertex wrong: " << id << " " << c << " " << mVertexesColor[id] << endl;
			flag = false;
		}
		//cout << id << " " << c << " " << mVertexesColor[id] << " " << mClash[id][c] << " " << mClash[id][mVertexesColor[id]] << endl;
	}
	if(cnt != move_vertex_size) {
		cout << "move_vertex_size wrong: " << cnt << " " << move_vertex_size << endl;
		flag = false;
	}
	if(!flag) exit(0);
	return;
}

void add_vertex(int id, int pos) {
	int cur_pos = mVertexesColor[id];
	//check_move_vertex();
	for(int i = 0; i < Neighbour_length[id]; i++) {
		int tmp_v = Neighbour[id][i]; 
		mClash[tmp_v][cur_pos] -= 1;
		mClash[tmp_v][pos] += 1;
		int edge_id = G[id][tmp_v];
		if(mVertexesColor[tmp_v] == cur_pos) {
			if(mClash[tmp_v][cur_pos] == 0) {
				ClashVertexes[ClashVertexesPos[tmp_v]] = ClashVertexes[--vertex_clash_length];
				ClashVertexesPos[ClashVertexes[ClashVertexesPos[tmp_v]]] = ClashVertexesPos[tmp_v];
				ClashVertexesPos[tmp_v] = -1;
				score[tmp_v] = 0;
			}
			swap_edge(edge_id, edge_clash_length--);
		}
		else if(mVertexesColor[tmp_v] == pos) {
			if(ClashVertexesPos[tmp_v] == -1) {
				ClashVertexes[vertex_clash_length] = tmp_v;
				ClashVertexesPos[tmp_v] = vertex_clash_length++;
			}
			swap_edge(edge_id, ++edge_clash_length);
		}
	}
	mVertexesColor[id] = pos;
	if(mClash[id][pos] == 0 && ClashVertexesPos[id] >= 0) {
		ClashVertexes[ClashVertexesPos[id]] = ClashVertexes[--vertex_clash_length];
		ClashVertexesPos[ClashVertexes[ClashVertexesPos[id]]] = ClashVertexesPos[id];
		ClashVertexesPos[id] = -1;
	}
	return;
}

int get_score(int id, int to) {
	return mClash[id][to] - mClash[id][mVertexesColor[id]];
}

int get_Col(int id) {
	return (id - 1) / square_length + 1;
}

int get_Row(int id) {
	return (id - 1) % square_length + 1;
}

int get_Group(int id) {
	return (id-1)/(group_size*order)*order + (id-1)%group_size/order + 1;
}

int calDist(int* a, int* b) {
	int res = 0;
	for(int i = 0; i < vertex_use_length; i++) {
		int id = mVertexes[i];
		if(a[id] != b[id]) res++;
	}
	return res;
}

void mStartTime() {
	times(&start);
	start_time = start.tms_utime + start.tms_stime; 
	return;
}

double mCurrentTime() {
	times(&finish);
	total_time = double(finish.tms_utime - start.tms_utime + finish.tms_stime - start.tms_stime)/sysconf(_SC_CLK_TCK);
	//total_time = round(total_time * 100) / 100.0;
	return total_time;
}

int makenumber(string str) {
	int res = 0;
	for(int i = 0; i < str.length(); i++) res = res * 10 + str[i] - '0';
	return res;
}

void mRead(string filename) {
	string tempstr1;
	string tempstr2;
	bool first;
	//cout << filename << endl;
	fp = freopen(filename.c_str(), "r", stdin); 
	//cout << ftell(fp) << endl;


	int index = 0;
	int** vertex;
	
	cin >> order;
	cin >> is_sat;
	group_size = order*order;
	vertex_size = group_size*group_size;
	edge_size = group_size*group_size*3*(group_size-1)/2;
	set_Variable_size();
	init();
	vertex = (int **) malloc(sizeof(int *) * (group_size+2));
	for (int i=1; i<=group_size; i++) vertex[i] = (int *) malloc(sizeof(int) * (group_size+2));
	for (int i=1; i<=group_size; i++) {
		for (int j=1; j<=group_size; j++) {
			cin >> vertex[i][j];
			int x = to_index(i, j, true);
			// cout<<"index: "<<x<<" ["<<i<<", "<<j<<"] = "<<vertex[i][j]<<endl;
			if (vertex[i][j] == -1) {
				for(int k=1; k<=group_size; k++) {
					vertex_color[x][vertex_color_length[x]] = k; 
					color_vertex_pos[x][k] = vertex_color_length[x]++;
					vertex_can_move[x][k] = true;
				}
			} else {
				vertex_color[x][vertex_color_length[x]] = vertex[i][j]; 
				color_vertex_pos[x][vertex[i][j]] = vertex_color_length[x]++;
				vertex_can_move[x][vertex[i][j]] = true;
			}
		}
	}
	for (int row=1; row<=group_size; row++) {
		for (int col1=1; col1<group_size; col1++) {
			for (int col2=col1+1; col2<=group_size; col2++) {
				int x = to_index(row, col1, true);
				int y = to_index(row, col2, true);
				Neighbour[x][Neighbour_length[x]++] = y;
				Neighbour[y][Neighbour_length[y]++] = x;
				edge[++index].x = x;
				edge[index].y = y;
				G[x][y] = index;
				G[y][x] = index;
			}
		}
	}
	for (int col=1; col<=group_size; col++) {
		for (int row1=1; row1<group_size; row1++) {
			for (int row2=row1+1; row2<=group_size; row2++) {
				int x = to_index(row1, col, true);
				int y = to_index(row2, col, true);
				Neighbour[x][Neighbour_length[x]++] = y;
				Neighbour[y][Neighbour_length[y]++] = x;
				edge[++index].x = x;
				edge[index].y = y;
				G[x][y] = index;
				G[y][x] = index;
			}
		}
	}
	for (int group=1; group<=group_size; group++) {
		for (int point1=1; point1<group_size; point1++) {
			for (int point2=point1+1; point2<=group_size; point2++) {
				int x = to_index(group, point1, false);
				int y = to_index(group, point2, false);
				Neighbour[x][Neighbour_length[x]++] = y;
				Neighbour[y][Neighbour_length[y]++] = x;
				edge[++index].x = x;
				edge[index].y = y;
				G[x][y] = index;
				G[y][x] = index;
			}
		}
	}
	// cout<<index<<endl;
	return;
}

void print_current() {
	for(int i = 1; i <= square_length; i++) {
		for(int j = 0; j < mColorsLength[i]; j++) {
			ans[mColors[i][j]] = i;
		}
	}
	for(int i = 0; i <= square_length; i++) {
		for(int j = 0; j <= square_length; j++) {
			if(i == 0) {
				if(j == 0) cout << "   ";
				else {
					if(j > 9) cout << " " << j;
					else cout << "  " << j;
				}
			}
			else {
				if(j == 0) {
					if(i > 9) cout << " " << i;
					else cout << "  " << i;
				}
				else {
					int id = square_length * (i - 1) + j;
					if(mVertexesColor[id] > 9) cout << " " << mVertexesColor[id];
					else cout << "  " << mVertexesColor[id];
				}
			}
		}
		cout << endl;
	}
	return;
}

void print_ans() {
	string outResult;
	string ofilename;
	string write;
	if(seed == 1) write = "w";
	else write = "a";
	const size_t found = filename.rfind('.');
	const size_t foundf = filename.rfind('/');
	if(std::string::npos != found) outResult = filename.substr(0, found);
	if(std::string::npos != foundf) ofilename = filename.substr(foundf + 1);
	if(total_clash == 0) {
		fp = freopen(outPut.c_str(), "a", stdout);
		//cout << ofilename << " Find solution " << best_time << seed << endl;
		printf("%s 1 %lf %d\n", ofilename.c_str(), best_time, seed);
		/*freopen(outResult.c_str(), write.c_str(), stdout);
		  cout << seed << endl;
		  for(int i = 1; i <= vertex_size; i++) {
		  cout << mVertexesColor[i] << " ";
		  if(i % square_length == 0) cout << endl;
		  }*/
	}
	else {
		fp = freopen(outPut.c_str(), "a", stdout);
		//cout << ofilename << " Not Found! " << best_clash << " " << seed << endl;
		printf("%s 0 %d %d\n", ofilename.c_str(), best_clash, seed);
		//freopen(outResult.c_str(), write.c_str(), stdout);
		//cout << seed << "No solution" << endl;
	}
	/*outResult = ofilename + "res.txt";
	fp = freopen(outResult.c_str(), "w", stdout);
	print_current();
	fp = freopen("/dev/tyy", "w", stdout);*/
	return;
}

void check_vertex_clash() {
	bool cvc_flag = true;
	int tmp_x, tmp_y;
	for(int i = 1; i <= vertex_size; i++) {
		for(int j = 1; j <= square_length; j++) {
			tmp_clash[i][j] = 0;
		}
	}
	for(int i = 1; i <= edge_size; i++) {
		tmp_x = edge[i].x;
		tmp_y = edge[i].y;
		if(mVertexesPosition[tmp_x] == -1 || mVertexesPosition[tmp_y] == -1) continue;
		tmp_clash[tmp_x][mVertexesColor[tmp_y]] += 1;
		tmp_clash[tmp_y][mVertexesColor[tmp_x]] += 1;
	}
	for(int i = 0; i < vertex_use_length; i++) {
		for(int j = 1; j <= square_length; j++) {
			if(mClash[mVertexes[i]][j] != tmp_clash[mVertexes[i]][j]) {
				cout << "vertex_clash edge wrong: " << mVertexes[i] << " " << j << " " << mClash[mVertexes[i]][j] << " " << tmp_clash[mVertexes[i]][j] << endl;
				cvc_flag = false;
			}
		}
	}
	if(!cvc_flag) exit(0);
	return;
}

void check_clash() {
	int res = 0;
	for(int i = 1; i <= edge_size; i++) {
		if(mVertexesPosition[edge[i].x] == -1 || mVertexesPosition[edge[i].y] == -1) continue;
		if(mVertexesColor[edge[i].x] == mVertexesColor[edge[i].y]) res++;
	}
	if(res != total_clash) {
		cout << "total_clash wrong" << endl;
		cout << res << " " << total_clash << endl;
		exit(0);
	}
	return;
}

void check_CL(int* tmp_arr) {
	int cur = tmp_arr[mVertexes[1]];
	for(int i = 2; i <= vertex_size; i++) {
		int tmp_v = mVertexes[i];
		if(cur != tmp_arr[tmp_v]) {
			if(CL_start_pos[tmp_arr[tmp_v]] != i) {
				cout << "check_CL false: " << tmp_v << " " << tmp_arr[tmp_v] << " " << i << " " << CL_start_pos[tmp_arr[tmp_v]] << endl;
				exit(0);
			}
			cur = tmp_arr[tmp_v];
		}
	}
	return;
}

void check_color_vertex() {
	bool flag = true;
	int val[square_length + 2];
	for(int i = 1; i <= square_length; i++) {
		memset(val, 0, sizeof(val));
		for(int j = 1; j <= square_length; j++) {
			int id = square_length * (i - 1) + j;
			for(int p = 0; p < vertex_color_length[id]; p++) {
				val[vertex_color[id][p]]++;
			}
		}
		for(int j = 1; j <= square_length; j++) {
			if(mCol_length[i][j] != val[j]) {
				flag = false;
				cout << "mCol wrong: " << i << " " << j << " " << mCol_length[i][j] << " " << val[j] << endl;
				for(int p = 1; p <= val[j]; p++) {
					cout << mCol[i][j][p] << " ";
				}
				cout << endl;
			}
		}
	}
	if(!flag) exit(0);
	return;
}

void check_answer() {
	bool flag = true;
	for(int i = 1; i <= edge_size; i++) {
		if(mVertexesColor[edge[i].x] == mVertexesColor[edge[i].y]) {
			cout << "answer wrong same color " << edge[i].x << " " << edge[i].y << " " << mVertexesColor[edge[i].x] << " " << mVertexesColor[edge[i].y] << endl;
			flag = false;
		}
	}
	for(int i = 1; i <= vertex_size; i++) {
		if(mVertexesColor[i] == 0) {
			cout << "answer wrong no color: " << i << " " << mVertexesColor[i] << endl;
			flag = false;
		}
		if(vertex_color_length[i] != 0 && !vertex_can_move[i][mVertexesColor[i]]) {
			cout << "answer wrong position wrong " << i << " " << mVertexesColor[i] << endl;
			for(int j = 1; j <= square_length; j++) {
				if(vertex_can_move[i][j]) cout << j << " ";
			}
			cout << endl;
			flag = false;
		}
	}
	if(!flag) exit(0);
	return;
}

void check_vertex_clash_length() {
	memset(tmp_bool, false, sizeof(bool) * (vertex_size + 1));
	int cnt = 0;
	for(int i = 0; i < vertex_use_length; i++) {
		int v_id = mVertexes[i];
		if(mClash[v_id][mVertexesColor[v_id]] > 0) cnt++;
	}
	if(cnt != vertex_clash_length) {
		cout << "check_vertex_clash_length wrong " << cnt << " " << vertex_clash_length << endl;
		exit(0);
	}
	return;
}

bool check_finish() {
	//cout << "check_finish " << best_clash << endl; 
	if(total_clash == 0) {
		mCurrentTime();
		best_time = total_time;
		check_answer();
		//cout << filename << " find answer: " << best_time << endl;
		printf("%s find answer: %lf\n", filename.c_str(), best_time);
		return true;
	}
	return false;
}

bool check_tabu(int id, int to, int iter) {
	if(tabu[id][to] <= iter) return true;
	return false;
}

bool check_tabuC(int id, int to) {
	if(tabuC[id][to]) {
		for(int i = 0; i < tabuCVlen[id][to]; i++) {
			if(mVertexesColor[tabuCV[id][to][i]] == to) return true;
		}
	}
	return false;
}
void mReduceVertexes() {
	pppp_cnt = 0;
	// cout << "mReduceVertexes start" << endl;
	for(int i = 1; i <= vertex_size; i++) if(vertex_color_length[i] == 1) pppp_cnt++;
	// cout<<pppp_cnt<<endl;
	// 固定点处理
	for (int i=1; i<=vertex_size; i++) {
		if (vertex_color_length[i] == 1) {
			int tmp_c = vertex_color[i][0];
			vertex_color_length[i] = 0;
			mVertexesColor[i] = tmp_c;
			vertex_color_pos[i][tmp_c] = mColorsLength[tmp_c];
			mColors[tmp_c][mColorsLength[tmp_c]++] = i;
			vertex_can_move[i][tmp_c] = false;
			// i不能再选择其他颜色，因此i所在的行、列、组的其他颜色的可选点长度都要减一
			for (int j=1; j<=square_length; j++) {
				if(mCol_pos[get_Col(i)][j][i] <= mCol_length[get_Col(i)][j]) swap_col(get_Col(i), j, i, mCol_length[get_Col(i)][j]--);
				if(mRow_pos[get_Row(i)][j][i] <= mRow_length[get_Row(i)][j]) swap_row(get_Row(i), j, i, mRow_length[get_Row(i)][j]--);
				if(mGroup_pos[get_Group(i)][j][i] <= mGroup_length[get_Group(i)][j]) swap_group(get_Group(i), j, i, mGroup_length[get_Group(i)][j]--);
			}
			// i所在行、列、组的tmp_c无法再被其他点选择
			swap_col(get_Col(i), tmp_c, i, 1);
			mCol_length[get_Col(i)][tmp_c] = 0;
			swap_row(get_Row(i), tmp_c, i, 1);
			mRow_length[get_Row(i)][tmp_c] = 0;
			swap_group(get_Group(i), tmp_c, i, 1);
			mGroup_length[get_Group(i)][tmp_c] = 0;
			for(int j = 0; j < Neighbour_length[i]; j++) {
				int tmp_V = Neighbour[i][j];
				// 删除i点的邻居的对应tmp_c
				if(color_vertex_pos[tmp_V][tmp_c] < vertex_color_length[tmp_V])
					swap_vertex_color(tmp_V, tmp_c, --vertex_color_length[tmp_V]);
				// i点的邻居不能再选择tmp_c
				if(mCol_pos[get_Col(tmp_V)][tmp_c][tmp_V] <= mCol_length[get_Col(tmp_V)][tmp_c])
					swap_col(get_Col(tmp_V), tmp_c, tmp_V, mCol_length[get_Col(tmp_V)][tmp_c]--);
				if(mRow_pos[get_Row(tmp_V)][tmp_c][tmp_V] <= mRow_length[get_Row(tmp_V)][tmp_c])
					swap_row(get_Row(tmp_V), tmp_c, tmp_V, mRow_length[get_Row(tmp_V)][tmp_c]--);
				if(mGroup_pos[get_Group(tmp_V)][tmp_c][tmp_V] <= mGroup_length[get_Group(tmp_V)][tmp_c])
					swap_group(get_Group(tmp_V), tmp_c, tmp_V, mGroup_length[get_Group(tmp_V)][tmp_c]--);
			}
		}
	}
	queue<St> q;
	St st;
	// 只能选择一个颜色的点
	for(int i = 1; i <= vertex_size; i++) {
		if(vertex_color_length[i] == 1) {
			st.id = i;
			st.c = vertex_color[i][0];
			//cout << "vcl: " << st.id << " " << st.c << endl;
			q.push(st);
		}
	}
	// 只有一个点可选的颜色
	for(int i = 1; i <= square_length; i++) {
		for(int j = 1; j <= square_length; j++) {
			if(mCol_length[i][j] == 1) {
				st.id = mCol[i][j][1];
				st.c = j;
				//cout << "Col: " << st.id << " " << st.c << endl;
				q.push(st);
			}	
			if(mRow_length[i][j] == 1) {
				st.id = mRow[i][j][1];
				st.c = j;
				//cout << "Row: " << st.id << " " << st.c << endl;
				q.push(st);
			}
			if(mGroup_length[i][j] == 1) {
				st.id = mGroup[i][j][1];
				st.c = j;
				//cout << "Row: " << st.id << " " << st.c << endl;
				q.push(st);
			}
		}
	}
	int cnt = 0;
	while(!q.empty()) {
		st = q.front();
		q.pop();
		int tmp_v = st.id;
		int tmp_c = st.c;
		if(mVertexesColor[tmp_v] > 0) continue;
		swap_vertex_color(tmp_v, tmp_c, 0);
		vertex_color_length[tmp_v] = 0;
		mColors[tmp_c][mColorsLength[tmp_c]] = tmp_v;
		vertex_color_pos[tmp_v][tmp_c] = mColorsLength[tmp_c]++;
		// 此时才开始将tmp_v赋值为tmp_c
		mVertexesColor[tmp_v] = tmp_c;
		vertex_can_move[tmp_v][tmp_c] = false;
		swap_col(get_Col(tmp_v), tmp_c, tmp_v, 1);
		mCol_length[get_Col(tmp_v)][tmp_c] = 0;
		swap_row(get_Row(tmp_v), tmp_c, tmp_v, 1);
		mRow_length[get_Row(tmp_v)][tmp_c] = 0;
		swap_group(get_Group(tmp_v), tmp_c, tmp_v, 1);
		mGroup_length[get_Group(tmp_v)][tmp_c] = 0;
		// 传播， 颜色
		for(int i = 1; i <= square_length; i++) {
			if(mCol_pos[get_Col(tmp_v)][i][tmp_v] <= mCol_length[get_Col(tmp_v)][i]) {
				swap_col(get_Col(tmp_v), i, tmp_v, mCol_length[get_Col(tmp_v)][i]--);
				if(mCol_length[get_Col(tmp_v)][i] == 1) {
					st.id = mCol[get_Col(tmp_v)][i][1];
					st.c = i;
					q.push(st);
				}
			}
			if(mRow_pos[get_Row(tmp_v)][i][tmp_v] <= mRow_length[get_Row(tmp_v)][i]) {
				swap_row(get_Row(tmp_v), i, tmp_v, mRow_length[get_Row(tmp_v)][i]--);
				if(mRow_length[get_Row(tmp_v)][i] == 1) {
					st.id = mRow[get_Row(tmp_v)][i][1];
					st.c = i;
					q.push(st);
				}
			}
			if(mGroup_pos[get_Group(tmp_v)][i][tmp_v] <= mGroup_length[get_Group(tmp_v)][i]) {
				swap_group(get_Group(tmp_v), i, tmp_v, mGroup_length[get_Group(tmp_v)][i]--);
				if(mGroup_length[get_Group(tmp_v)][i] == 1) {
					st.id = mGroup[get_Group(tmp_v)][i][1];
					st.c = i;
					q.push(st);
				}
			}
		}
		for(int i = 1; i <= square_length; i++) if(color_vp[i][tmp_v] < color_vertex_length[i]) swap_color_vertex(i, tmp_v, --color_vertex_length[i]);
		//cout << tmp_v << " " << tmp_c << endl;
		// 传播， 邻居
		for(int i = 0; i < Neighbour_length[tmp_v]; i++) {
			int tmp_V = Neighbour[tmp_v][i];
			if(color_vertex_pos[tmp_V][tmp_c] < vertex_color_length[tmp_V]) {
				swap_vertex_color(tmp_V, tmp_c, vertex_color_length[tmp_V] - 1);
				vertex_color_length[tmp_V]--;
				if(vertex_color_length[tmp_V] == 1) {
					st.id = tmp_V;
					st.c = vertex_color[tmp_V][0];
					//cout << "add vcl: " << st.id << " " << st.c << endl;
					q.push(st);
				}
			}
			if(color_vp[tmp_c][tmp_V] < color_vertex_length[tmp_c]) {
				swap_color_vertex(tmp_c, tmp_V, --color_vertex_length[tmp_c]);
			}
			if(mCol_pos[get_Col(tmp_V)][tmp_c][tmp_V] <= mCol_length[get_Col(tmp_V)][tmp_c]) {
				if(mCol_length[get_Col(tmp_V)][tmp_c] != 0) swap_col(get_Col(tmp_V), tmp_c, tmp_V, mCol_length[get_Col(tmp_V)][tmp_c]--);
				if(mCol_length[get_Col(tmp_V)][tmp_c] == 1) {
					st.id = mCol[get_Col(tmp_V)][tmp_c][1];
					st.c = tmp_c;
					//cout << "add col: " << st.id << " " << st.c << endl;
					q.push(st);
				}
			}
			if(mRow_pos[get_Row(tmp_V)][tmp_c][tmp_V] <= mRow_length[get_Row(tmp_V)][tmp_c]) {
				if(mRow_length[get_Row(tmp_V)][tmp_c] != 0) swap_row(get_Row(tmp_V), tmp_c, tmp_V, mRow_length[get_Row(tmp_V)][tmp_c]--);
				if(mRow_length[get_Row(tmp_V)][tmp_c] == 1) {
					st.id = mRow[get_Row(tmp_V)][tmp_c][1];
					st.c = tmp_c;
					//cout << "add row: " << st.id << " " << st.c << endl;
					q.push(st); 
				}
			}
			if(mGroup_pos[get_Group(tmp_V)][tmp_c][tmp_V] <= mGroup_length[get_Group(tmp_V)][tmp_c]) {
				if(mGroup_length[get_Group(tmp_V)][tmp_c] != 0) swap_group(get_Group(tmp_V), tmp_c, tmp_V, mGroup_length[get_Group(tmp_V)][tmp_c]--);
				if(mGroup_length[get_Group(tmp_V)][tmp_c] == 1) {
					st.id = mGroup[get_Group(tmp_V)][tmp_c][1];
					st.c = tmp_c;
					//cout << "add row: " << st.id << " " << st.c << endl;
					q.push(st); 
				}
			}
			vertex_can_move[tmp_V][tmp_c] = false;
		}
	}
	vertex_use_length = 0;
	vertex_use_length_tmp = 0;
	edge_use_length = 0;
	for(int i = 1; i <= vertex_size; i++) {
		// 更新颜色可选长度
		if(mVertexesColor[i] > 0) color_max_size[mVertexesColor[i]]--;
		else {
			mVertexes[vertex_use_length] = i;
			mVertexesPosition[i] = vertex_use_length++;
		}
	}
	for(int i = 0; i < vertex_use_length; i++) {
		int vertex_id = mVertexes[i];
		// 更新邻居信息，将已赋值的点从邻居中删除
		for(int j = 0; j < Neighbour_length[vertex_id]; j++) {
			int tmp_v = Neighbour[vertex_id][j];
			if(mVertexesColor[tmp_v] > 0) {
				Neighbour[vertex_id][j] = Neighbour[vertex_id][--Neighbour_length[vertex_id]];
				j--;
			}
			// 更新边信息，将没用的边删除
			else {
				int edge_id = G[vertex_id][tmp_v];
				if(mEdgesPosition[edge_id] > edge_use_length) swap_edge(edge_id, ++edge_use_length);
			}
		}
	}
	// cout << "Vertex left: " << vertex_use_length << endl; 
	// cout << "Vertex reduce: " << vertex_size - vertex_use_length << endl;
	// print_current();
	return; 
}

void build() {
	total_clash = 0;
	update_Iter = mPopulationMaxVOld[mPopulation_id];
	// 初始化
	for(int i = 0; i < vertex_use_length; i++) {
		score[mVertexes[i]] = 0;
		for(int j = 1; j <= square_length; j++) {
			mClash[mVertexes[i]][j] = 0;
			tabu[mVertexes[i]][j] = 0;
		}
	}
	edge_clash_length = 0;
	// 计算总冲突，更新边的位置
	for(int i = 1; i <= edge_use_length; i++) {
		mClash[edge[mEdges[i]].x][mVertexesColor[edge[mEdges[i]].y]] += 1;
		mClash[edge[mEdges[i]].y][mVertexesColor[edge[mEdges[i]].x]] += 1;
		if(mVertexesColor[edge[mEdges[i]].x] == mVertexesColor[edge[mEdges[i]].y]) {
			total_clash += 1;
			swap_edge(mEdges[i], ++edge_clash_length);
		}
	}
	for(int i = 0; i < vertex_use_length; i++) {
		ClashVertexesPos[mVertexes[i]] = -1;
		vertex_old[mVertexes[i]][0] = mPopulationVertexOld[mPopulation_id][mVertexes[i]];
	}
	vertex_clash_length = 0;
	move_vertex_size = 0;
	// 计算存在冲突的点
	for(int i = 0; i < vertex_use_length; i++) {
		int vertex_id = mVertexes[i];
		if(mClash[vertex_id][mVertexesColor[vertex_id]] > 0) {
			ClashVertexes[vertex_clash_length] = vertex_id;
			ClashVertexesPos[vertex_id] = vertex_clash_length++;
		}
	}
	//check_move_vertex();
	cout << total_clash <<endl;
	return;
}

void increase_score() {
	for(int i = 1; i <= edge_clash_length; i++) {
		score[edge[mEdges[i]].x]++;
		score[edge[mEdges[i]].y]++;
	}
	return;
}

//TODO: main function
int tabuSearch() {
	int tSminClash;
	//cout << "before build" << endl;
	build();
	tSminClash = total_clash;
	//cout << "init: " << total_clash << endl;
	if(total_clash == 0) return tSminClash;
	for(int i = 0; i < vertex_use_length; i++) mVertexesColorTmp[mVertexes[i]] = mVertexesColor[mVertexes[i]];
	int min_sub_clash;
	int min_sub_tabu_clash;
	long long min_sub_cs;
	long long min_sub_tabu_cs;
	int cnt;
	int tabu_cnt;
	int no_improve_iters = 0;
	int move_id;
	int move_to;
	int total_cnt;
	long long total_iters = mPopulationMaxVOld[mPopulation_id] + max_iter;
	//long long total_iters = mPopulationMaxVOld[mPopulation_id] + INT_MAX;
	for(long long iters = mPopulationMaxVOld[mPopulation_id] + 1; iters < total_iters; iters++) {
		//cout << iters << " " << total_clash << " " << tSminClash << " " << vertex_clash_length << " " << population_cnt << " " << move_vertex_size << endl;
		min_sub_clash = INT_MAX;
		min_sub_tabu_clash = INT_MAX;
		//min_sub_clash = 1;
		//min_sub_tabu_clash = 1;
		cnt = 0;
		tabu_cnt = 0;
		total_cnt = 0;
		for(int i = 0; i < vertex_clash_length; i++) {
			int vertex_id = ClashVertexes[i];
			for(int j = 0; j < vertex_color_length[vertex_id]; j++) {
				int tmp_c = vertex_color[vertex_id][j];
				if(tmp_c == mVertexesColor[vertex_id]) continue;
				int tmp_s = get_score(vertex_id, tmp_c);
				//long long tmp_cs = 0;
				if(check_tabu(vertex_id, tmp_c, iters)) {
					if(tmp_s < min_sub_clash) {
						cnt = 0;
						tmp_st[cnt].id = vertex_id;
						tmp_st[cnt].c = tmp_c;
						tmp_st[cnt].score = tmp_s;
						min_sub_clash = tmp_s;
						cnt++;
					}
					else if(tmp_s == min_sub_clash) {
						tmp_st[cnt].id = vertex_id;
						tmp_st[cnt].c = tmp_c;
						tmp_st[cnt].score = tmp_s;
						cnt++;						
					}
				}
				else {
					if(tmp_s < min_sub_tabu_clash) {
						tabu_cnt = 0;
						tmp_tabu_st[tabu_cnt].id = vertex_id;
						tmp_tabu_st[tabu_cnt].c = tmp_c;
						tmp_tabu_st[tabu_cnt].score = tmp_s;
						min_sub_tabu_clash = tmp_s;
						tabu_cnt++;
					}
					else if(tmp_s == min_sub_tabu_clash) {
						tmp_tabu_st[tabu_cnt].id = vertex_id;
						tmp_tabu_st[tabu_cnt].c = tmp_c;
						tmp_tabu_st[tabu_cnt].score = tmp_s;
						tabu_cnt++;
					}
				}
			}
		}
		//check_move_vertex();
		//cout << total_cnt << " " << cnt << " " << min_sub_clash << " " << tabu_cnt << " " << min_sub_tabu_clash << endl;
		total_all++;
		if(cnt == 0 && tabu_cnt == 0) {
			return tSminClash;
		}
		else if((tabu_cnt > 0 && min_sub_tabu_clash < min_sub_clash && (total_clash + min_sub_tabu_clash < tSminClash)) || (cnt == 0 && tabu_cnt > 0)) {
			int r;
			min_sub_tabu_cs = LLONG_MIN;
			int cs_cnt = 0;
			int s_cnt = min(3, tabu_cnt);
			for(int i = 0; i < s_cnt; i++) {
				r = rand() % tabu_cnt;
				long long tmp_cs = score[tmp_tabu_st[r].id];
				if(min_sub_tabu_cs < tmp_cs) {
					cs_cnt == 0;
					min_sub_tabu_cs = tmp_cs;
					bms_array[cs_cnt++] = tmp_tabu_st[r];
				}	
				else if(min_sub_tabu_cs == tmp_cs) bms_array[cs_cnt++] = tmp_tabu_st[r];
			}
			r = rand() % cs_cnt;
			move_id = bms_array[r].id;
			move_to = bms_array[r].c;
			int old_c = mVertexesColor[move_id];
			tabu[move_id][old_c] = iters + rand() % tabuStep + alpha * vertex_clash_length;
			add_vertex(move_id, move_to);
			total_clash += min_sub_tabu_clash;
			if(vertex_old[move_id][1] <= update_Iter) vertex_old[move_id][0] = vertex_old[move_id][1];
			vertex_old[move_id][1] = iters;
			increase_score();
			score[move_id] = 0;
		}
		else {
			int r;
			min_sub_cs = LLONG_MIN;
			int cs_cnt = 0;
			int s_cnt = min(3, cnt);
			for(int i = 0; i < s_cnt; i++) {
				r = rand() % cnt;
				long long tmp_cs = score[tmp_st[r].id];
				if(min_sub_cs < tmp_cs) {
					cs_cnt == 0;
					min_sub_cs = tmp_cs;
					bms_array[cs_cnt++] = tmp_st[r];
				}	
				else if(min_sub_cs == tmp_cs) bms_array[cs_cnt++] = tmp_st[r];
			}
			r = rand() % cs_cnt;
			move_id = bms_array[r].id;
			move_to = bms_array[r].c;
			int old_c = mVertexesColor[move_id];
			tabu[move_id][old_c] = iters + rand() % tabuStep + alpha * vertex_clash_length;
			add_vertex(move_id, move_to);
			total_clash += min_sub_clash;
			if(vertex_old[move_id][1] <= update_Iter) vertex_old[move_id][0] = vertex_old[move_id][1];
			vertex_old[move_id][1] = iters;
			increase_score();
			score[move_id] = 0;
		}
		//cout << total_all << " " << total_zero << endl;
		if(total_clash <= tSminClash) {
			tSminClash = total_clash;
			update_Iter = iters;
			for(int i = 0; i < vertex_use_length; i++) {
				mVertexesColorTmp[mVertexes[i]] = mVertexesColor[mVertexes[i]];
			}
			for(int i = 1; i <= edge_clash_length; i++) best_clash_array[i - 1].e_id = mEdges[i];
			for(int i = 1; i <= edge_clash_length; i++) best_clash_array[i - 1].color = mVertexesColor[edge[mEdges[i]].x];
			if(tSminClash == 0) return tSminClash;
			no_improve_iters = 0;
		}
		else no_improve_iters++;
	}
	return tSminClash;
}

bool LocalSearch() {
	local_min_clash = INT_MAX;
	int tabu_clash = tabuSearch();
	cout << tabu_clash << " " << best_clash << endl;
	if(tabu_clash == 0) return true;
	if(tabu_clash < local_min_clash) {
		local_min_clash = tabu_clash;
		if(local_min_clash < best_clash) {
			best_clash = local_min_clash;
			no_improve_steps = 1;
		}
	}
	return false;
}

//
void mGenerate() {
	for(int i = 0; i < vertex_use_length; i++) {
		int vertex_id = mVertexes[i];
		int r = rand() % vertex_color_length[vertex_id];
		mVertexesColor[vertex_id] = vertex_color[vertex_id][r];
	}
	print_current();
	if(LocalSearch()) return; 
	return;
}

void mCopy() {
	if(!mPopulationFlag) mPopulation_id = rand() % population_cnt;
	//cout << "mPopulation_id: " << mPopulation_id << endl;
	for(int i = 0; i < vertex_use_length; i++) {
		mVertexesColor[mVertexes[i]] = mPopulation[mPopulation_id][mVertexes[i]];
	}
	for(int i = 0; i < best_clash; i++) {
		best_clash_array[i].e_id = mPopulationClash[mPopulation_id][i];
		best_clash_array[i].color = mPopulationClashColor[mPopulation_id][i];
	}
	return;
}

void CleanTabuC() {
	for(int i = 0; i < mPopulationClashLen; i++) {
		int e_id = tabuE[i];
		tabuC[edge[e_id].x][tabuEC[i]] = false;
		tabuC[edge[e_id].y][tabuEC[i]] = false;
	}
	return;
}

void SetTabuC() {
	for(int i = 0; i < mPopulationClashLen; i++) tabuE[i] = mPopulationClash[mPopulation_id][i];
	for(int i = 0; i < mPopulationClashLen; i++) tabuEC[i] = mPopulationClashColor[mPopulation_id][i];
	for(int i = 0; i < mPopulationClashLen; i++) {
		tabuCVlen[edge[tabuE[i]].x][tabuEC[i]] = 0;
		tabuCVlen[edge[tabuE[i]].y][tabuEC[i]] = 0;
	}
	for(int i = 0; i < mPopulationClashLen; i++) {
		//cout << edge[tabuE[i]].x << " " << edge[tabuE[i]].y << " " << tabuEC[i] << endl;
		tabuC[edge[tabuE[i]].x][tabuEC[i]] = true;
		tabuCV[edge[tabuE[i]].x][tabuEC[i]][tabuCVlen[edge[tabuE[i]].x][tabuEC[i]]++] = edge[tabuE[i]].y;
		tabuC[edge[tabuE[i]].y][tabuEC[i]] = true;
		tabuCV[edge[tabuE[i]].y][tabuEC[i]][tabuCVlen[edge[tabuE[i]].y][tabuEC[i]]++] = edge[tabuE[i]].x;
	}
	return;
}

void mPopulationUpdate() {
	mPopulationFlag = false;
	sort(best_clash_array, best_clash_array + best_clash);
	if(best_clash < mPopulationClashLen) {
		mPopulationClashLen = best_clash;
		population_cnt = 0;
	}
	bool flag = false;
	for(int i = 0; i < population_cnt; i++) {
		flag = true;
		for(int j = 0; j < best_clash; j++) {
			if((mPopulationClash[i][j] == best_clash_array[j].e_id && mPopulationClashColor[i][j] != best_clash_array[j].color) || mPopulationClash[i][j] != best_clash_array[j].e_id) {
				flag = false;
				break;
			}
		}
		if(flag) {
			for(int j = 0; j < best_clash; j++) {
				mPopulationClash[i][j] = best_clash_array[j].e_id;
				mPopulationClashColor[i][j] = best_clash_array[j].color;
			}
			for(int j = 0; j < vertex_use_length; j++) {
				mPopulation[i][mVertexes[j]] = mVertexesColorTmp[mVertexes[j]];
				if(vertex_old[mVertexes[j]][1] <= update_Iter) mPopulationVertexOld[i][mVertexes[j]] = vertex_old[mVertexes[j]][1];
				else mPopulationVertexOld[i][mVertexes[j]] = vertex_old[mVertexes[j]][0];
			}
			mPopulationMaxVOld[i] = update_Iter;
			mPopulationOld[i] = Iter;
			break;
		}
	}
	if(!flag) {
		if(population_cnt != population_size) {
			for(int i = 0; i < best_clash; i++) {
				mPopulationClash[population_cnt][i] = best_clash_array[i].e_id;
				mPopulationClashColor[population_cnt][i] = best_clash_array[i].color;
			}
			for(int i = 0; i < vertex_use_length; i++) {
				mPopulation[population_cnt][mVertexes[i]] = mVertexesColorTmp[mVertexes[i]];
				if(vertex_old[mVertexes[i]][1] <= update_Iter) mPopulationVertexOld[population_cnt][mVertexes[i]] = vertex_old[mVertexes[i]][1];
				else mPopulationVertexOld[population_cnt][mVertexes[i]] = vertex_old[mVertexes[i]][0];
			}
			mPopulationMaxVOld[population_cnt] = update_Iter;
			mPopulationOld[population_cnt] = Iter;
			population_cnt++;
		}
		else {
			int p_id;
			int minOld = INT_MAX;
			for(int i = 0; i < population_cnt; i++) {
				if(mPopulationOld[i] < minOld) {
					minOld = mPopulationOld[i];
					p_id = i;
				}
			}
			for(int i = 0; i < best_clash; i++) {
				mPopulationClash[p_id][i] = best_clash_array[i].e_id;
				mPopulationClashColor[p_id][i] = best_clash_array[i].color;
			}
			for(int i = 0; i < vertex_use_length; i++) {
				mPopulation[p_id][mVertexes[i]] = mVertexesColorTmp[mVertexes[i]];
				if(vertex_old[mVertexes[i]][1] <= update_Iter) mPopulationVertexOld[p_id][mVertexes[i]] = vertex_old[mVertexes[i]][1];
				else mPopulationVertexOld[p_id][mVertexes[i]] = vertex_old[mVertexes[i]][0];
			}
			mPopulationMaxVOld[p_id] = update_Iter;
			mPopulationOld[p_id] = Iter;
			mPopulationFlag = true;
			mPopulation_id = p_id;
		}
	}
	//for(int i = 0; i < vertex_use_length; i++) cout << mVertexes[i] << " " << vertex_color_length[mVertexes[i]] << " " << mPopulationVertexOld[0][mVertexes[i]] << " " << vertex_old[mVertexes[i]][0] << " " << vertex_old[mVertexes[i]][1] << " " << update_Iter << endl;
	return;
}

void mPerturbation() {
	long long max_old = LLONG_MIN;
	long long min_old = LLONG_MAX;
	for(int i = 0; i < vertex_use_length; i++) {
		min_old = min(min_old, mPopulationVertexOld[mPopulation_id][mVertexes[i]]);
	}
	max_old = mPopulationMaxVOld[mPopulation_id];
	max_old = min_old + (max_old - min_old) / 5;
	int cnt = 0;
	for(int i = 0; i < vertex_use_length; i++) {
		if(mPopulationVertexOld[mPopulation_id][mVertexes[i]] >= min_old && mPopulationVertexOld[mPopulation_id][mVertexes[i]] <= max_old) {
			tmp_array[cnt++] = mVertexes[i];
			//cout << mVertexes[i] << " " << mPopulationVertexOld[0][mVertexes[i]] << " " << max_old << endl;
		}
	}
	//cout << cnt << " " << max_old << " " << min_old << endl;
	int p_cnt = cnt / 2;
	p_cnt = min(p_cnt, cnt);
	for(int i = 0; i < p_cnt; i++) {
		int r = rand() % cnt;
		mVertexesColor[tmp_array[r]] = vertex_color[tmp_array[r]][rand() % vertex_color_length[tmp_array[r]]];
		swap(tmp_array[r], tmp_array[--cnt]);
	}
	return;
}

int main(int argc, char* argv[]) {
	outPut = argv[0];
	outPut = outPut.substr(2, outPut.length() - 2);
	outPut += "Result.txt";
	best_clash = INT_MAX;
	// filename = argv[1];
	// seed = atoi(argv[2]);
	// time_limit = atof(argv[3]);
	filename = "test.txt";
	seed = 1;
	time_limit = 10;
	srand(seed);
	total_all = 0;
	total_zero = 0;
	// cout << filename << endl;
	mRead(filename);
	// cout << "Read file finish" << endl;
	mReduceVertexes(); 
	mStartTime();
	//cout << "Reduce vertexes finish" << endl;
	mGenerate();
	// cout << "mGenerate finish" << endl;
	// if(check_finish()) {
	// 	print_ans();
	// 	return 0;
	// }
	// steps = 1;
	// no_improve_steps = 1;
	// total_Iters = 10000;
	// bool tabuCFlag;
	// for(Iter = 0; total_time < time_limit; Iter++) {
	// 	mPopulationUpdate();
	// 	mCopy();
	// 	mPerturbation();
	// 	LocalSearch();
	// 	if(check_finish()) break;
	// 	mCurrentTime();
	// }
	// print_ans();
	return 0;
}

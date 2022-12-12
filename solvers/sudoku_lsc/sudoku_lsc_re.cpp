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
	int w;
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
		
const int tabuStep = 10;
const int maxLength = 3;
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
int is_sat;
int vertex_size;
int edge_size;		
int square_size;

int** mColorWeight;
int* mColorWeight_Length;

int* mVertexesColor_tmp;
int** vertex_color_tmp;			
int* vertex_color_length_tmp;	
int** vertex_color_pos_tmp;
// int** color_vertex_tmp;
// int* color_vertex_length_tmp;
// int** color_vertex_pos_tmp;
int*** mCol_tmp;
int*** mCol_pos_tmp;
int** mCol_length_tmp;
int*** mRow_tmp;
int*** mRow_pos_tmp;
int** mRow_length_tmp;
int*** mSqu_tmp;
int*** mSqu_pos_tmp;
int** mSqu_length_tmp;
int* color_length;
int* color_length_tmp;

int** Neighbour;			
int* Neighbour_length;		
int** vertex_color;			
int* vertex_color_length;	
int** vertex_color_pos;  	
int* mVertexes;				
int* mVertexesPosition;		
int* CL_start_pos;       	
int vertex_use_length;
int edge_use_length;
int** color_vertex;				
int* color_vertex_length;			
int** color_vertex_pos;		
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
int** freq;                 
int* total_freq;            
int* color_clash;           
int*** mCol;
int*** mCol_pos;
int** mCol_length;
int*** mRow;
int*** mRow_pos;
int** mRow_length;
int*** mSqu;
int*** mSqu_pos;
int** mSqu_length;
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
int** tmp_clash;
BClash* best_clash_array;
int* tmp_p_array;
long long** vertex_old;
int update_Iter;

int to_index(int index1, int index2, bool type) {
	// row, col
	if(type) return (index1-1)*square_size + index2;
	// squ, point
	int row_low = int((index1-1)/order)*order + int((index2-1)/order);
	int col = ((index1-1)%order)*order + (index2-1)%order + 1;
	return row_low*square_size + col;
}

void set_Variable_size() {
	G = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) G[i] = (int *) malloc(sizeof(int) * (vertex_size + 2));
	edge = (Edge *) malloc(sizeof(Edge) * (edge_size + 1));
	Neighbour = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) Neighbour[i] = (int *) malloc(sizeof(int) * (3 * square_size));
	Neighbour_length = (int *) malloc(sizeof(int) * (vertex_size + 2));


	mColorWeight = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) mColorWeight[i] = (int *) malloc(sizeof(int) * (square_size + 2));
	mColorWeight_Length = (int *) malloc(sizeof(int) * (vertex_size + 2));

	mVertexesColor_tmp = (int *) malloc(sizeof(int) * (vertex_size + 2));
	vertex_color_tmp = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) vertex_color_tmp[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	vertex_color_length_tmp = (int *) malloc(sizeof(int) * (vertex_size + 2));
	vertex_color_pos_tmp = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) vertex_color_pos_tmp[i] = (int *) malloc(sizeof(int) * (square_size + 2));
	// color_vertex_tmp = (int **) malloc(sizeof(int *) * (square_length + 2));
	// for(int i = 1; i <= square_length; i++) color_vertex_tmp[i] = (int *) malloc(sizeof(int) * (vertex_size + 2));
	// color_vertex_length_tmp = (int *) malloc(sizeof(int) * (square_length + 2));
	// color_vertex_pos_tmp = (int **) malloc(sizeof(int *) * (square_length + 2));
	// for(int i = 1; i <= square_length; i++) color_vertex_pos_tmp[i] = (int *) malloc(sizeof(int) * (vertex_size + 2));
	mCol_tmp = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mCol_tmp[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mCol_tmp[i][j] = (int *) malloc(sizeof(int) * (square_size + 3));
	}
	mCol_pos_tmp = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mCol_pos_tmp[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mCol_pos_tmp[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 3));
	}
	mCol_length_tmp = (int **) malloc(sizeof(int *) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) mCol_length_tmp[i] = (int *) malloc(sizeof(int) * (square_size + 3));
	mRow_tmp = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mRow_tmp[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mRow_tmp[i][j] = (int *) malloc(sizeof(int) * (square_size + 3));
	}
	mRow_pos_tmp = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mRow_pos_tmp[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mRow_pos_tmp[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 3));
	}
	mRow_length_tmp = (int **) malloc(sizeof(int *) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) mRow_length_tmp[i] = (int *) malloc(sizeof(int) * (square_size + 3));
	mSqu_tmp = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mSqu_tmp[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mSqu_tmp[i][j] = (int *) malloc(sizeof(int) * (square_size + 3));
	}
	mSqu_pos_tmp = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mSqu_pos_tmp[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mSqu_pos_tmp[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 3));
	}
	mSqu_length_tmp = (int **) malloc(sizeof(int *) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) mSqu_length_tmp[i] = (int *) malloc(sizeof(int) * (square_size + 3));
	color_length = (int *) malloc(sizeof(int) * (square_size + 1));
	color_length_tmp = (int *) malloc(sizeof(int) * (square_size + 1));

	vertex_color = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) vertex_color[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	vertex_color_length = (int *) malloc(sizeof(int) * (vertex_size + 2));
	vertex_color_pos = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) vertex_color_pos[i] = (int *) malloc(sizeof(int) * (square_size + 2));
	mVertexes = (int *) malloc(sizeof(int) * (vertex_size + 2));
	mVertexesPosition = (int *) malloc(sizeof(int) * (vertex_size + 2));
	CL_start_pos = (int *) malloc(sizeof(int) * (square_size + 2));
	color_vertex = (int **) malloc(sizeof(int *) * (square_size + 2));
	for(int i = 1; i <= square_size; i++) color_vertex[i] = (int *) malloc(sizeof(int) * (vertex_size + 2));
	color_vertex_length = (int *) malloc(sizeof(int) * (square_size + 2));
	color_vertex_pos = (int **) malloc(sizeof(int *) * (square_size + 2));
	for(int i = 1; i <= square_size; i++) color_vertex_pos[i] = (int *) malloc(sizeof(int) * (vertex_size + 2));
	mEdges = (int *) malloc(sizeof(int) * (edge_size + 2));
	mEdgesPosition = (int *) malloc(sizeof(int) * (edge_size + 2));
	mVertexesColor = (int *) malloc(sizeof(int) * (vertex_size + 2));
	mVertexesColorTmp = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mVertexesColorBest = (int *) malloc(sizeof(int) * (vertex_size + 1));
	score = (long long *) malloc(sizeof(long long) * (edge_size + 2));
	mClash = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) mClash[i] = (int *) malloc(sizeof(int) * (square_size + 2));
	tmp_array = (int *) malloc(sizeof(int) * (vertex_size + 2));
	ans = (int *) malloc(sizeof(int) * (vertex_size + 2));
	tabu = (int **) malloc(sizeof(int *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) tabu[i] = (int *) malloc(sizeof(long long) * (square_size + 2));
	cc = (bool **) malloc(sizeof(bool *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) cc[i] = (bool * ) malloc(sizeof(bool) * (square_size + 1));
	tabuC = (bool **) malloc(sizeof(bool *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) tabuC[i] = (bool *) malloc(sizeof(bool) * (square_size + 1));
	tabuCV = (int ***) malloc(sizeof(int **) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) {
		tabuCV[i] = (int **) malloc(sizeof(int *) * (square_size + 1));
		for(int j = 1; j <= square_size; j++) tabuCV[i][j] = (int *) malloc(sizeof(int) * vertex_size);
	}
	tabuCVlen = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) tabuCVlen[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	tabuE = (int *) malloc(sizeof(int) * edge_size);
	tabuEC = (int *) malloc(sizeof(int) * edge_size);
	edge_array = (Edge *) malloc(sizeof(Edge) * (edge_size + 2)); 
	vertex_can_move = (bool **) malloc(sizeof(bool *) * (vertex_size + 2));
	for(int i = 1; i <= vertex_size; i++) vertex_can_move[i] = (bool *) malloc(sizeof(bool) * (square_size + 2));
	color_clash = (int *) malloc(sizeof(int) * (square_size + 2));
	init_best_order_array = (int *) malloc(sizeof(int) * (vertex_size + 2));
	tmp_bool = (bool *) malloc(sizeof(bool) * (vertex_size + 2));
	tmp_iboa = (int *) malloc(sizeof(int) * (vertex_size + 2));
	tmp_vcl = (int *) malloc(sizeof(int) * (vertex_size + 2));
	best_array = (int *) malloc(sizeof(int) * (vertex_size + 2));
	square_array = (int *) malloc(sizeof(int) * (square_size + 2));
	square_cnt = (int *) malloc(sizeof(int) * (square_size + 2));
	tmp_st = (St *) malloc(sizeof(St) * (vertex_size * square_size));
	tmp_tabu_st = (St *) malloc(sizeof(St) * (vertex_size * square_size));
	over_color = (int *) malloc(sizeof(int) * (square_size + 3));
	mCol = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mCol[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mCol[i][j] = (int *) malloc(sizeof(int) * (square_size + 3));
	}
	mCol_pos = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mCol_pos[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mCol_pos[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 3));
	}
	mCol_length = (int **) malloc(sizeof(int *) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) mCol_length[i] = (int *) malloc(sizeof(int) * (square_size + 3));
	mRow = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mRow[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mRow[i][j] = (int *) malloc(sizeof(int) * (square_size + 3));
	}
	mRow_pos = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mRow_pos[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mRow_pos[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 3));
	}
	mRow_length = (int **) malloc(sizeof(int *) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) mRow_length[i] = (int *) malloc(sizeof(int) * (square_size + 3));
	mSqu = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mSqu[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mSqu[i][j] = (int *) malloc(sizeof(int) * (square_size + 3));
	}
	mSqu_pos = (int ***) malloc(sizeof(int **) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) {
		mSqu_pos[i] = (int **) malloc(sizeof(int *) * (square_size + 3));
		for(int j = 1; j <= square_size; j++) mSqu_pos[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 3));
	}
	mSqu_length = (int **) malloc(sizeof(int *) * (square_size + 3));
	for(int i = 1; i <= square_size; i++) mSqu_length[i] = (int *) malloc(sizeof(int) * (square_size + 3));
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
	for(int i = 1; i <= vertex_size; i++) tmp_clash[i] = (int *) malloc(sizeof(int) * (square_size + 2));
	best_clash_array = (BClash *) malloc(sizeof(BClash) * (edge_size + 1));
	tmp_p_array = (int *) malloc(sizeof(int) * population_size);
	vertex_old = (long long **) malloc(sizeof(long long *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) vertex_old[i] = (long long *) malloc(sizeof(long long) * 2);
	move_vertex = (St *) malloc(sizeof(St) * (square_size * vertex_size + 1));
	bms_array = (St *) malloc(sizeof(St) * (square_size * vertex_size + 1));
	move_vertex_array = (int *) malloc(sizeof(int) * (square_size * vertex_size + 1));
	move_vertex_position = (int *) malloc(sizeof(int) * (square_size * vertex_size + 1));
	move_vertex_id = (int **) malloc(sizeof(int * ) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) move_vertex_id[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	return;
}

void init() {
	for(int i = 1; i <= vertex_size; i++) for(int j = 1; j <= vertex_size; j++) G[i][j] = -1;
	for(int i = 1; i <= vertex_size; i++) Neighbour_length[i] = 0;
	for(int i = 1; i <= vertex_size; i++) vertex_color_length[i] = 0;
	for(int i = 1; i <= vertex_size; i++) for(int j = 1; j <= square_size; j++) vertex_color_pos[i][j] = square_size + 1;
	
	for(int i = 1; i <= vertex_size; i++) {
		mVertexesPosition[i] = -1;
	}
	CL_start_pos[1] = 1;
	vertex_use_length = vertex_size;
	for(int i = 1; i <= square_size; i++) {
		for(int j = 1; j <= vertex_size; j++) color_vertex_pos[i][j] = 0;
	}
	for(int i = 1; i <= edge_size; i++) {
		mEdges[i] = i;
		mEdgesPosition[i] = i;
	}
	for(int i = 1; i <= vertex_size; i++) mVertexesColor[i] = -1;
	for(int i = 1; i <= vertex_size; i++) for(int j = 1; j <= square_size; j++) mClash[i][j] = 0;
	total_clash = 0;
	edge_clash = 0;
	for(int i = 1; i <= vertex_size; i++) {
		for(int j = 1; j <= square_size; j++) tabuC[i][j] = false;
	}
	tabuELen = 0;
	for(int i = 1; i <= vertex_size; i++) {
		for(int j = 1; j <= square_size; j++) vertex_can_move[i][j] = false;
	}
	for(int i = 1; i <= square_size; i++) color_clash[i] = 0;
	best_clash = INT_MAX;
	for(int i = 1; i <= square_size; i++) {
		for(int j = 1; j <= square_size; j++) {
			for(int p = 1; p <= square_size; p++) {
				mCol[i][j][p] = square_size * (i - 1) + p;
				mCol_pos[i][j][square_size * (i - 1) + p] = p;
			}
		}
	}
	for(int i = 1; i <= square_size; i++) for(int j = 1; j <= square_size; j++) mCol_length[i][j] = square_size;
	for(int i = 1; i <= square_size; i++) {
		for(int j = 1; j <= square_size; j++) {
			for(int p = 1; p <= square_size; p++) {
				mRow[i][j][p] = square_size * (p - 1) + i;
				mRow_pos[i][j][square_size * (p - 1) + i] = p;
			}
		}
	}
	for(int i = 1; i <= square_size; i++) for(int j = 1; j <= square_size; j++) mRow_length[i][j] = square_size;
	for(int i = 1; i <= square_size; i++) {
		for(int j = 1; j <= square_size; j++) {
			for(int p = 1; p <= square_size; p++) {
				mSqu[i][j][p] = to_index(i, p, false);
				mSqu_pos[i][j][to_index(i, p, false)] = p;
			}
		}
	}
	for(int i = 1; i <= square_size; i++) for(int j = 1; j <= square_size; j++) mSqu_length[i][j] = square_size;
	for(int i = 1; i <= vertex_size; i++) t_array[i] = false;
	t_cnt = 0;
	for(int i = 1; i <= vertex_size; i++) {
		for(int j = 1; j <= square_size; j++) {
			tmp_clash[i][j] = 0;
		}
	}
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
	int p_a = vertex_color_pos[id][c_a];
	vertex_color[id][p_a] = c_b;
	vertex_color[id][p_b] = c_a;
	vertex_color_pos[id][c_a] = p_b;
	vertex_color_pos[id][c_b] = p_a;
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

void swap_squ(int squ_id, int c_id, int id_a, int pos_b) {
	int id_b = mSqu[squ_id][c_id][pos_b];
	int pos_a = mSqu_pos[squ_id][c_id][id_a];
	mSqu[squ_id][c_id][pos_a] = id_b;
	mSqu[squ_id][c_id][pos_b] = id_a;
	mSqu_pos[squ_id][c_id][id_a] = pos_b;
	mSqu_pos[squ_id][c_id][id_b] = pos_a;
	return;
}

void swap_vertex_color_tmp(int id, int c_a, int p_b) {
	int c_b = vertex_color_tmp[id][p_b];
	int p_a = vertex_color_pos_tmp[id][c_a];
	vertex_color_tmp[id][p_a] = c_b;
	vertex_color_tmp[id][p_b] = c_a;
	vertex_color_pos_tmp[id][c_a] = p_b;
	vertex_color_pos_tmp[id][c_b] = p_a;
	return;
}

void swap_col_tmp(int col_id, int c_id, int id_a, int pos_b) {
	int id_b = mCol_tmp[col_id][c_id][pos_b];
	int pos_a = mCol_pos_tmp[col_id][c_id][id_a];
	mCol_tmp[col_id][c_id][pos_a] = id_b;
	mCol_tmp[col_id][c_id][pos_b] = id_a;
	mCol_pos_tmp[col_id][c_id][id_a] = pos_b;
	mCol_pos_tmp[col_id][c_id][id_b] = pos_a;
	return;
}

void swap_row_tmp(int row_id, int c_id, int id_a, int pos_b) {
	int id_b = mRow_tmp[row_id][c_id][pos_b];
	int pos_a = mRow_pos_tmp[row_id][c_id][id_a];
	mRow_tmp[row_id][c_id][pos_a] = id_b;
	mRow_tmp[row_id][c_id][pos_b] = id_a;
	mRow_pos_tmp[row_id][c_id][id_a] = pos_b;
	mRow_pos_tmp[row_id][c_id][id_b] = pos_a;
	return;
}

void swap_squ_tmp(int squ_id, int c_id, int id_a, int pos_b) {
	int id_b = mSqu_tmp[squ_id][c_id][pos_b];
	int pos_a = mSqu_pos_tmp[squ_id][c_id][id_a];
	mSqu_tmp[squ_id][c_id][pos_a] = id_b;
	mSqu_tmp[squ_id][c_id][pos_b] = id_a;
	mSqu_pos_tmp[squ_id][c_id][id_a] = pos_b;
	mSqu_pos_tmp[squ_id][c_id][id_b] = pos_a;
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
		// mClash[tmp_v][cur_pos] -= edge[G[id][tmp_v]].w;
		// mClash[tmp_v][pos] += edge[G[id][tmp_v]].w;
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

/// @brief pscore
/// @param id 
/// @param to 
/// @return 
int get_score(int id, int to) {
	return mClash[id][to] - mClash[id][mVertexesColor[id]];
}

int get_Col(int id) {
	return (id - 1) / square_size + 1;
}

int get_Row(int id) {
	return (id - 1) % square_size + 1;
}

int get_Squ(int id) {
	return (id-1)/(square_size*order)*order + (id-1)%square_size/order + 1;
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
	square_size = order*order;
	vertex_size = square_size*square_size;
	edge_size = square_size*square_size*(3*square_size-2*order-1)/2;
	set_Variable_size();
	init();
	vertex = (int **) malloc(sizeof(int *) * (square_size+2));
	for (int i=1; i<=square_size; i++) vertex[i] = (int *) malloc(sizeof(int) * (square_size+2));
	for (int i=1; i<=square_size; i++) {
		for (int j=1; j<=square_size; j++) {
			cin >> vertex[i][j];
			int x = to_index(i, j, true);
			if (vertex[i][j] == -1) {
				for(int k=1; k<=square_size; k++) {
					vertex_color[x][vertex_color_length[x]] = k; 
					vertex_color_pos[x][k] = vertex_color_length[x]++;
					vertex_can_move[x][k] = true;
				}
			} else {
				vertex_color[x][vertex_color_length[x]] = vertex[i][j]; 
				vertex_color_pos[x][vertex[i][j]] = vertex_color_length[x]++;
				vertex_can_move[x][vertex[i][j]] = true;
			}
		}
	}
	for (int row=1; row<=square_size; row++) {
		for (int col1=1; col1<square_size; col1++) {
			for (int col2=col1+1; col2<=square_size; col2++) {
				int x = to_index(row, col1, true);
				int y = to_index(row, col2, true);
				Neighbour[x][Neighbour_length[x]++] = y;
				Neighbour[y][Neighbour_length[y]++] = x;
				edge[++index].x = x;
				edge[index].y = y;
				edge[index].w = 1;
				G[x][y] = index;
				G[y][x] = index;
			}
		}
	}
	for (int col=1; col<=square_size; col++) {
		for (int row1=1; row1<square_size; row1++) {
			for (int row2=row1+1; row2<=square_size; row2++) {
				int x = to_index(row1, col, true);
				int y = to_index(row2, col, true);
				Neighbour[x][Neighbour_length[x]++] = y;
				Neighbour[y][Neighbour_length[y]++] = x;
				edge[++index].x = x;
				edge[index].y = y;
				edge[index].w = 1;
				G[x][y] = index;
				G[y][x] = index;
			}
		}
	}
	for (int squ=1; squ<=square_size; squ++) {
		for (int point1=1; point1<square_size; point1++) {
			for (int point2=point1+1; point2<=square_size; point2++) {
				int x = to_index(squ, point1, false);
				int y = to_index(squ, point2, false);
				if (G[x][y] == -1) {
					Neighbour[x][Neighbour_length[x]++] = y;
					Neighbour[y][Neighbour_length[y]++] = x;
					edge[++index].x = x;
					edge[index].y = y;
					edge[index].w = 1;
					G[x][y] = index;
					G[y][x] = index;
				} else edge[G[x][y]].w++;
			}
		}
	}
	// cout<<index<<endl;
	return;
}

void print_current() {
	// for(int i = 1; i <= square_size; i++) {
	// 	for(int j = 0; j < color_vertex_length[i]; j++) {
	// 		ans[color_vertex[i][j]] = i;
	// 	}
	// }
	printf("======================================================================\n");
	cout << " \t|\t";
	for(int i = 1; i <= square_size; i++) {
		if (i != 1) cout << "\t";
		cout << i;
		if (i%order == 0) cout << "\t ";
	}
	cout << endl;
	cout << "=\t#\t";
	for(int i = 1; i <= square_size; i++) {
		if (i != 1) cout << "\t";
		cout << "=";
		if (i%order == 0) cout << "\t*";
	}
	cout << endl;

	for(int i = 1; i <= square_size; i++) {
		cout << i << "\t|\t";
		for(int j = 1; j <= square_size; j++) {
			int id = square_size * (i - 1) + j;
			if (j != 1) cout << "\t";
			cout << mVertexesColorTmp[id];
			if (j%order == 0) cout << "\t|";
		}
		cout << endl;
		if (i%order == 0) {
			cout << "\t#\t";
			for(int j = 1; j <= square_size; j++) {
				if (j != 1) cout << "\t";
				cout << "—";
				if (j%order == 0) cout << "\t*";
			}
			cout << endl;
		}
	}
	return;
}

void print_info() {
	printf("======================================================================\n");
	// printf("current total unfixed vertex: %d\n", vertex_use_length);
	printf("current best clash: %d\n", best_clash);
	// printf("----------------------------------------------------------------------\n");
	// printf("current clash: \n");
	int col_cnt=0, row_cnt=0, squ_cnt=0;
	for (int i=0; i<best_clash; i++) {
		int x = edge[best_clash_array[i].e_id].x;
		int y = edge[best_clash_array[i].e_id].y;
		printf("vertex(%d, %d) and vertex(%d, %d) same color: %d\n", get_Col(x), get_Row(x), get_Col(y), get_Row(y), best_clash_array[i].color);
		if (get_Col(x) == get_Col(y)) col_cnt++;
		else if (get_Row(x) == get_Row(y)) row_cnt++;
		if (get_Squ(x) == get_Squ(y)) squ_cnt++;
	}
	printf("----------------------------------------------------------------------\n");
	printf("current col clash: %d\n", col_cnt);
	printf("current row clash: %d\n", row_cnt);
	printf("current squ clash: %d\n", squ_cnt);
	// printf("----------------------------------------------------------------------\n");
	// printf("vertex color move: \n");
	// int col_tmp[square_size+1];
	// int row_tmp[square_size+1];
	// int squ_tmp[square_size+1];
	// for (int i=1; i<=square_size; i++) {
	// 	col_tmp[i] = 0;
	// 	row_tmp[i] = 0;
	// 	squ_tmp[i] = 0;
	// }
	// for(int i = 0; i < vertex_use_length; i++) {
	// 	if (mVertexesColor_tmp[mVertexes[i]] != mVertexesColorTmp[mVertexes[i]]) {
	// 		col_tmp[get_Col(mVertexes[i])]++;
	// 		row_tmp[get_Row(mVertexes[i])]++;
	// 		squ_tmp[get_Squ(mVertexes[i])]++;
	// 		// printf("vertex(%d, %d) move from %d to %d\n", get_Col(mVertexes[i]), get_Row(mVertexes[i]), mVertexesColor_tmp[mVertexes[i]], mVertexesColorTmp[mVertexes[i]]);
	// 	}
	// }
	// printf("----------------------------------------------------------------------\n");
	// for (int i=1; i<=square_size; i++) {
	// 	printf("col num of index %d : %d\n", i, col_tmp[i]);
	// }
	// printf("----------------------------------------------------------------------\n");
	// for (int i=1; i<=square_size; i++) {
	// 	printf("row num of index %d : %d\n", i, row_tmp[i]);
	// }
	// printf("----------------------------------------------------------------------\n");
	// for (int i=1; i<=square_size; i++) {
	// 	printf("squ num of index %d : %d\n", i, squ_tmp[i]);
	// }

	// for (int i=1; i<=square_size; i++) {
	// 	color_length_tmp[i] = 0;
	// }
	// for(int i = 0; i < vertex_use_length; i++) {
	// 	color_length_tmp[mVertexesColorTmp[mVertexes[i]]]++;
	// }
	// printf("----------------------------------------------------------------------\n");
	// printf("current color length: \n");
	// for (int i=1; i<=square_size; i++) {
	// 	printf("color length of %d: %d\n", i, color_length[i]+color_length_tmp[i]);
	// }
	
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
		for(int j = 1; j <= square_size; j++) {
			tmp_clash[i][j] = 0;
		}
	}
	for(int i = 1; i <= edge_size; i++) {
		tmp_x = edge[i].x;
		tmp_y = edge[i].y;
		if(mVertexesPosition[tmp_x] == -1 || mVertexesPosition[tmp_y] == -1) continue;
		tmp_clash[tmp_x][mVertexesColor[tmp_y]] += 1;
		tmp_clash[tmp_y][mVertexesColor[tmp_x]] += 1;
		// tmp_clash[tmp_x][mVertexesColor[tmp_y]] += edge[i].w;
		// tmp_clash[tmp_y][mVertexesColor[tmp_x]] += edge[i].w;
	}
	for(int i = 0; i < vertex_use_length; i++) {
		for(int j = 1; j <= square_size; j++) {
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
		if(mVertexesColor[edge[i].x] == mVertexesColor[edge[i].y]) res += 1;
		// if(mVertexesColor[edge[i].x] == mVertexesColor[edge[i].y]) res += edge[i].w;
	}
	if(res != total_clash) {
		cout << "total_clash wrong" << endl;
		cout << res << " " << total_clash << endl;
		// exit(0);
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
	int val[square_size + 2];
	for(int i = 1; i <= square_size; i++) {
		memset(val, 0, sizeof(val));
		for(int j = 1; j <= square_size; j++) {
			int id = square_size * (i - 1) + j;
			for(int p = 0; p < vertex_color_length[id]; p++) {
				val[vertex_color[id][p]]++;
			}
		}
		for(int j = 1; j <= square_size; j++) {
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
			for(int j = 1; j <= square_size; j++) {
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

int mCheck_AC(int vertex_id, int color) {

	// 单元格已固定的颜色列表，单元格的可选颜色列表长度
	for (int i=1; i<=vertex_size; i++) {
		mVertexesColor_tmp[i] = mVertexesColor[i];
	}
	// 单元格的可选颜色列表，及其哈希表
	for (int i=1; i<=vertex_size; i++) {
		vertex_color_length_tmp[i] = vertex_color_length[i];
		for (int j=0; j<vertex_color_length_tmp[i]; j++) {
			vertex_color_tmp[i][j] = vertex_color[i][j];
		}
		for (int j=1; j<=square_size; j++) {
			vertex_color_pos_tmp[i][j] = vertex_color_pos[i][j];
		}
	}

	// 行列组相关
	for (int i=1; i<=square_size; i++) {
		for (int j=1; j<=square_size; j++) {
			mCol_length_tmp[i][j] = mCol_length[i][j];
			mRow_length_tmp[i][j] = mRow_length[i][j];
			mSqu_length_tmp[i][j] = mSqu_length[i][j];
			for (int k=1; k<=mCol_length_tmp[i][j]; k++) {
				mCol_tmp[i][j][k] = mCol[i][j][k];
			}
			for (int k=1; k<=mRow_length_tmp[i][j]; k++) {
				mRow_tmp[i][j][k] = mRow[i][j][k];
			}
			for (int k=1; k<=mSqu_length_tmp[i][j]; k++) {
				mSqu_tmp[i][j][k] = mSqu[i][j][k];
			}
			for (int k=1; k<=vertex_size; k++) {
				mCol_pos_tmp[i][j][k] = mCol_pos[i][j][k];
				mRow_pos_tmp[i][j][k] = mRow_pos[i][j][k];
				mSqu_pos_tmp[i][j][k] = mSqu_pos[i][j][k];
			}
		}
	}

	int score = 0;
	queue<St> q;
	St st;
	st.id = vertex_id;
	st.c = color;
	q.push(st);

	while(!q.empty()) {
		st = q.front();
		q.pop();
		int tmp_v = st.id;
		int tmp_c = st.c;
		if(mVertexesColor_tmp[tmp_v] > 0) continue;
		swap_vertex_color_tmp(tmp_v, tmp_c, 0);
		if (vertex_color_length_tmp[tmp_v] == 0) return 0;
		vertex_color_length_tmp[tmp_v] = 0;
		mVertexesColor_tmp[tmp_v] = tmp_c;
		// swap_col_tmp(get_Col(tmp_v), tmp_c, tmp_v, 1);
		mCol_length_tmp[get_Col(tmp_v)][tmp_c] = 0;
		// swap_row_tmp(get_Row(tmp_v), tmp_c, tmp_v, 1);
		mRow_length_tmp[get_Row(tmp_v)][tmp_c] = 0;
		// swap_squ_tmp(get_Squ(tmp_v), tmp_c, tmp_v, 1);
		mSqu_length_tmp[get_Squ(tmp_v)][tmp_c] = 0;	

		for(int i = 1; i <= square_size; i++) {
			if(mCol_pos_tmp[get_Col(tmp_v)][i][tmp_v] <= mCol_length_tmp[get_Col(tmp_v)][i]) {
				swap_col_tmp(get_Col(tmp_v), i, tmp_v, mCol_length_tmp[get_Col(tmp_v)][i]--);
				if(mCol_length_tmp[get_Col(tmp_v)][i] == 1) {
					st.id = mCol_tmp[get_Col(tmp_v)][i][1];
					st.c = i;
					q.push(st);
				}
			}
			if(mRow_pos_tmp[get_Row(tmp_v)][i][tmp_v] <= mRow_length_tmp[get_Row(tmp_v)][i]) {
				swap_row_tmp(get_Row(tmp_v), i, tmp_v, mRow_length_tmp[get_Row(tmp_v)][i]--);
				if(mRow_length_tmp[get_Row(tmp_v)][i] == 1) {
					st.id = mRow_tmp[get_Row(tmp_v)][i][1];
					st.c = i;
					q.push(st);
				}
			}
			if(mSqu_pos_tmp[get_Squ(tmp_v)][i][tmp_v] <= mSqu_length_tmp[get_Squ(tmp_v)][i]) {
				swap_squ_tmp(get_Squ(tmp_v), i, tmp_v, mSqu_length_tmp[get_Squ(tmp_v)][i]--);
				if(mSqu_length_tmp[get_Squ(tmp_v)][i] == 1) {
					st.id = mSqu_tmp[get_Squ(tmp_v)][i][1];
					st.c = i;
					q.push(st);
				}
			}
		}
		for(int i = 0; i < Neighbour_length[tmp_v]; i++) {
			int tmp_V = Neighbour[tmp_v][i];
			if(vertex_color_pos_tmp[tmp_V][tmp_c] < vertex_color_length_tmp[tmp_V]) {
				swap_vertex_color_tmp(tmp_V, tmp_c, vertex_color_length_tmp[tmp_V] - 1);
				score++;
				vertex_color_length_tmp[tmp_V]--;
				if(vertex_color_length_tmp[tmp_V] == 1) {
					st.id = tmp_V;
					st.c = vertex_color_tmp[tmp_V][0];
					q.push(st);
				}
			}
			if(mCol_pos_tmp[get_Col(tmp_V)][tmp_c][tmp_V] <= mCol_length_tmp[get_Col(tmp_V)][tmp_c]) {
				if(mCol_length_tmp[get_Col(tmp_V)][tmp_c] != 0) swap_col_tmp(get_Col(tmp_V), tmp_c, tmp_V, mCol_length_tmp[get_Col(tmp_V)][tmp_c]--);
				if(mCol_length_tmp[get_Col(tmp_V)][tmp_c] == 1) {
					st.id = mCol_tmp[get_Col(tmp_V)][tmp_c][1];
					st.c = tmp_c;
					q.push(st);
				}
			}
			if(mRow_pos_tmp[get_Row(tmp_V)][tmp_c][tmp_V] <= mRow_length_tmp[get_Row(tmp_V)][tmp_c]) {
				if(mRow_length_tmp[get_Row(tmp_V)][tmp_c] != 0) swap_row_tmp(get_Row(tmp_V), tmp_c, tmp_V, mRow_length_tmp[get_Row(tmp_V)][tmp_c]--);
				if(mRow_length_tmp[get_Row(tmp_V)][tmp_c] == 1) {
					st.id = mRow_tmp[get_Row(tmp_V)][tmp_c][1];
					st.c = tmp_c;
					q.push(st); 
				}
			}
			if(mSqu_pos_tmp[get_Squ(tmp_V)][tmp_c][tmp_V] <= mSqu_length_tmp[get_Squ(tmp_V)][tmp_c]) {
				if(mSqu_length_tmp[get_Squ(tmp_V)][tmp_c] != 0) swap_squ_tmp(get_Squ(tmp_V), tmp_c, tmp_V, mSqu_length_tmp[get_Squ(tmp_V)][tmp_c]--);
				if(mSqu_length_tmp[get_Squ(tmp_V)][tmp_c] == 1) {
					st.id = mSqu_tmp[get_Squ(tmp_V)][tmp_c][1];
					st.c = tmp_c;
					q.push(st); 
				}
			}
		}
	}

	return score;
}

void mArcConsistency() {
	for (int i = 0; i < vertex_use_length; i++) {
		int vertex_id = mVertexes[i];
		bool conflict = false;
		for (int j=0; j<vertex_color_length[vertex_id]; j++) {
			int color = vertex_color[vertex_id][j];
			int weight = mCheck_AC(vertex_id, color);
			if (weight == 0) {
				conflict = true;
				cout << vertex_id << " " << color << endl;
				if(vertex_color_pos[vertex_id][color] < vertex_color_length[vertex_id])
					swap_vertex_color(vertex_id, color, --vertex_color_length[vertex_id]);
				if(mCol_pos[get_Col(vertex_id)][color][vertex_id] <= mCol_length[get_Col(vertex_id)][color])
					swap_col(get_Col(vertex_id), color, vertex_id, mCol_length[get_Col(vertex_id)][color]--);
				if(mCol_pos[get_Row(vertex_id)][color][vertex_id] <= mRow_length[get_Row(vertex_id)][color])
					swap_row(get_Row(vertex_id), color, vertex_id, mRow_length[get_Row(vertex_id)][color]--);
				if(mCol_pos[get_Squ(vertex_id)][color][vertex_id] <= mSqu_length[get_Squ(vertex_id)][color])
					swap_squ(get_Squ(vertex_id), color, vertex_id, mSqu_length[get_Squ(vertex_id)][color]--);
			} else {
				mColorWeight[vertex_id][color] = weight;
			}
		}
		if (conflict) {
			for(int tmp_c = 1; tmp_c <= square_size; tmp_c++) {
				if(vertex_color_length[vertex_id] == 1 || mCol_length[get_Col(vertex_id)][tmp_c] == 1 || mCol_length[get_Col(vertex_id)][tmp_c] == 1 || mCol_length[get_Col(vertex_id)][tmp_c] == 1) {
					queue<St> q;
					St st;
					st.id = vertex_id;
					st.c = vertex_color[vertex_id][0];
					q.push(st);
					while(!q.empty()) {
						st = q.front();
						q.pop();
						int tmp_v = st.id;
						int tmp_c = st.c;
						if(mVertexesColor[tmp_v] > 0) continue;
						swap_vertex_color(tmp_v, tmp_c, 0);
						vertex_color_length[tmp_v] = 0;
						color_vertex[tmp_c][color_vertex_length[tmp_c]] = tmp_v;
						color_vertex_pos[tmp_c][tmp_v] = color_vertex_length[tmp_c]++;
						mVertexesColor[tmp_v] = tmp_c;
						vertex_can_move[tmp_v][tmp_c] = false; 
						swap_col(get_Col(tmp_v), tmp_c, tmp_v, 1);
						mCol_length[get_Col(tmp_v)][tmp_c] = 0;
						swap_row(get_Row(tmp_v), tmp_c, tmp_v, 1);
						mRow_length[get_Row(tmp_v)][tmp_c] = 0;
						swap_squ(get_Squ(tmp_v), tmp_c, tmp_v, 1);
						mSqu_length[get_Squ(tmp_v)][tmp_c] = 0;
						for(int i = 1; i <= square_size; i++) {
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
							if(mSqu_pos[get_Squ(tmp_v)][i][tmp_v] <= mSqu_length[get_Squ(tmp_v)][i]) {
								swap_squ(get_Squ(tmp_v), i, tmp_v, mSqu_length[get_Squ(tmp_v)][i]--);
								if(mSqu_length[get_Squ(tmp_v)][i] == 1) {
									st.id = mSqu[get_Squ(tmp_v)][i][1];
									st.c = i;
									q.push(st);
								}
							}
						}
						for(int i = 0; i < Neighbour_length[tmp_v]; i++) {
							int tmp_V = Neighbour[tmp_v][i];
							if(vertex_color_pos[tmp_V][tmp_c] < vertex_color_length[tmp_V]) {
								swap_vertex_color(tmp_V, tmp_c, vertex_color_length[tmp_V] - 1);
								vertex_color_length[tmp_V]--;
								if(vertex_color_length[tmp_V] == 1) {
									st.id = tmp_V;
									st.c = vertex_color[tmp_V][0];
									q.push(st);
								}
							}
							if(mCol_pos[get_Col(tmp_V)][tmp_c][tmp_V] <= mCol_length[get_Col(tmp_V)][tmp_c]) {
								if(mCol_length[get_Col(tmp_V)][tmp_c] != 0) swap_col(get_Col(tmp_V), tmp_c, tmp_V, mCol_length[get_Col(tmp_V)][tmp_c]--);
								if(mCol_length[get_Col(tmp_V)][tmp_c] == 1) {
									st.id = mCol[get_Col(tmp_V)][tmp_c][1];
									st.c = tmp_c;
									q.push(st);
								}
							}
							if(mRow_pos[get_Row(tmp_V)][tmp_c][tmp_V] <= mRow_length[get_Row(tmp_V)][tmp_c]) {
								if(mRow_length[get_Row(tmp_V)][tmp_c] != 0) swap_row(get_Row(tmp_V), tmp_c, tmp_V, mRow_length[get_Row(tmp_V)][tmp_c]--);
								if(mRow_length[get_Row(tmp_V)][tmp_c] == 1) {
									st.id = mRow[get_Row(tmp_V)][tmp_c][1];
									st.c = tmp_c;
									q.push(st); 
								}
							}
							if(mSqu_pos[get_Squ(tmp_V)][tmp_c][tmp_V] <= mSqu_length[get_Squ(tmp_V)][tmp_c]) {
								if(mSqu_length[get_Squ(tmp_V)][tmp_c] != 0) swap_squ(get_Squ(tmp_V), tmp_c, tmp_V, mSqu_length[get_Squ(tmp_V)][tmp_c]--);
								if(mSqu_length[get_Squ(tmp_V)][tmp_c] == 1) {
									st.id = mSqu[get_Squ(tmp_V)][tmp_c][1];
									st.c = tmp_c;
									q.push(st); 
								}
							}
							vertex_can_move[tmp_V][tmp_c] = false;
						}
					}
				}
			}
		}
	}

	edge_use_length = 0;
	for (int i=0; i<vertex_use_length; i++) {
		if (mVertexesColor[mVertexes[i]] > 0) {
			mVertexes[i] = mVertexes[--vertex_use_length];
			mVertexesPosition[mVertexes[i]] = i;
			i--;
		}
	}
	for (int i=0; i<vertex_use_length; i++) {
		int vertex_id = mVertexes[i];
		for (int j=0; j<Neighbour_length[vertex_id]; j++) {
			int tmp_v = Neighbour[vertex_id][j];
			if (mVertexesColor[tmp_v] > 0) {
				Neighbour[vertex_id][j] = Neighbour[vertex_id][--Neighbour_length[vertex_id]];
				j--;
			}
			else {
				int edge_id = G[vertex_id][tmp_v];
				if(mEdgesPosition[edge_id] > edge_use_length) swap_edge(edge_id, ++edge_use_length);
			}
		}
	}
	for (int i=1; i<=square_size; i++) {
		color_length[i] = 0;
	}
	for(int i = 1; i <= vertex_size; i++) {
		if(mVertexesColor[i] > 0) color_length[mVertexesColor[i]]++;
	}
}

// TODO 单元传播
void mReduceVertexes() {
	pppp_cnt = 0;
	for(int i = 1; i <= vertex_size; i++) if(vertex_color_length[i] == 1) pppp_cnt++;
	// 固定点处理
	for (int i=1; i<=vertex_size; i++) {
		if (vertex_color_length[i] == 1) {
			int tmp_c = vertex_color[i][0];
			vertex_color_length[i] = 0;
			mVertexesColor[i] = tmp_c;
			color_vertex_pos[tmp_c][i] = color_vertex_length[tmp_c];
			color_vertex[tmp_c][color_vertex_length[tmp_c]++] = i;
			vertex_can_move[i][tmp_c] = false;
			// i不能再选择其他颜色，因此i所在的行、列、组的其他颜色的可选点长度都要减一
			for (int j=1; j<=square_size; j++) {
				if(mCol_pos[get_Col(i)][j][i] <= mCol_length[get_Col(i)][j]) swap_col(get_Col(i), j, i, mCol_length[get_Col(i)][j]--);
				if(mRow_pos[get_Row(i)][j][i] <= mRow_length[get_Row(i)][j]) swap_row(get_Row(i), j, i, mRow_length[get_Row(i)][j]--);
				if(mSqu_pos[get_Squ(i)][j][i] <= mSqu_length[get_Squ(i)][j]) swap_squ(get_Squ(i), j, i, mSqu_length[get_Squ(i)][j]--);
			}
			// i所在行、列、组的tmp_c无法再被其他点选择
			swap_col(get_Col(i), tmp_c, i, 1);
			mCol_length[get_Col(i)][tmp_c] = 0;
			swap_row(get_Row(i), tmp_c, i, 1);
			mRow_length[get_Row(i)][tmp_c] = 0;
			swap_squ(get_Squ(i), tmp_c, i, 1);
			mSqu_length[get_Squ(i)][tmp_c] = 0;
			for(int j = 0; j < Neighbour_length[i]; j++) {
				int tmp_V = Neighbour[i][j];
				// 删除i点的邻居的对应tmp_c
				if(vertex_color_pos[tmp_V][tmp_c] < vertex_color_length[tmp_V])
					swap_vertex_color(tmp_V, tmp_c, --vertex_color_length[tmp_V]);
				// i点的邻居不能再选择tmp_c
				if(mCol_pos[get_Col(tmp_V)][tmp_c][tmp_V] <= mCol_length[get_Col(tmp_V)][tmp_c])
					swap_col(get_Col(tmp_V), tmp_c, tmp_V, mCol_length[get_Col(tmp_V)][tmp_c]--);
				if(mRow_pos[get_Row(tmp_V)][tmp_c][tmp_V] <= mRow_length[get_Row(tmp_V)][tmp_c])
					swap_row(get_Row(tmp_V), tmp_c, tmp_V, mRow_length[get_Row(tmp_V)][tmp_c]--);
				if(mSqu_pos[get_Squ(tmp_V)][tmp_c][tmp_V] <= mSqu_length[get_Squ(tmp_V)][tmp_c])
					swap_squ(get_Squ(tmp_V), tmp_c, tmp_V, mSqu_length[get_Squ(tmp_V)][tmp_c]--);
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
	for(int i = 1; i <= square_size; i++) {
		for(int j = 1; j <= square_size; j++) {
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
			if(mSqu_length[i][j] == 1) {
				st.id = mSqu[i][j][1];
				st.c = j;
				//cout << "Squ: " << st.id << " " << st.c << endl;
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
		color_vertex[tmp_c][color_vertex_length[tmp_c]] = tmp_v;
		color_vertex_pos[tmp_c][tmp_v] = color_vertex_length[tmp_c]++;
		// 此时才开始将tmp_v赋值为tmp_c
		mVertexesColor[tmp_v] = tmp_c;
		vertex_can_move[tmp_v][tmp_c] = false;
		swap_col(get_Col(tmp_v), tmp_c, tmp_v, 1);
		mCol_length[get_Col(tmp_v)][tmp_c] = 0;
		swap_row(get_Row(tmp_v), tmp_c, tmp_v, 1);
		mRow_length[get_Row(tmp_v)][tmp_c] = 0;
		swap_squ(get_Squ(tmp_v), tmp_c, tmp_v, 1);
		mSqu_length[get_Squ(tmp_v)][tmp_c] = 0;
		// 传播， 颜色
		for(int i = 1; i <= square_size; i++) {
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
			if(mSqu_pos[get_Squ(tmp_v)][i][tmp_v] <= mSqu_length[get_Squ(tmp_v)][i]) {
				swap_squ(get_Squ(tmp_v), i, tmp_v, mSqu_length[get_Squ(tmp_v)][i]--);
				if(mSqu_length[get_Squ(tmp_v)][i] == 1) {
					st.id = mSqu[get_Squ(tmp_v)][i][1];
					st.c = i;
					q.push(st);
				}
			}
		}
		//cout << tmp_v << " " << tmp_c << endl;
		// 传播， 邻居
		for(int i = 0; i < Neighbour_length[tmp_v]; i++) {
			int tmp_V = Neighbour[tmp_v][i];
			if(vertex_color_pos[tmp_V][tmp_c] < vertex_color_length[tmp_V]) {
				swap_vertex_color(tmp_V, tmp_c, vertex_color_length[tmp_V] - 1);
				vertex_color_length[tmp_V]--;
				if(vertex_color_length[tmp_V] == 1) {
					st.id = tmp_V;
					st.c = vertex_color[tmp_V][0];
					//cout << "add vcl: " << st.id << " " << st.c << endl;
					q.push(st);
				}
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
			if(mSqu_pos[get_Squ(tmp_V)][tmp_c][tmp_V] <= mSqu_length[get_Squ(tmp_V)][tmp_c]) {
				if(mSqu_length[get_Squ(tmp_V)][tmp_c] != 0) swap_squ(get_Squ(tmp_V), tmp_c, tmp_V, mSqu_length[get_Squ(tmp_V)][tmp_c]--);
				if(mSqu_length[get_Squ(tmp_V)][tmp_c] == 1) {
					st.id = mSqu[get_Squ(tmp_V)][tmp_c][1];
					st.c = tmp_c;
					//cout << "add row: " << st.id << " " << st.c << endl;
					q.push(st); 
				}
			}
			vertex_can_move[tmp_V][tmp_c] = false;
		}
	}

	vertex_use_length = 0;
	edge_use_length = 0;
	for(int i = 1; i <= vertex_size; i++) {
		// 更新颜色可选长度
		if(mVertexesColor[i] < 0) {
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
	for (int i=1; i<=square_size; i++) {
		color_length[i] = 0;
	}
	for(int i=1; i<=vertex_size; i++) {
		if(mVertexesColor[i] > 0) color_length[mVertexesColor[i]]++;
	}
	// cout << "Vertex left: " << vertex_use_length << endl; 
	// cout << "Vertex reduce: " << vertex_size - vertex_use_length << endl;
	// print_current();
	return;
}

void build() {
	total_clash = 0;
	update_Iter = mPopulationMaxVOld[mPopulation_id];
    cout << update_Iter << endl;
	// 初始化
	for(int i = 0; i < vertex_use_length; i++) {
		// score 表示点的得分
		score[mVertexes[i]] = 0;
		for(int j = 1; j <= square_size; j++) {
			mClash[mVertexes[i]][j] = 0;
			tabu[mVertexes[i]][j] = 0;
		}
	}
	// 存在冲突的边的个数
	edge_clash_length = 0;
	// 计算点选择某一颜色导致的冲突个数，  
	for(int i = 1; i <= edge_use_length; i++) {
		// mClash[edge[mEdges[i]].x][mVertexesColor[edge[mEdges[i]].y]] += edge[mEdges[i]].w;
		// mClash[edge[mEdges[i]].y][mVertexesColor[edge[mEdges[i]].x]] += edge[mEdges[i]].w;
		mClash[edge[mEdges[i]].x][mVertexesColor[edge[mEdges[i]].y]] += 1;
		mClash[edge[mEdges[i]].y][mVertexesColor[edge[mEdges[i]].x]] += 1;
		if(mVertexesColor[edge[mEdges[i]].x] == mVertexesColor[edge[mEdges[i]].y]) {
			// cout<<edge[mEdges[i]].x<<" - "<<edge[mEdges[i]].y<<endl;
			total_clash += 1;
			// total_clash += edge[mEdges[i]].w;
			swap_edge(mEdges[i], ++edge_clash_length);
		}
	}
	for(int i = 0; i < vertex_use_length; i++) {
		ClashVertexesPos[mVertexes[i]] = -1;
		vertex_old[mVertexes[i]][0] = mPopulationVertexOld[mPopulation_id][mVertexes[i]];
	}
	vertex_clash_length = 0;
	move_vertex_size = 0;
	// 存储存在冲突的点
	for(int i = 0; i < vertex_use_length; i++) {
		int vertex_id = mVertexes[i];
		if(mClash[vertex_id][mVertexesColor[vertex_id]] > 0) {
			ClashVertexes[vertex_clash_length] = vertex_id;
			ClashVertexesPos[vertex_id] = vertex_clash_length++;
		}
	}
	//check_move_vertex();
	// cout << total_clash <<endl;
	return;
}

void increase_score() {
	for(int i = 1; i <= edge_clash_length; i++) {
		// score[edge[mEdges[i]].x] += 1;
		// score[edge[mEdges[i]].y] += 1;
		score[edge[mEdges[i]].x] += edge[mEdges[i]].w;
		score[edge[mEdges[i]].y] += edge[mEdges[i]].w;
	}
	return;
}

//TODO: main function
int tabuSearch() {
	int tSminClash;
	build();
	tSminClash = total_clash;
	if (total_clash == 0) return tSminClash;
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
	for (long long iters = mPopulationMaxVOld[mPopulation_id] + 1; iters < total_iters; iters++) {
		// cout << iters << " " << total_clash << " " << tSminClash << " " << vertex_clash_length << " " << population_cnt << " " << move_vertex_size << endl;
		min_sub_clash = INT_MAX;
		min_sub_tabu_clash = INT_MAX;
		//min_sub_clash = 1;
		//min_sub_tabu_clash = 1;
		cnt = 0;
		tabu_cnt = 0;
		total_cnt = 0;
		// 遍历所有当前赋值存在冲突的点
		for (int i = 0; i < vertex_clash_length; i++) {
			int vertex_id = ClashVertexes[i];
			// 遍历当前点换成其他颜色的得分（减少的冲突数 是负数，越小越好）
			for(int j = 0; j < vertex_color_length[vertex_id]; j++) {
				int tmp_c = vertex_color[vertex_id][j];
				if(tmp_c == mVertexesColor[vertex_id]) continue;
				// 计算得分
				int tmp_s = get_score(vertex_id, tmp_c);
				//long long tmp_cs = 0;
				// 不在禁忌列表的话
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
				// 被禁忌的话
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
		if (cnt == 0 && tabu_cnt == 0) {
			return tSminClash;
		}
		// tabu
		// 当被禁忌的最小值比不被禁忌的最小值小、并且采用该move比当前最小值还小时，或者不存在不被禁忌的好move时，进入该循环；
		else if ((tabu_cnt > 0 && min_sub_tabu_clash < min_sub_clash && (total_clash + min_sub_tabu_clash < tSminClash)) || (cnt == 0 && tabu_cnt > 0)) {
			int r;
			min_sub_tabu_cs = LLONG_MIN;
			int cs_cnt = 0;
			int s_cnt = min(maxLength, tabu_cnt);
			// 随机选择三个操作，在三个操作中选择点的score最小的操作
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
			// 更新tabu表，它由三部分组成
			tabu[move_id][old_c] = iters + rand() % tabuStep + alpha * vertex_clash_length;
			// 实施move
			add_vertex(move_id, move_to);
			total_clash += min_sub_tabu_clash;
			if(vertex_old[move_id][1] <= update_Iter) vertex_old[move_id][0] = vertex_old[move_id][1];
			vertex_old[move_id][1] = iters;
			// updating rule 1
			increase_score();
			score[move_id] = 0;
		}
		// not tabu
		// 否则就选择非禁忌的操作
		else {
			int r;
			min_sub_cs = LLONG_MIN;
			int cs_cnt = 0;
			int s_cnt = min(maxLength, cnt);
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
			// cout<<move_id<<" : "<<mVertexesColor[move_id]<<" -> "<<move_to<<endl;
			// cout<<"clash nums of "<<mVertexesColor[move_id]<<": "<<mClash[move_id][mVertexesColor[move_id]]<<endl;
			// cout<<"clash nums of "<<move_to<<": "<<mClash[move_id][move_to]<<endl;
			add_vertex(move_id, move_to);
			total_clash += min_sub_clash;
			if(vertex_old[move_id][1] <= update_Iter) vertex_old[move_id][0] = vertex_old[move_id][1];
			vertex_old[move_id][1] = iters;
			increase_score();
			score[move_id] = 0;
		}
		if (total_clash <= tSminClash) {
			tSminClash = total_clash;
			update_Iter = iters;
			for(int i = 0; i < vertex_use_length; i++) {
				mVertexesColorTmp[mVertexes[i]] = mVertexesColor[mVertexes[i]];
			}
			for(int i = 1; i <= edge_clash_length; i++) best_clash_array[i - 1].e_id = mEdges[i];
			for(int i = 1; i <= edge_clash_length; i++) best_clash_array[i - 1].color = mVertexesColor[edge[mEdges[i]].x];
			if(tSminClash == 0) return tSminClash;
			no_improve_iters = 0;
		} else no_improve_iters++;
	}
	return tSminClash;
}

bool LocalSearch() {
	int tabu_clash = tabuSearch();
	if(tabu_clash < best_clash) {
		best_clash = tabu_clash;
		no_improve_steps = 1;
	}
	// print_current();
	// print_info();
	for(int i = 0; i < vertex_use_length; i++) {
		mVertexesColor_tmp[mVertexes[i]] = mVertexesColorTmp[mVertexes[i]];
	}
	if(tabu_clash == 0) return true;
	return false;
}
// algorithm begin
void mGenerate() {
	for(int i = 0; i < vertex_use_length; i++) {
		int vertex_id = mVertexes[i];
		// int origin_color = 0;
		// for (int j=1; j<vertex_color_length[vertex_id]; j++) {
		// 	if (mColorWeight[vertex_id][vertex_color[vertex_id][j]] > mColorWeight[vertex_id][vertex_color[vertex_id][origin_color]]) origin_color = j;
		// }
		// mVertexesColor[vertex_id] = vertex_color[vertex_id][origin_color];
		int r = rand() % vertex_color_length[vertex_id];
		mVertexesColor[vertex_id] = vertex_color[vertex_id][r];
	}
	for (int i=1; i<=vertex_size; i++) {
		// mVertexesColorTmp[i] = mVertexesColor[i];
		// mVertexesColor_tmp[i] = mVertexesColor[i];
		mVertexesColorTmp[i] = 0;
		mVertexesColor_tmp[i] = 0;
	}
	// print_current();
	if(LocalSearch()) return; 
	return;
}

void mCopy() {
	// 从pool池中随机选择一个状态
	if(!mPopulationFlag) mPopulation_id = rand() % population_cnt;
	//cout << "mPopulation_id: " << mPopulation_id << endl;
	//复制点的赋值
	for(int i = 0; i < vertex_use_length; i++) {
		mVertexesColor[mVertexes[i]] = mPopulation[mPopulation_id][mVertexes[i]];
	}
	//复制best_clash_array
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
	// 当前比以前都好
	if(best_clash < mPopulationClashLen) {
		mPopulationClashLen = best_clash;
		population_cnt = 0;
	}
	bool flag = false;
	for(int i = 0; i < population_cnt; i++) {
		flag = true;
		for(int j = 0; j < best_clash; j++) {
			// 若 mPopulationClash 和 best_clash_array 不一致
			if((mPopulationClash[i][j] == best_clash_array[j].e_id && mPopulationClashColor[i][j] != best_clash_array[j].color) || mPopulationClash[i][j] != best_clash_array[j].e_id) {
				flag = false;
				break;
			}
		}
		// 若当前状态和pool池中的某状态“相似”，则更新池中的state
		if(flag) {
			for(int j = 0; j < best_clash; j++) {
				mPopulationClash[i][j] = best_clash_array[j].e_id;
				mPopulationClashColor[i][j] = best_clash_array[j].color;
			}
			for(int j = 0; j < vertex_use_length; j++) {
				mPopulation[i][mVertexes[j]] = mVertexesColorTmp[mVertexes[j]];
                // 存储最优的state
				if(vertex_old[mVertexes[j]][1] <= update_Iter) mPopulationVertexOld[i][mVertexes[j]] = vertex_old[mVertexes[j]][1];
				else mPopulationVertexOld[i][mVertexes[j]] = vertex_old[mVertexes[j]][0];
			}
			mPopulationMaxVOld[i] = update_Iter;
			// 记录大循环数
			mPopulationOld[i] = Iter;
			break;
		}
	}
	// 将当前点存入pool池中
	if(!flag) {
		// 若pool池未满
		if(population_cnt != population_size) {
			// 存储mPopulationClash
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
		// 若pool池已满，则删除最老的状态
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
	// for(int i = 0; i < vertex_use_length; i++) cout << mVertexes[i] << " " << vertex_color_length[mVertexes[i]] << " " << mPopulationVertexOld[0][mVertexes[i]] << " " << vertex_old[mVertexes[i]][0] << " " << vertex_old[mVertexes[i]][1] << " " << update_Iter << endl;
	return;
}

void mPerturbation() {
	long long max_old = LLONG_MIN;
	long long min_old = LLONG_MAX;
	for(int i = 0; i < vertex_use_length; i++) {
		min_old = min(min_old, mPopulationVertexOld[mPopulation_id][mVertexes[i]]);
	}
	max_old = mPopulationMaxVOld[mPopulation_id];
	// 参数为0.2
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
	best_clash = INT_MAX;
	filename = argv[1];
	seed = atoi(argv[2]);
	time_limit = atof(argv[3]);
	// filename = "inst36x36_50_0.txt";
	// seed = 2;
	// time_limit = 10;
	srand(seed);
	mRead(filename);
	mReduceVertexes(); 
	// mArcConsistency();

	mStartTime();
	mGenerate();
	// cout << "mGenerate finish" << endl;
	if(check_finish()) {
		// print_current();
		// print_ans();
		return 0;
	}
	steps = 1;
	no_improve_steps = 1;
	total_Iters = 10000;
	for(Iter = 0; total_time < time_limit; Iter++) {
		mPopulationUpdate();
		mCopy();
		mPerturbation();
		LocalSearch();
		if(check_finish()) break;
		mCurrentTime();
	}
	// print_current();
	// print_ans();
	return 0;
}

#include <bits/stdc++.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <libgen.h>
#include <cassert>
using namespace std;

struct Edge {
	int x;
	int y;
	int w;
};

// temp
int** tmp_clash;
int* tmp_st;
int* tmp_tabu_st;
int* tmp_sub_st;
int* tmp_sub_tabu_st;
int* tmp_bms_st;
int* tmp_sub_bms_st;
int** tmp_vertex;
int* tmp_sp_vertexes;
bool* tmp_flag;

// const & hyper
const int total_Iters = 10000;
const int tabuStep = 10;
const int max_bms_length = 3;
const int CC = 0;
const int max_iter = 1000;
const int max_no_improve_iter = 10000;
const int max_no_improve_search = 10;
const double alpha1 = 0.9;
const double alpha2 = 0.6;
const double alpha3 = 4.0;
const int alpha4 = 5;
const int bscore_weight = 1000;
const int pool_size = 50;

string filename;
int seed;
double time_limit;

// data
tms start;
tms finish;
int start_time;
double total_time;
double best_time;
int no_improve_search;
int mIter;
int last_id;

int order;
int is_sat;
int vertex_size;
int square_size;
int edge_size;

Edge* edges;
int** edge_map;
int* mVertexes;
int mVertexes_length;
int* mEdges;
int mEdges_length;
int** mNeighbours;
int* mNeighbours_length;

int*** mCol;
int*** mCol_pos;
int** mCol_length;
int*** mRow;
int*** mRow_pos;
int** mRow_length;
int*** mSqu;
int*** mSqu_pos;
int** mSqu_length;
bool** vertex_can_move;
int** vertex_color;
int* vertex_color_length;
int** vertex_color_pos;
int** color_vertex;
int* color_vertex_length;
int** color_vertex_pos;

int* mVertexesColor;
int* mVertexesColor_tmp;
int* mEdgesCout;

int clash_best;
int clash_cur;
int** mClash;
int** mTabu;
bool* mTabuV;
long long* vscore;
int* mClashEdges;
int* mClashEdges_tmp;
int* mClashEdges_pos;
int mClashEdges_length;
int mClashEdges_length_tmp;
int maxClashVertex;
int* mClashVertexes;
int* mClashVertexes_pos;
int mClashVertexes_length;
int maxClashTabuVertex;
int* mClashTabuVertexes;
int* mClashTabuVertexes_pos;
int mClashTabuVertexes_length;

int** mPoolVertexesColor;
int** mPoolVertexesClash;
int* mPoolMaxIter;
int** mPoolColor_length;
int** mPoolClash_length;
int mPool_length;

void set_Variable_size() {
	tmp_st = (int *) malloc(sizeof(int) * (vertex_size * square_size));
	tmp_tabu_st = (int *) malloc(sizeof(int) * (vertex_size * square_size));
	tmp_sub_st = (int *) malloc(sizeof(int) * (vertex_size * square_size));
	tmp_sub_tabu_st = (int *) malloc(sizeof(int) * (vertex_size * square_size));
	tmp_clash = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	tmp_bms_st = (int *) malloc(sizeof(int) * (max_bms_length + 1));
	tmp_sub_bms_st = (int *) malloc(sizeof(int) * (max_bms_length + 1));
	for(int i = 1; i <= vertex_size; i++) tmp_clash[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	tmp_vertex = (int **) malloc(sizeof(int *) * (square_size + 1));
	for (int i=1; i<=square_size; i++) tmp_vertex[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	tmp_flag = (bool *) malloc(sizeof(bool) * (vertex_size + 1));
    
	edges = (Edge *) malloc(sizeof(Edge) * (edge_size + 1));
	edge_map = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	mVertexes = (int *) malloc(sizeof(int) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) edge_map[i] = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mEdges = (int *) malloc(sizeof(int) * (edge_size + 1));
	mNeighbours = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) mNeighbours[i] = (int *) malloc(sizeof(int) * (3 * square_size));
	mNeighbours_length = (int *) malloc(sizeof(int) * (vertex_size + 1));

	mCol = (int ***) malloc(sizeof(int **) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) {
		mCol[i] = (int **) malloc(sizeof(int *) * (square_size + 1));
		for(int j = 1; j <= square_size; j++) mCol[i][j] = (int *) malloc(sizeof(int) * (square_size + 1));
	}
	mCol_pos = (int ***) malloc(sizeof(int **) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) {
		mCol_pos[i] = (int **) malloc(sizeof(int *) * (square_size + 1));
		for(int j = 1; j <= square_size; j++) mCol_pos[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 1));
	}
	mCol_length = (int **) malloc(sizeof(int *) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) mCol_length[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	mRow = (int ***) malloc(sizeof(int **) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) {
		mRow[i] = (int **) malloc(sizeof(int *) * (square_size + 1));
		for(int j = 1; j <= square_size; j++) mRow[i][j] = (int *) malloc(sizeof(int) * (square_size + 1));
	}
	mRow_pos = (int ***) malloc(sizeof(int **) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) {
		mRow_pos[i] = (int **) malloc(sizeof(int *) * (square_size + 1));
		for(int j = 1; j <= square_size; j++) mRow_pos[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 1));
	}
	mRow_length = (int **) malloc(sizeof(int *) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) mRow_length[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	mSqu = (int ***) malloc(sizeof(int **) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) {
		mSqu[i] = (int **) malloc(sizeof(int *) * (square_size + 1));
		for(int j = 1; j <= square_size; j++) mSqu[i][j] = (int *) malloc(sizeof(int) * (square_size + 1));
	}
	mSqu_pos = (int ***) malloc(sizeof(int **) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) {
		mSqu_pos[i] = (int **) malloc(sizeof(int *) * (square_size + 1));
		for(int j = 1; j <= square_size; j++) mSqu_pos[i][j] = (int *) malloc(sizeof(int) * (vertex_size + 1));
	}
	mSqu_length = (int **) malloc(sizeof(int *) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) mSqu_length[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	vertex_can_move = (bool **) malloc(sizeof(bool *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) vertex_can_move[i] = (bool *) malloc(sizeof(bool) * (square_size + 1));
	vertex_color = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) vertex_color[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	vertex_color_pos = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) vertex_color_pos[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	vertex_color_length = (int *) malloc(sizeof(int) * (vertex_size + 1));
	color_vertex = (int **) malloc(sizeof(int *) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) color_vertex[i] = (int *) malloc(sizeof(int) * (vertex_size + 1));
	color_vertex_pos = (int **) malloc(sizeof(int *) * (square_size + 1));
	for(int i = 1; i <= square_size; i++) color_vertex_pos[i] = (int *) malloc(sizeof(int) * (vertex_size + 1));
	color_vertex_length = (int *) malloc(sizeof(int) * (square_size + 1));

	mVertexesColor = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mVertexesColor_tmp = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mClash = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) mClash[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	mTabu = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) mTabu[i] = (int *) malloc(sizeof(long long) * (square_size + 1));
	mTabuV = (bool *) malloc(sizeof(bool) * (vertex_size + 1));
	vscore = (long long *) malloc(sizeof(long long) * (edge_size + 1));
    mClashEdges = (int *) malloc(sizeof(int) * (edge_size + 1));
	mClashEdges_tmp = (int *) malloc(sizeof(int) * (edge_size + 1));
    mClashEdges_pos = (int *) malloc(sizeof(int) * (edge_size + 1));
	mClashVertexes = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mClashVertexes_pos = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mClashTabuVertexes = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mClashTabuVertexes_pos = (int *) malloc(sizeof(int) * (vertex_size + 1));

	mPoolVertexesColor = (int **) malloc(sizeof(int *) * (pool_size));
	for(int i = 0; i < pool_size; i++) mPoolVertexesColor[i] = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mPoolVertexesClash = (int **) malloc(sizeof(int *) * (pool_size));
	for(int i = 0; i < pool_size; i++) mPoolVertexesClash[i] = (int *) malloc(sizeof(int) * (vertex_size + 1));
	mPoolColor_length = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) mPoolColor_length[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	mPoolClash_length = (int **) malloc(sizeof(int *) * (vertex_size + 1));
	for(int i = 1; i <= vertex_size; i++) mPoolClash_length[i] = (int *) malloc(sizeof(int) * (square_size + 1));
	mPoolMaxIter = (int *) malloc(sizeof(int) * (pool_size + 1));

	return;
}

int to_index(int index1, int index2, bool type) {
	// row, col
	if(type) return (index1-1)*square_size + index2;
	// squ, point
	int row_low = int((index1-1)/order)*order + int((index2-1)/order);
	int col = ((index1-1)%order)*order + (index2-1)%order + 1;
	return row_low*square_size + col;
}

void init() {
	for(int i = 1; i <= vertex_size; i++) {
		mNeighbours_length[i] = 0;
		vertex_color_length[i] = 0;
		mVertexesColor[i] = -1;
		for(int j = 1; j <= vertex_size; j++) edge_map[i][j] = -1;
		for(int j = 1; j <= square_size; j++) {
			mClash[i][j] = 0;
			vertex_color_pos[i][j] = square_size + 1;
			vertex_can_move[i][j] = false;
			mPoolColor_length[i][j] = 0;
			mPoolClash_length[i][j] = 0;
		}
	}
	for(int i = 1; i <= square_size; i++) for(int j = 1; j <= vertex_size; j++) color_vertex_pos[i][j] = 0;
	
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
	for(int i = 1; i <= vertex_size; i++) for(int j = 1; j <= square_size; j++) tmp_clash[i][j] = 0;

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

int get_score(int id, int to) {
	return mClash[id][to] - mClash[id][mVertexesColor[id]];
}

int get_nscore(int id, int to) {
	return 1;
}

int get_bscore(int id, int to) {
    if (color_vertex_length[to]-square_size < 0) return 1;
    else return -1;
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

void mRead(string filename) {
	string tempstr1;
	string tempstr2;
	FILE *fp = freopen(filename.c_str(), "r", stdin); 
	int index = 0;
	cin >> order;
	cin >> is_sat;
	square_size = order*order;
	vertex_size = square_size*square_size;
	edge_size = square_size*square_size*(3*square_size-2*order-1)/2;
	set_Variable_size();
	init();
	for (int i=1; i<=square_size; i++) {
		for (int j=1; j<=square_size; j++) {
			cin >> tmp_vertex[i][j];
			int x = to_index(i, j, true);
			if (tmp_vertex[i][j] == -1) {
				for(int k=1; k<=square_size; k++) {
					vertex_color[x][vertex_color_length[x]] = k; 
					vertex_color_pos[x][k] = vertex_color_length[x]++;
					vertex_can_move[x][k] = true;
				}
			} else {
				vertex_color[x][vertex_color_length[x]] = tmp_vertex[i][j]; 
				vertex_color_pos[x][tmp_vertex[i][j]] = vertex_color_length[x]++;
				vertex_can_move[x][tmp_vertex[i][j]] = true;
			}
		}
	}
	for (int row=1; row<=square_size; row++) {
		for (int col1=1; col1<square_size; col1++) {
			for (int col2=col1+1; col2<=square_size; col2++) {
				int x = to_index(row, col1, true);
				int y = to_index(row, col2, true);
				mNeighbours[x][mNeighbours_length[x]++] = y;
				mNeighbours[y][mNeighbours_length[y]++] = x;
				edges[++index].x = x;
				edges[index].y = y;
				edges[index].w = 1;
				edge_map[x][y] = index;
				edge_map[y][x] = index;
			}
		}
	}
	for (int col=1; col<=square_size; col++) {
		for (int row1=1; row1<square_size; row1++) {
			for (int row2=row1+1; row2<=square_size; row2++) {
				int x = to_index(row1, col, true);
				int y = to_index(row2, col, true);
				mNeighbours[x][mNeighbours_length[x]++] = y;
				mNeighbours[y][mNeighbours_length[y]++] = x;
				edges[++index].x = x;
				edges[index].y = y;
				edges[index].w = 1;
				edge_map[x][y] = index;
				edge_map[y][x] = index;
			}
		}
	}
	for (int squ=1; squ<=square_size; squ++) {
		for (int point1=1; point1<square_size; point1++) {
			for (int point2=point1+1; point2<=square_size; point2++) {
				int x = to_index(squ, point1, false);
				int y = to_index(squ, point2, false);
				if (edge_map[x][y] == -1) {
					mNeighbours[x][mNeighbours_length[x]++] = y;
					mNeighbours[y][mNeighbours_length[y]++] = x;
					edges[++index].x = x;
					edges[index].y = y;
					edges[index].w = 1;
					edge_map[x][y] = index;
					edge_map[y][x] = index;
				}
				// } else edges[edge_map[x][y]].w++;
			}
		}
	}
	return;
}

void check_answer() {
	bool flag = true;
	for (int i=0; i<mVertexes_length; i++) {
		mVertexesColor[mVertexes[i]] = mVertexesColor_tmp[mVertexes[i]];
	}
	for(int i = 1; i <= edge_size; i++) {
		if(mVertexesColor[edges[i].x] == mVertexesColor[edges[i].y]) {
			cout << "answer wrong same color " << edges[i].x << " " << edges[i].y << " " << mVertexesColor[edges[i].x] << " " << mVertexesColor[edges[i].y] << endl;
			flag = false;
		}
	}
	for(int i = 1; i <= vertex_size; i++) {
		if(mVertexesColor[i] == -1) {
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

bool check_finish() {
	if(clash_best == 0) {
		mCurrentTime();
		best_time = total_time;
		check_answer();
		printf("%s find answer: %lf\n", filename.c_str(), best_time);
		return true;
	}
	return false;
}

bool check_tabu(int id, int to, int iter) {
	if (mTabu[id][to] <= iter) return true;
	return false;
}

void mReduceVertexes() {
	for (int i=1; i<=vertex_size; i++) {
		if (vertex_color_length[i] == 1) {
			int tmp_c = vertex_color[i][0];
			vertex_color_length[i] = 0;
			mVertexesColor[i] = tmp_c;
			color_vertex[tmp_c][color_vertex_length[tmp_c]] = i;
			color_vertex_pos[tmp_c][i] = color_vertex_length[tmp_c]++;
			vertex_can_move[i][tmp_c] = false;
			for (int j=1; j<=square_size; j++) {
				if(mCol_pos[get_Col(i)][j][i] <= mCol_length[get_Col(i)][j]) swap_col(get_Col(i), j, i, mCol_length[get_Col(i)][j]--);
				if(mRow_pos[get_Row(i)][j][i] <= mRow_length[get_Row(i)][j]) swap_row(get_Row(i), j, i, mRow_length[get_Row(i)][j]--);
				if(mSqu_pos[get_Squ(i)][j][i] <= mSqu_length[get_Squ(i)][j]) swap_squ(get_Squ(i), j, i, mSqu_length[get_Squ(i)][j]--);
			}
			swap_col(get_Col(i), tmp_c, i, 1);
			mCol_length[get_Col(i)][tmp_c] = 0;
			swap_row(get_Row(i), tmp_c, i, 1);
			mRow_length[get_Row(i)][tmp_c] = 0;
			swap_squ(get_Squ(i), tmp_c, i, 1);
			mSqu_length[get_Squ(i)][tmp_c] = 0;
			for(int j = 0; j < mNeighbours_length[i]; j++) {
				int tmp_V = mNeighbours[i][j];
				if(vertex_color_pos[tmp_V][tmp_c] < vertex_color_length[tmp_V])
					swap_vertex_color(tmp_V, tmp_c, --vertex_color_length[tmp_V]);
				if(mCol_pos[get_Col(tmp_V)][tmp_c][tmp_V] <= mCol_length[get_Col(tmp_V)][tmp_c])
					swap_col(get_Col(tmp_V), tmp_c, tmp_V, mCol_length[get_Col(tmp_V)][tmp_c]--);
				if(mRow_pos[get_Row(tmp_V)][tmp_c][tmp_V] <= mRow_length[get_Row(tmp_V)][tmp_c])
					swap_row(get_Row(tmp_V), tmp_c, tmp_V, mRow_length[get_Row(tmp_V)][tmp_c]--);
				if(mSqu_pos[get_Squ(tmp_V)][tmp_c][tmp_V] <= mSqu_length[get_Squ(tmp_V)][tmp_c])
					swap_squ(get_Squ(tmp_V), tmp_c, tmp_V, mSqu_length[get_Squ(tmp_V)][tmp_c]--);
			}
		}
	}
	queue<int> q;
	for(int i = 1; i <= vertex_size; i++) {
		if(vertex_color_length[i] == 1) q.push(i);
	}
	for(int i = 1; i <= square_size; i++) {
		for(int j = 1; j <= square_size; j++) {
			if(mCol_length[i][j] == 1) {
				int tmp_v = mCol[i][j][1];
				swap_vertex_color(tmp_v, j, 0);
				q.push(tmp_v);
			}	
			if(mRow_length[i][j] == 1) {
				int tmp_v = mRow[i][j][1];
				swap_vertex_color(tmp_v, j, 0);
				q.push(tmp_v);
			}
			if(mSqu_length[i][j] == 1) {
				int tmp_v = mSqu[i][j][1];
				swap_vertex_color(tmp_v, j, 0);
				q.push(tmp_v);
			}
		}
	}
	while(!q.empty()) {
		int tmp_v = q.front();
		int tmp_c = vertex_color[tmp_v][0];
		q.pop();
		if(mVertexesColor[tmp_v] > 0) continue;
		vertex_color_length[tmp_v] = 0;
		color_vertex[tmp_c][color_vertex_length[tmp_c]] = tmp_v;
		color_vertex_pos[tmp_c][tmp_v] = color_vertex_length[tmp_c]++;
		mVertexesColor[tmp_v] = tmp_c;
		vertex_can_move[tmp_v][tmp_c] = false;
		mCol_length[get_Col(tmp_v)][tmp_c] = 0;
		mRow_length[get_Row(tmp_v)][tmp_c] = 0;
		mSqu_length[get_Squ(tmp_v)][tmp_c] = 0;
		for(int i = 1; i <= square_size; i++) {
			if(mCol_pos[get_Col(tmp_v)][i][tmp_v] <= mCol_length[get_Col(tmp_v)][i]) {
				swap_col(get_Col(tmp_v), i, tmp_v, mCol_length[get_Col(tmp_v)][i]--);
				if(mCol_length[get_Col(tmp_v)][i] == 1) {
					int v = mCol[get_Col(tmp_v)][i][1];
					swap_vertex_color(v, i, 0);
					q.push(v);
				}
			}
			if(mRow_pos[get_Row(tmp_v)][i][tmp_v] <= mRow_length[get_Row(tmp_v)][i]) {
				swap_row(get_Row(tmp_v), i, tmp_v, mRow_length[get_Row(tmp_v)][i]--);
				if(mRow_length[get_Row(tmp_v)][i] == 1) {
					int v = mRow[get_Row(tmp_v)][i][1];
					swap_vertex_color(v, i, 0);
					q.push(v);
				}
			}
			if(mSqu_pos[get_Squ(tmp_v)][i][tmp_v] <= mSqu_length[get_Squ(tmp_v)][i]) {
				swap_squ(get_Squ(tmp_v), i, tmp_v, mSqu_length[get_Squ(tmp_v)][i]--);
				if(mSqu_length[get_Squ(tmp_v)][i] == 1) {
					int v = mSqu[get_Squ(tmp_v)][i][1];
					swap_vertex_color(v, i, 0);
					q.push(v);
				}
			}
		}
		for(int i = 0; i < mNeighbours_length[tmp_v]; i++) {
			int v = mNeighbours[tmp_v][i];
			if(vertex_color_pos[v][tmp_c] < vertex_color_length[v]) {
				swap_vertex_color(v, tmp_c, vertex_color_length[v] - 1);
				vertex_color_length[v]--;
				if(vertex_color_length[v] == 1) {
					q.push(v);
				}
			}
			if(mCol_pos[get_Col(v)][tmp_c][v] <= mCol_length[get_Col(v)][tmp_c]) {
				if(mCol_length[get_Col(v)][tmp_c] != 0) swap_col(get_Col(v), tmp_c, v, mCol_length[get_Col(v)][tmp_c]--);
				if(mCol_length[get_Col(v)][tmp_c] == 1) {
					int v1 = mCol[get_Col(v)][tmp_c][1];
					swap_vertex_color(v1, tmp_c, 0);
					q.push(v1);
				}
			}
			if(mRow_pos[get_Row(v)][tmp_c][v] <= mRow_length[get_Row(v)][tmp_c]) {
				if(mRow_length[get_Row(v)][tmp_c] != 0) swap_row(get_Row(v), tmp_c, v, mRow_length[get_Row(v)][tmp_c]--);
				if(mRow_length[get_Row(v)][tmp_c] == 1) {
					int v1 = mRow[get_Row(v)][tmp_c][1];
					swap_vertex_color(v1, tmp_c, 0);
					q.push(v1);
				}
			}
			if(mSqu_pos[get_Squ(v)][tmp_c][v] <= mSqu_length[get_Squ(v)][tmp_c]) {
				if(mSqu_length[get_Squ(v)][tmp_c] != 0) swap_squ(get_Squ(v), tmp_c, v, mSqu_length[get_Squ(v)][tmp_c]--);
				if(mSqu_length[get_Squ(v)][tmp_c] == 1) {
					int v1 = mSqu[get_Squ(v)][tmp_c][1];
					swap_vertex_color(v1, tmp_c, 0);
					q.push(v1);
				}
			}
			vertex_can_move[v][tmp_c] = false;
		}
	}

	mVertexes_length = 0;
	mEdges_length = 0;
	for(int i = 1; i <= vertex_size; i++) {
		if (mVertexesColor[i] < 0) {
			mVertexes[mVertexes_length++] = i;
		}
	}
	for(int i = 0; i < mVertexes_length; i++) {
		int vertex_id = mVertexes[i];
		for(int j = 0; j < mNeighbours_length[vertex_id]; j++) {
			int tmp_v = mNeighbours[vertex_id][j];
			if(mVertexesColor[tmp_v] > 0) {
				mNeighbours[vertex_id][j--] = mNeighbours[vertex_id][--mNeighbours_length[vertex_id]];
			} else if (vertex_id < tmp_v) {
                mEdges[mEdges_length++] = edge_map[vertex_id][tmp_v];
			}
		}
	}
	return;
}

void update_info(int id, int pos) {
	int cur_pos = mVertexesColor[id];
	for (int i = 0; i < mNeighbours_length[id]; i++) {
		int tmp_v = mNeighbours[id][i];
		int tmp_c = mVertexesColor[tmp_v];
		// mClash[tmp_v][cur_pos] -= edges[edge_map[id][tmp_v]].w;
		// mClash[tmp_v][pos] += edges[edge_map[id][tmp_v]].w;
		bool flag = (mClash[tmp_v][tmp_c] >= vscore[tmp_v]);
		mClash[tmp_v][cur_pos] -= 1;
		// cout << "vscore[" << tmp_v << "]: " << vscore[tmp_v] << ", mClash: " << mClash[tmp_v][tmp_c] << endl;
		// cout << "mClash[" << tmp_v << "][" << cur_pos << "]: " << mClash[tmp_v][cur_pos]+1 << "->" << mClash[tmp_v][cur_pos] << endl;
		// if (tmp_c != cur_pos && mClash[tmp_v][cur_pos] < vscore[tmp_v]) {
		// 	cout << "update1 tmp_v: " << tmp_v << "(" << mClash[tmp_v][tmp_c] << ", " << vscore[tmp_v] << "), vscore from " << vscore[tmp_v] << " to " << mClash[tmp_v][cur_pos] << endl;
		// 	vscore[tmp_v] = mClash[tmp_v][cur_pos];
		// }
		mClash[tmp_v][pos] += 1;
		// cout << "mClash[" << tmp_v << "][" << pos << "]: " << mClash[tmp_v][pos]-1 << "->" << mClash[tmp_v][pos] << endl;
		// if (tmp_c != pos && mClash[tmp_v][pos]-1 == vscore[tmp_v]) {
		// 	vscore[tmp_v] = INT_MAX;
		// 	for (int j=0; j<vertex_color_length[tmp_v]; j++) {
		// 		if (tmp_c == vertex_color[tmp_v][j]) continue;
		// 		int tmp_clash = mClash[tmp_v][vertex_color[tmp_v][j]];
		// 		if (tmp_clash < vscore[tmp_v]) vscore[tmp_v] = tmp_clash;
		// 	}
		// 	cout << "update2 tmp_v: " << tmp_v << "(" << mClash[tmp_v][tmp_c] << ", " << vscore[tmp_v] << "), vscore from " << mClash[tmp_v][pos]-1 << " to " << vscore[tmp_v] << endl;
		// }

		vscore[tmp_v] = INT_MAX;
		for (int j=0; j<vertex_color_length[tmp_v]; j++) {
			if (tmp_c == vertex_color[tmp_v][j]) continue;
			int tmp_clash = mClash[tmp_v][vertex_color[tmp_v][j]];
			if (tmp_clash < vscore[tmp_v]) vscore[tmp_v] = tmp_clash;
		}

		if (!flag && mClash[tmp_v][tmp_c] >= vscore[tmp_v]) {
			if (mTabuV[tmp_v]) {
				// if (mClash[tmp_v][mVertexesColor[tmp_v]]-vscore[tmp_v] > maxClashVertex) {
				// 	maxClashVertex = vscore[tmp_v];
				// }
				// assert(mClashVertexes_pos[tmp_v] >= mClashVertexes_length || mClashVertexes[mClashVertexes_pos[tmp_v]] != tmp_v);
				mClashVertexes[mClashVertexes_length] = tmp_v;
				mClashVertexes_pos[tmp_v] = mClashVertexes_length++;
				cout << "update3 tmp_v: " << tmp_v << ", insert into mClashVertexes." << endl;
			} else {
				// if (mClash[tmp_v][mVertexesColor[tmp_v]]-vscore[tmp_v] > maxClashTabuVertex) {
				// 	maxClashTabuVertex = vscore[tmp_v];
				// }
				// assert(mClashTabuVertexes_pos[tmp_v] >= mClashTabuVertexes_length || mClashTabuVertexes[mClashTabuVertexes_pos[tmp_v]] != tmp_v);
				mClashTabuVertexes[mClashTabuVertexes_length] = tmp_v;
				mClashTabuVertexes_pos[tmp_v] = mClashTabuVertexes_length++;
				cout << "update4 tmp_v: " << tmp_v << ", insert into mClashTabuVertexes." << endl;
			}
		} else if (flag && mClash[tmp_v][tmp_c] < vscore[tmp_v]) {
			if (mTabuV[tmp_v]) {
				// assert(mClashVertexes[mClashVertexes_pos[tmp_v]] == tmp_v);
				mClashVertexes[mClashVertexes_pos[tmp_v]] = mClashVertexes[--mClashVertexes_length];
				mClashVertexes_pos[mClashVertexes[mClashVertexes_length]] = mClashVertexes_pos[tmp_v];
				cout << "update5 tmp_v: " << tmp_v << "(" << mClash[tmp_v][tmp_c] << "), delete from mClashVertexes." << endl;
			} else {
				// assert(mClashTabuVertexes[mClashTabuVertexes_pos[tmp_v]] == tmp_v);
				mClashTabuVertexes[mClashTabuVertexes_pos[tmp_v]] = mClashTabuVertexes[--mClashTabuVertexes_length];
				mClashTabuVertexes_pos[mClashTabuVertexes[mClashTabuVertexes_length]] = mClashTabuVertexes_pos[tmp_v];
				cout << "update6 tmp_v: " << tmp_v << ", delete from mClashTabuVertexes." << endl;
			}
		}


		int edge_id = edge_map[id][tmp_v];
		if (tmp_c == cur_pos) {
            mClashEdges[mClashEdges_pos[edge_id]] = mClashEdges[--mClashEdges_length];
            mClashEdges_pos[mClashEdges[mClashEdges_length]] = mClashEdges_pos[edge_id];
		} else if (tmp_c == pos) {
            mClashEdges[mClashEdges_length] = edge_id;
			mClashEdges_pos[edge_id] = mClashEdges_length++;
			if (!mTabuV[tmp_v]) {
				mTabuV[tmp_v] = true;
				if (mClash[tmp_v][tmp_c] >= vscore[tmp_v]) {
					// assert(mClashTabuVertexes[mClashTabuVertexes_pos[tmp_v]] == tmp_v);
					// assert(mClashVertexes_pos[tmp_v] >= mClashVertexes_length || mClashVertexes[mClashVertexes_pos[tmp_v]] != tmp_v);
					mClashTabuVertexes[mClashTabuVertexes_pos[id]] = mClashTabuVertexes[--mClashTabuVertexes_length];
					mClashTabuVertexes_pos[mClashTabuVertexes[mClashTabuVertexes_length]] = mClashTabuVertexes_pos[id];
					mClashVertexes[mClashVertexes_length] = id;
					mClashVertexes_pos[id] = mClashVertexes_length++;
					cout << "update6 tmp_v: " << tmp_v << ", from mClashTabuVertexes to mClashVertexes." << endl;
				}
			}	
		}
	}
	mVertexesColor[id] = pos;

	vscore[id] = INT_MAX;
	for (int j=0; j<vertex_color_length[id]; j++) {
		if (mVertexesColor[id] == vertex_color[id][j]) continue;
		int tmp_clash = mClash[id][vertex_color[id][j]];
		if (tmp_clash < vscore[id]) vscore[id] = tmp_clash;
	}

	if (mTabuV[id]) {
		mTabuV[id] = false;
		// assert(mClashVertexes[mClashVertexes_pos[id]] == id);
		mClashVertexes[mClashVertexes_pos[id]] = mClashVertexes[--mClashVertexes_length];
		mClashVertexes_pos[mClashVertexes[mClashVertexes_length]] = mClashVertexes_pos[id];
		
		if (mClash[id][pos] >= vscore[id]) {
			// assert(mClashTabuVertexes_pos[id] >= mClashTabuVertexes_length || mClashTabuVertexes[mClashTabuVertexes_pos[id]] != id);
			mClashTabuVertexes[mClashTabuVertexes_length] = id;
			mClashTabuVertexes_pos[id] = mClashTabuVertexes_length++;
			cout << "update7 vertex_id: " << id << ", from mClashVertexes to mClashTabuVertexes." << endl;
		} else {
			cout << "update8 vertex_id: " << id << ", delete from mClashVertexes." << endl;
		}
	} else if (mClash[id][pos] < vscore[id]) {
		// assert(mClashTabuVertexes[mClashTabuVertexes_pos[id]] == id);
		mClashTabuVertexes[mClashTabuVertexes_pos[id]] = mClashTabuVertexes[--mClashTabuVertexes_length];
		mClashTabuVertexes_pos[mClashTabuVertexes[mClashTabuVertexes_length]] = mClashTabuVertexes_pos[id];
		cout << "update9 vertex_id: " << id << ", delete from mClashTabuVertexes." << endl;
	}


	// mClashVertexes_length = 0;
	// mClashTabuVertexes_length = 0;
	// for (int i = 0; i < mVertexes_length; i++) {
	// 	int vertex_id = mVertexes[i];
	// 	vscore[vertex_id] = INT_MAX;
	// 	for (int j=0; j<vertex_color_length[vertex_id]; j++) {
	// 		if (mVertexesColor[vertex_id] == vertex_color[vertex_id][j]) continue;
	// 		int tmp_clash = mClash[vertex_id][vertex_color[vertex_id][j]];
	// 		if (tmp_clash < vscore[vertex_id]) vscore[vertex_id] = tmp_clash;
	// 	}
	// 	if (mClash[vertex_id][mVertexesColor[vertex_id]] >= vscore[vertex_id]) {
	// 		if (mTabuV[vertex_id]) {
	// 			mClashVertexes[mClashVertexes_length] = vertex_id;
	// 			mClashVertexes_pos[vertex_id] = mClashVertexes_length++;
	// 		} else {
	// 			mClashTabuVertexes[mClashTabuVertexes_length] = vertex_id;
	// 			mClashTabuVertexes_pos[vertex_id] = mClashTabuVertexes_length++;
	// 		}
	// 	}
	// }


	// cout << "------------------------------------------" << endl;
	// cout << "vscore" << endl;
	// for (int i=0; i<mVertexes_length; i++) {
	// 	cout << "vertex_id: " << mVertexes[i] << ", vscore: " << vscore[mVertexes[i]] << endl;
	// }
	cout << "------------------------------------------" << endl;
	cout << "mClashVertexes_length: " << mClashVertexes_length << endl;
	for (int i = 0; i < mClashVertexes_length; i++) {
		cout << mClashVertexes[i] << ": " << vscore[mClashVertexes[i]]-mClash[mClashVertexes[i]][mVertexesColor[mClashVertexes[i]]] << ", tabuV: " << mTabuV[mClashVertexes[i]] << ", vscore: " << vscore[mClashVertexes[i]] << endl;
	}
	cout << "------------------------------------------" << endl;

	cout << "mClashTabuVertexes_length: " << mClashTabuVertexes_length << endl;
	for (int i = 0; i < mClashTabuVertexes_length; i++) {
		cout << mClashTabuVertexes[i] << ": " << vscore[mClashTabuVertexes[i]]-mClash[mClashTabuVertexes[i]][mVertexesColor[mClashTabuVertexes[i]]] << ", tabuV: " << mTabuV[mClashTabuVertexes[i]] << ", vscore: " << vscore[mClashTabuVertexes[i]] << endl;
	}


	// for (int j=0; j<vertex_color_length[id]; j++) {
	// 	if (mVertexesColor[id] == vertex_color[id][j]) continue;
	// 	int tmp_clash = mClash[id][vertex_color[id][j]];
	// 	if (tmp_clash < vscore[id]) vscore[id] = tmp_clash;
	// }
	// if (mClash[id][pos] == 0 && mClashVertexes_pos[id] >= 0) {
	// 	mClashVertexes[mClashVertexes_pos[id]] = mClashVertexes[--mClashVertexes_length];
	// 	mClashVertexes_pos[mClashVertexes[mClashVertexes_length]] = mClashVertexes_pos[id];
	// 	mClashVertexes_pos[id] = -1;
	// }
	return;
}

void build() {
	for(int i=0; i<mVertexes_length; i++) {
		vscore[mVertexes[i]] = INT_MAX;
		mTabuV[mVertexes[i]] = true;
		for (int j = 1; j <= square_size; j++) {
			mClash[mVertexes[i]][j] = 0;
			mTabu[mVertexes[i]][j] = 0;
		}
	}
	mClashEdges_length = 0;
	for (int i = 0; i < mEdges_length; i++) {
        int edge_id = mEdges[i];
		// mClash[edges[edge_id].x][mVertexesColor[edges[edge_id].y]] += edges[edge_id].w;
		// mClash[edges[edge_id].y][mVertexesColor[edges[edge_id].x]] += edges[edge_id].w;
		mClash[edges[edge_id].x][mVertexesColor[edges[edge_id].y]] += 1;
		mClash[edges[edge_id].y][mVertexesColor[edges[edge_id].x]] += 1;
		if(mVertexesColor[edges[edge_id].x] == mVertexesColor[edges[edge_id].y]) {
            mClashEdges[mClashEdges_length] = edge_id;
            mClashEdges_pos[edge_id] = mClashEdges_length++;
		}
	}
	// maxClashVertex = INT_MIN;
	// maxClashTabuVertex = INT_MIN;
	mClashVertexes_length = 0;
	mClashTabuVertexes_length = 0;
	for (int i = 0; i < mVertexes_length; i++) {
		int vertex_id = mVertexes[i];
		for (int j=0; j<vertex_color_length[vertex_id]; j++) {
			if (mVertexesColor[vertex_id] == vertex_color[vertex_id][j]) continue;
			int tmp_clash = mClash[vertex_id][vertex_color[vertex_id][j]];
			if (tmp_clash < vscore[vertex_id]) vscore[vertex_id] = tmp_clash;
			cout << "mClash[" << vertex_id << "][" << vertex_color[vertex_id][j] << "]: " << tmp_clash << endl;
		}
		cout << "vscore[" << vertex_id << "]: " << vscore[vertex_id] << endl;
		if (mClash[vertex_id][mVertexesColor[vertex_id]] >= vscore[vertex_id]) {
			// if (mClash[vertex_id][mVertexesColor[vertex_id]]-vscore[vertex_id] > maxClashVertex) {
			// 	maxClashVertex = vscore[vertex_id];
			// }
			mClashVertexes[mClashVertexes_length] = vertex_id;
			mClashVertexes_pos[vertex_id] = mClashVertexes_length++;
			// if (mTabuV[vertex_id]) {
			// 	if (mClash[vertex_id][mVertexesColor[vertex_id]]-vscore[vertex_id] > maxClashVertex) {
			// 		maxClashVertex = vscore[vertex_id];
			// 	}
			// 	mClashVertexes[mClashVertexes_length] = vertex_id;
			// 	mClashVertexes_pos[vertex_id] = mClashVertexes_length++;
			// } else {
			// 	if (mClash[vertex_id][mVertexesColor[vertex_id]]-vscore[vertex_id] > maxClashTabuVertex) {
			// 		maxClashTabuVertex = vscore[vertex_id];
			// 	}
			// 	mClashTabuVertexes[mClashTabuVertexes_length] = vertex_id;
			// 	mClashTabuVertexes_pos[vertex_id] = mClashTabuVertexes_length++;
			// }
		}

		// vscore[vertex_id] = mClash[vertex_id][mVertexesColor[vertex_id]] - vscore[vertex_id];

		// if (mTabuV[vertex_id]) {
		// 	if (vscore[vertex_id] > maxClashVertex) {
		// 		maxClashVertex = vscore[vertex_id];
		// 		mClashVertexes_length = 0;
		// 		mClashVertexes[mClashVertexes_length] = vertex_id;
		// 		mClashVertexes_pos[vertex_id] = mClashVertexes_length++;
		// 	} else if (vscore[vertex_id] == maxClashVertex) {
		// 		mClashVertexes[mClashVertexes_length] = vertex_id;
		// 		mClashVertexes_pos[vertex_id] = mClashVertexes_length++;
		// 	}
		// } else {
		// 	if (vscore[vertex_id] > maxClashTabuVertex) {
		// 		maxClashTabuVertex = vscore[vertex_id];
		// 		mClashTabuVertexes_length = 0;
		// 		mClashTabuVertexes[mClashTabuVertexes_length] = vertex_id;
		// 		mClashTabuVertexes_pos[vertex_id] = mClashTabuVertexes_length++;
		// 	} else if (vscore[vertex_id] == maxClashTabuVertex) {
		// 		mClashTabuVertexes[mClashTabuVertexes_length] = vertex_id;
		// 		mClashTabuVertexes_pos[vertex_id] = mClashTabuVertexes_length++;
		// 	}
		// }
	}

	cout << "------------------------------------------" << endl;
	cout << "mClashVertexes_length: " << mClashVertexes_length << endl;
	for (int i = 0; i < mClashVertexes_length; i++) {
		cout << mClashVertexes[i] << ": " << vscore[mClashVertexes[i]]-mClash[mClashVertexes[i]][mVertexesColor[mClashVertexes[i]]] << ", tabuV: " << mTabuV[mClashVertexes[i]] << ", vscore: " << vscore[mClashVertexes[i]] << endl;
	}
	cout << "------------------------------------------" << endl;

	cout << "mClashTabuVertexes_length: " << mClashTabuVertexes_length << endl;
	for (int i = 0; i < mClashTabuVertexes_length; i++) {
		cout << mClashTabuVertexes[i] << ": " << vscore[mClashTabuVertexes[i]]-mClash[mClashTabuVertexes[i]][mVertexesColor[mClashTabuVertexes[i]]] << ", tabuV: " << mTabuV[mClashTabuVertexes[i]] << ", vscore: " << vscore[mClashTabuVertexes[i]] << endl;
	}


	return;
}

int mTabuSearch() {
	int tSminClash;
	build();
	tSminClash = mClashEdges_length;
	if (tSminClash == 0) return tSminClash;
	for(int i = 0; i < mVertexes_length; i++) mVertexesColor_tmp[mVertexes[i]] = mVertexesColor[mVertexes[i]];
	// int no_improve_iter = 0;
	int r;
	int min_clash;
	int min_tabu_clash;
	int cnt;
	int tabu_cnt;
	int move_id;
	int move_to;
	int move_from;
	long long max_cs;
	int cs_cnt;
	// int min_sub_clash;
	// int min_sub_tabu_clash;
	// int sub_cnt;
	// int sub_tabu_cnt;
	// int max_bs;
	// int v_cnt;
	// int bs_cnt;
	for (int iters=1; iters<=mPoolMaxIter[last_id]; iters++) {
		min_clash = INT_MAX;
		min_tabu_clash = INT_MAX;
		max_cs = INT_MIN;
		cnt = 0;
		tabu_cnt = 0;
		// min_sub_clash = INT_MAX;
		// min_sub_tabu_clash = INT_MAX;
		// max_bs = INT_MIN;
		// sub_cnt = 0;
		// sub_tabu_cnt = 0;
		for (int i=0; i<mClashVertexes_length; i++) {
			int vertex_id = mClashVertexes[i];
			// if (mClash[vertex_id][mVertexesColor[vertex_id]]-vscore[vertex_id] < maxClashVertex) continue;
			for(int j=0; j<vertex_color_length[vertex_id]; j++) {
				int tmp_c = vertex_color[vertex_id][j];
				if (tmp_c == mVertexesColor[vertex_id]) continue;
				int tmp_s = get_score(vertex_id, tmp_c);
				if (tmp_s < min_clash) {
					min_clash = tmp_s;
					cnt = 0;
					tmp_st[cnt] = vertex_id;
					tmp_sub_st[cnt++] = tmp_c;
				} else if (tmp_s == min_clash) {
					tmp_st[cnt] = vertex_id;
					tmp_sub_st[cnt++] = tmp_c;
				}
			}
		}
		for (int i=0; i<mClashTabuVertexes_length; i++) {
			int vertex_id = mClashTabuVertexes[i];
			// if (mClash[vertex_id][mVertexesColor[vertex_id]]-vscore[vertex_id] < maxClashTabuVertex) continue;
			for(int j=0; j<vertex_color_length[vertex_id]; j++) {
				int tmp_c = vertex_color[vertex_id][j];
				if (tmp_c == mVertexesColor[vertex_id]) continue;
				int tmp_s = get_score(vertex_id, tmp_c);
				if (tmp_s < min_tabu_clash) {
					tabu_cnt = 0;
					min_tabu_clash = tmp_s;
					tmp_tabu_st[tabu_cnt] = vertex_id;
					tmp_sub_tabu_st[tabu_cnt] = tmp_c;
					tabu_cnt++;
				} else if (tmp_s == min_tabu_clash) {
					tmp_tabu_st[tabu_cnt] = vertex_id;
					tmp_sub_tabu_st[tabu_cnt] = tmp_c;
					tabu_cnt++;
				}
			}
		}
		cout << "=============================================" << endl;
		cout << "min_clash: " << min_clash << ", min_tabu_clash: " << min_tabu_clash << endl;
		cout << "cnt: " << cnt << ", tabu_cnt: " << tabu_cnt << endl;
		if (cnt == 0 && tabu_cnt == 0) {
			return tSminClash;
		} else if ((tabu_cnt > 0 && min_tabu_clash < min_clash && (mClashEdges_length + min_tabu_clash < tSminClash)) || (cnt == 0 && tabu_cnt > 0)) {
			int v_cnt = min(max_bms_length, tabu_cnt);
			for(int i = 0; i < v_cnt; i++) {
				r = rand() % tabu_cnt;
				long long tmp_cs = get_nscore(tmp_tabu_st[r], tmp_sub_tabu_st[r]);
				if (max_cs < tmp_cs) {
					cs_cnt = 0;
					max_cs = tmp_cs;
					tmp_bms_st[cs_cnt] = tmp_tabu_st[r];
					tmp_sub_bms_st[cs_cnt++] = tmp_sub_tabu_st[r];
				} else if (max_cs == tmp_cs) {
					tmp_bms_st[cs_cnt] = tmp_tabu_st[r];
					tmp_sub_bms_st[cs_cnt++] = tmp_sub_tabu_st[r];
				}
			}
		} else {
			int v_cnt = min(max_bms_length, cnt);
			for(int i = 0; i < v_cnt; i++) {
				r = rand() % cnt;
				long long tmp_cs = get_nscore(tmp_st[r], tmp_sub_st[r]);
				if (max_cs < tmp_cs) {
					cs_cnt = 0;
					max_cs = tmp_cs;
					tmp_bms_st[cs_cnt] = tmp_st[r];
					tmp_sub_bms_st[cs_cnt++] = tmp_sub_st[r];
				} else if (max_cs == tmp_cs) {
					tmp_bms_st[cs_cnt] = tmp_st[r];
					tmp_sub_bms_st[cs_cnt++] = tmp_sub_st[r];
				}
			}
		}

		r = rand() % cs_cnt;
		move_id = tmp_bms_st[r];
		move_to = tmp_sub_bms_st[r];
		mTabu[move_id][mVertexesColor[move_id]] = iters + rand() % tabuStep + alpha2 * mClashVertexes_length;
		cout << "choose vertex " << move_id << " change to " << move_to << ", tabuV:" << mTabuV[move_id] << endl;
		// cout << "mClashEdges_length: " << mClashEdges_length << ", mClashVertexes_length: " << mClashVertexes_length << ", mClashTabuVertexes_length: " << mClashTabuVertexes_length << endl;
		// cout << "total: " << mClashVertexes_length + mClashTabuVertexes_length << endl;
		update_info(move_id, move_to);
		if (mClashEdges_length <= tSminClash) {
			tSminClash = mClashEdges_length;
			for (int i=0; i<mVertexes_length; i++) mVertexesColor_tmp[mVertexes[i]] = mVertexesColor[mVertexes[i]]; 
			for (int i=0; i<mClashEdges_length; i++) mClashEdges_tmp[i] = mClashEdges[i];
			if(tSminClash == 0) return tSminClash;
		}
	}
	return tSminClash;
}

bool check_diff() {
	bool flag = false;
	for (int j=0; j<mVertexes_length; j++) {
		if (mPoolVertexesColor[last_id][mVertexes[j]] != mVertexesColor_tmp[mVertexes[j]]) {
			flag = true;
			break;
		} 
	}
	if (!flag) return false;
	for (int i=0; i<mPool_length; i++) {
		flag = false;
		for (int j=0; j<mVertexes_length; j++) {
			if (mPoolVertexesColor[i][mVertexes[j]] != mVertexesColor_tmp[mVertexes[j]]) {
				flag = true;
				break;
			} 
		}
		if (!flag) return false;
	}
	return true;
}

bool mLocalSearch() {
	mIter++;
	clash_cur = mTabuSearch();
	if(clash_cur < clash_best) {
		mPool_length = 0;
		clash_best = clash_cur;
		for (int i=0; i<mVertexes_length; i++) mPoolVertexesColor[mPool_length][mVertexes[i]] = mVertexesColor_tmp[mVertexes[i]];
		for (int i=0; i<mEdges_length; i++) edges[mEdges[i]].w = 1;
		mPoolMaxIter[mPool_length] = max_iter;
		mPool_length++;
	} else if (clash_cur == clash_best && check_diff()) {
		if (mPool_length < pool_size) {
			for (int i=0; i<mVertexes_length; i++) mPoolVertexesColor[mPool_length][mVertexes[i]] = mVertexesColor_tmp[mVertexes[i]];
			mPoolMaxIter[mPool_length] = max_iter;
			mPool_length++;
		} else {
			int r = rand()%mPool_length;
			for (int i=0; i<mVertexes_length; i++) mPoolVertexesColor[r][mVertexes[i]] = mVertexesColor_tmp[mVertexes[i]];
			mPoolMaxIter[r] = max_iter;
		}
		for (int i=0; i<clash_cur; i++) {
			if (rand()%alpha4 == 0) edges[mClashEdges_tmp[i]].w++;
		}
	} else {
		mPoolMaxIter[last_id] += alpha3*max_iter;
	}

	last_id = rand()%mPool_length;
	for (int i=0; i<mVertexes_length; i++) {
		mVertexesColor[mVertexes[i]] = mPoolVertexesColor[last_id][mVertexes[i]];
	}

	if(clash_best == 0) return true;
	return false;
}

void mGenerate() {
	for (int i=0; i<pool_size; i++) mPoolMaxIter[i] = max_iter;
	for(int i = 0; i < mVertexes_length; i++) {
		int vertex_id = mVertexes[i];
		int tmp_c = vertex_color[vertex_id][rand() % vertex_color_length[vertex_id]];
		mVertexesColor[vertex_id] = tmp_c;
	}
	for (int i=1; i<=vertex_size; i++) {
		// mVertexesColor_tmp[i] = -1;
		mVertexesColor_tmp[i] = mVertexesColor[i];
	}
	mIter = 0;
	last_id = 0;
	mPool_length = 0;
	clash_best = INT_MAX;
	no_improve_search = 0;
	if(mLocalSearch()) return; 
	return;
}


void print_info() {
	printf("======================================================================\n");
	printf("current min clash length: %d\n", clash_cur);
	return;
}

int main(int argc, char* argv[]) {
	clash_best = INT_MAX;
	// filename = argv[1];
	// seed = atoi(argv[2]);
	// time_limit = atof(argv[3]);
	filename = "inst9x9_0_0.txt";
	seed = 1;
	time_limit = 60;
	srand(seed);
	mRead(filename);
	mReduceVertexes();

	mStartTime();
	mGenerate();
	return 0;
	if(check_finish()) return 0;
	for(int iter = 0; total_time < time_limit; iter++) {
		// if (mLocalSearch()) break;
		mLocalSearch();
		print_info();
		if(check_finish()) break;
		mCurrentTime();
	}
	return 0;
}

// g++ sudoku_ls.cpp -std=c++11 -O3 -o sudoku_ls
// ./sudoku_ls inst9x9_0_0.txt 1 60
#include <bits/stdc++.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <libgen.h>
using namespace std;

struct Edge {
	int x;
	int y;
	int w;
};

// temp
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
const int max_iter = 100000;
const int max_no_improve_iter = 10000;
const int max_bms_length = 1;
const double alpha1 = 100;
const double alpha2 = 1.0;
const double alpha3 = 4.0;
const int alpha4 = 5;
const int tabuStep = 20;
const int pool_size = 100;

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
int last_id;

int order;
int prop;
int var_size;
int exp_size;
int color_size;
int edge_size;

Edge* edges;
int** edge_map;
int* mVars;
int mVars_length;
int* mExps;
int mExps_length;
int* mEdges;
int mEdges_length;
int** mNeighbours;
int* mNeighbours_length;

int** var_exp;
int* var_exp_length;
int** exp_var;
int* exp_var_length;
int** exp_coef;
int* exp_coef_length;

int** var_color;
int* var_color_length;
int** var_color_pos;

int* mVarsColor;
int* mVarsColor_tmp;
int* mExpsColor;

int clash_best;
int clash_cur;
int** mClash;
int** mTabu;
bool* mTabuV;
long long* mscore;
long long* nscore;
int mode;
int* mClashEdges;
int* mClashEdges_tmp;
int* mClashEdges_pos;
int mClashEdges_length;
int mClashEdges_length_tmp;
int* mExcelVars;
int* mExcelVars_pos;
int mExcelVars_length;
int* mClashVars;
int* mClashVars_pos;
int mClashVars_length;

int** mPoolVarsColor;
int* mPoolMaxIter;
int mPool_length;

void set_Variable_size() {
	tmp_st = (int *) malloc(sizeof(int) * (exp_size * color_size));
	tmp_tabu_st = (int *) malloc(sizeof(int) * (exp_size * color_size));
	tmp_sub_st = (int *) malloc(sizeof(int) * (exp_size * color_size));
	tmp_sub_tabu_st = (int *) malloc(sizeof(int) * (exp_size * color_size));
	tmp_bms_st = (int *) malloc(sizeof(int) * (max_bms_length + 1));
	tmp_sub_bms_st = (int *) malloc(sizeof(int) * (max_bms_length + 1));
	tmp_vertex = (int **) malloc(sizeof(int *) * (color_size + 1));
	for (int i=1; i<=color_size; i++) tmp_vertex[i] = (int *) malloc(sizeof(int) * (color_size + 1));
	tmp_flag = (bool *) malloc(sizeof(bool) * (exp_size + 1));
    
	edges = (Edge *) malloc(sizeof(Edge) * (edge_size + 1));
	edge_map = (int **) malloc(sizeof(int *) * (exp_size + 1));
	for(int i = 1; i <= exp_size; i++) edge_map[i] = (int *) malloc(sizeof(int) * (exp_size + 1));
	mVars = (int *) malloc(sizeof(int) * (var_size + 1));
	mExps = (int *) malloc(sizeof(int) * (exp_size + 1));
	mEdges = (int *) malloc(sizeof(int) * (edge_size + 1));
	mNeighbours = (int **) malloc(sizeof(int *) * (exp_size + 1));
	for(int i = 1; i <= exp_size; i++) mNeighbours[i] = (int *) malloc(sizeof(int) * (var_size + 1));
	mNeighbours_length = (int *) malloc(sizeof(int) * (exp_size + 1));

	var_exp = (int **) malloc(sizeof(int *) * (var_size + 1));
	for(int i = 1; i <= var_size; i++) var_exp[i] = (int *) malloc(sizeof(int) * (exp_size + 1));
	var_exp_length = (int *) malloc(sizeof(int) * (var_size + 1));

	exp_var = (int **) malloc(sizeof(int *) * (exp_size + 1));
	for(int i = 1; i <= exp_size; i++) exp_var[i] = (int *) malloc(sizeof(int) * (var_size + 1));
	exp_var_length = (int *) malloc(sizeof(int) * (exp_size + 1));

	exp_coef = (int **) malloc(sizeof(int *) * (exp_size + 1));
	for(int i = 1; i <= exp_size; i++) exp_coef[i] = (int *) malloc(sizeof(int) * (var_size + 2));
	exp_coef_length = (int *) malloc(sizeof(int) * (exp_size + 1));

	var_color = (int **) malloc(sizeof(int *) * (exp_size + 1));
	for(int i = 1; i <= exp_size; i++) var_color[i] = (int *) malloc(sizeof(int) * (color_size + 1));
	var_color_pos = (int **) malloc(sizeof(int *) * (exp_size + 1));
	for(int i = 1; i <= exp_size; i++) var_color_pos[i] = (int *) malloc(sizeof(int) * (color_size + 1));
	var_color_length = (int *) malloc(sizeof(int) * (exp_size + 1));
	// color_var = (int **) malloc(sizeof(int *) * (color_size + 1));
	// for(int i = 1; i <= color_size; i++) color_var[i] = (int *) malloc(sizeof(int) * (exp_size + 1));
	// color_var_pos = (int **) malloc(sizeof(int *) * (color_size + 1));
	// for(int i = 1; i <= color_size; i++) color_var_pos[i] = (int *) malloc(sizeof(int) * (exp_size + 1));
	// color_var_length = (int *) malloc(sizeof(int) * (color_size + 1));
	// color_var_recent = (int *) malloc(sizeof(int) * (color_size + 1));

	mVarsColor = (int *) malloc(sizeof(int) * (var_size + 1));
	mVarsColor_tmp = (int *) malloc(sizeof(int) * (var_size + 1));
	mExpsColor = (int *) malloc(sizeof(int) * (exp_size + 1));
	mClash = (int **) malloc(sizeof(int *) * (var_size + 1));
	for(int i = 1; i <= var_size; i++) mClash[i] = (int *) malloc(sizeof(int) * (color_size + 1));
	mTabu = (int **) malloc(sizeof(int *) * (var_size + 1));
	for(int i = 1; i <= var_size; i++) mTabu[i] = (int *) malloc(sizeof(long long) * (color_size + 1));
	mTabuV = (bool *) malloc(sizeof(bool) * (var_size + 1));
	mscore = (long long *) malloc(sizeof(long long) * (var_size + 1));
	nscore = (long long *) malloc(sizeof(long long) * (var_size + 1));
    mClashEdges = (int *) malloc(sizeof(int) * (edge_size + 1));
	mClashEdges_tmp = (int *) malloc(sizeof(int) * (edge_size + 1));
    mClashEdges_pos = (int *) malloc(sizeof(int) * (edge_size + 1));
	mExcelVars = (int *) malloc(sizeof(int) * (var_size + 1));
	mExcelVars_pos = (int *) malloc(sizeof(int) * (var_size + 1));
	mClashVars = (int *) malloc(sizeof(int) * (var_size + 1));
	mClashVars_pos = (int *) malloc(sizeof(int) * (var_size + 1));

	mPoolVarsColor = (int **) malloc(sizeof(int *) * (pool_size));
	for(int i = 0; i < pool_size; i++) mPoolVarsColor[i] = (int *) malloc(sizeof(int) * (var_size + 1));
	mPoolMaxIter = (int *) malloc(sizeof(int) * (pool_size + 1));

	return;
}

int to_index(int index) {
	return index + var_size;
}

int to_index(int x, int y, bool type) {
	if (type) return (x-1)*order + y;
	else return order*order + (x-1)*order + y; 
}


int to_sum_index(int index) {
	return index + color_size;
}

int to_dif_index(int index) {
	return index + 2*color_size;
}

int to_pre_color(int index, int color, bool type) {
	if(type) return index + color;
	return color_size - color - index + 1;
}

int to_sum_color(int index, int color) {
	return color - index;
}

int to_dif_color(int index, int color) {
	return color_size - color - index + 1;
}

int get_type(int index) {
	return (index-1) / color_size;
}

void init() {
	for(int i = 1; i <= exp_size; i++) {
		mNeighbours_length[i] = 0;
		exp_var_length[i] = 0;
		exp_coef_length[i] = 0;
		mExpsColor[i] = -1;
		for(int j = 1; j <= exp_size; j++) edge_map[i][j] = -1;
	}

	for(int i = 1; i <= var_size; i++) {
		var_color_length[i] = 0;
		var_exp_length[i] = 0;
		mVarsColor[i] = -1;
	}

	return;
}

void swap_var_color(int id, int c_a, int p_b) {
	int c_b = var_color[id][p_b];
	int p_a = var_color_pos[id][c_a];
	var_color[id][p_a] = c_b;
	var_color[id][p_b] = c_a;
	var_color_pos[id][c_a] = p_b;
	var_color_pos[id][c_b] = p_a;
	return;
}

// void swap_color_var(int id, int v_a, int p_b) {
// 	int v_b = color_var[id][p_b];
// 	int p_a = color_var_pos[id][v_a];
// 	color_var[id][p_a] = v_b;
// 	color_var[id][p_b] = v_a;
// 	color_var_pos[id][v_a] = p_b;
// 	color_var_pos[id][v_b] = p_a;
// 	return;
// }

int get_score(int id, int to) {
	return mClash[id][to] - mClash[id][mVarsColor[id]];
}

int get_Col(int id) {
	return (id - 1) / color_size + 1;
}

int get_Row(int id) {
	return (id - 1) % color_size + 1;
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
	// FILE *fp = freopen(filename.c_str(), "r", stdin);  
	order = stoi(filename);
	var_size = order*order*2;
	exp_size = order*order*3;
	color_size = order;
	edge_size = order*order*(order-1)*(order+5)/2;
	set_Variable_size();
	init();

	// Initialize variable domains
	// Initialize relationships between variables and expressions
	for (int i=1; i<=var_size; i++) {
		for (int j=1; j<=color_size; j++) {
			var_color[i][var_color_length[i]] = j; 
			var_color_pos[i][j] = var_color_length[i]++;
		}
		var_exp[i][var_exp_length[i]++] = i;
		exp_var[i][exp_var_length[i]++] = i;
		exp_coef[i][exp_coef_length[i]++] = 1;
		int tmp_exp;
		if (i<=order*order) {
			tmp_exp = var_size + i;
			var_exp[i][var_exp_length[i]++] = tmp_exp;
			exp_var[tmp_exp][exp_var_length[tmp_exp]++] = i;
			exp_coef[tmp_exp][exp_coef_length[tmp_exp]++] = order;
		} else {
			tmp_exp = order*order + i;
			var_exp[i][var_exp_length[i]++] = tmp_exp;
			exp_var[tmp_exp][exp_var_length[tmp_exp]++] = i;
			exp_coef[tmp_exp][exp_coef_length[tmp_exp]++] = 1;
		}
	}

	int index = 0;
	// Latin square constraints
	for (int i=1; i<=order; i++) {
		for (int x=1; x<order; x++) {
			int exp1 = to_index(i, x, true);
			int exp3 = to_index(x, i, true);
			int exp5 = to_index(i, x, false);
			int exp7 = to_index(x, i, false);
			for (int y=x+1; y<=order; y++) {
				int exp2 = to_index(i, y, true);
				int exp4 = to_index(y, i, true);
				int exp6 = to_index(i, y, false);
				int exp8 = to_index(y, i, false);
				mNeighbours[exp1][mNeighbours_length[exp1]++] = exp2;
				mNeighbours[exp2][mNeighbours_length[exp2]++] = exp1;
				edges[++index].x = exp1;
				edges[index].y = exp2;
				edges[index].w = 1;
				edge_map[exp1][exp2] = index;
				edge_map[exp2][exp1] = index;
				mNeighbours[exp3][mNeighbours_length[exp3]++] = exp4;
				mNeighbours[exp4][mNeighbours_length[exp4]++] = exp3;
				edges[++index].x = exp3;
				edges[index].y = exp4;
				edges[index].w = 1;
				edge_map[exp3][exp4] = index;
				edge_map[exp4][exp3] = index;
				mNeighbours[exp5][mNeighbours_length[exp5]++] = exp6;
				mNeighbours[exp6][mNeighbours_length[exp6]++] = exp5;
				edges[++index].x = exp5;
				edges[index].y = exp6;
				edges[index].w = 1;
				edge_map[exp5][exp6] = index;
				edge_map[exp6][exp5] = index;
				mNeighbours[exp7][mNeighbours_length[exp7]++] = exp8;
				mNeighbours[exp8][mNeighbours_length[exp8]++] = exp7;
				edges[++index].x = exp7;
				edges[index].y = exp8;
				edges[index].w = 1;
				edge_map[exp7][exp8] = index;
				edge_map[exp8][exp7] = index;
			}
		}
	}
	// Orthogonal constraints
	for (int i=1; i<order*order; i++) {
		int exp1 = to_index(i);
		for (int j=i+1; j<=order*order; j++) {
			int exp2 = to_index(j);
			mNeighbours[exp1][mNeighbours_length[exp1]++] = exp2;
			mNeighbours[exp2][mNeighbours_length[exp2]++] = exp1;
			edges[++index].x = exp1;
			edges[index].y = exp2;
			edges[index].w = 1;
			edge_map[exp1][exp2] = index;
			edge_map[exp2][exp1] = index;
		}
	}
	return;
}

void print_current() {
	printf("======================================================================\n");
	cout << " \t|\t";
	for(int i = 1; i <= color_size; i++) {
		if (i != 1) cout << "\t";
		cout << i;
		if (i%order == 0) cout << "\t ";
	}
	cout << endl;
	cout << "=\t#\t";
	for(int i = 1; i <= color_size; i++) {
		if (i != 1) cout << "\t";
		cout << "=";
		if (i%order == 0) cout << "\t*";
	}
	cout << endl;
	for(int i = 1; i <= color_size; i++) {
		cout << i << "\t|\t";
		for(int j = 1; j <= color_size; j++) {
			int id = color_size * (i - 1) + j;
			if (j != 1) cout << "\t";
			cout << mVarsColor_tmp[id];
			if (j%order == 0) cout << "\t|";
		}
		cout << endl;
		if (i%order == 0) {
			cout << "\t#\t";
			for(int j = 1; j <= color_size; j++) {
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
	printf("current min clash length: %d\n", clash_cur);
	return;
}

void check_answer() {
	bool flag = true;
	for (int i=1; i<=exp_size; i++) {
		int val = 0;
		for (int j=0; j<exp_var_length[i]; j++) val += mVarsColor[exp_var[i][j]] * exp_coef[i][j];
		mExpsColor[i] = val;
	}
	// for (int i=0; i<mExps_length; i++) {
	// 	cout << "exp_id: " << mExps[i] << ", color: " << mExpsColor[mExps[i]] << endl;
	// }
	for (int i = 1; i <= edge_size; i++) {
		if(mExpsColor[edges[i].x] == mExpsColor[edges[i].y]) {
			cout << "answer wrong same color " << edges[i].x << " " << edges[i].y << " " << mExpsColor[edges[i].x] << " " << mExpsColor[edges[i].y] << endl;
			flag = false;
		}
	}
	for (int i=1; i<=var_size; i++) {
		if(mVarsColor_tmp[i] == -1) {
			cout << "answer wrong no color: " << i << " " << mVarsColor_tmp[i] << endl;
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
	mVars_length = 0;
	mExps_length = 0;
	mEdges_length = 0;
	for (int i=1; i<=var_size; i++) mVars[mVars_length++] = i;
	for (int i=1; i<=exp_size; i++) mExps[mExps_length++] = i;
	for (int i=1; i<=edge_size; i++) mEdges[mEdges_length++] = i;
	return;
}

void update_info(int id, int pos) {
	int cur_pos = mVarsColor[id];
	mVarsColor[id] = pos;

	// cout << "id: " << id << ", pos: " << pos << ", cur_pos: " << cur_pos << endl;
	for (int i=0; i<var_exp_length[id]; i++) {
		int exp = var_exp[id][i];
		if (exp_var_length[exp] == 1) {
			mExpsColor[exp] = pos;
			for (int j=0; j<mNeighbours_length[exp]; j++) {
				int tmp_e = mNeighbours[exp][j];
				int tmp_v = exp_var[tmp_e][0];
				int edge_id = edge_map[exp][tmp_e];
				mClash[tmp_v][cur_pos] -= edges[edge_id].w;
				mClash[tmp_v][pos] += edges[edge_id].w;
				if (mVarsColor[tmp_v] == cur_pos) {
					if (mClash[tmp_v][cur_pos] == 0) {
						mClashVars[mClashVars_pos[tmp_v]] = mClashVars[--mClashVars_length];
						mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[tmp_v];
					}
					mClashEdges[mClashEdges_pos[edge_id]] = mClashEdges[--mClashEdges_length];
					mClashEdges_pos[mClashEdges[mClashEdges_length]] = mClashEdges_pos[edge_id];
				} else if (mVarsColor[tmp_v] == pos) {
					if (mClashVars[mClashVars_pos[tmp_v]] != tmp_v) {
						mClashVars[mClashVars_length] = tmp_v;
						mClashVars_pos[tmp_v] = mClashVars_length++;
					}
					mClashEdges[mClashEdges_length] = edge_id;
					mClashEdges_pos[edge_id] = mClashEdges_length++;
				}
			}
		} else {
			int index1;
			int index2;
			for (int j=0; j<exp_var_length[exp]; j++) {
				if (exp_var[exp][j] == id) index1 = j;
				else index2 = j;
			}
			int cur_color = mExpsColor[exp];
			mExpsColor[exp] = mExpsColor[exp] - cur_pos*exp_coef[exp][index1] + pos*exp_coef[exp][index1];
			for (int j=0; j<mNeighbours_length[exp]; j++) {
				int tmp_v = mNeighbours[exp][j];
				int edge_id = edge_map[exp][tmp_v];
				int x1 = exp_var[exp][index1];
				int x2 = exp_var[exp][index2];
				int y1 = exp_var[tmp_v][index1];
				int y2 = exp_var[tmp_v][index2];

				if (mVarsColor[x2] == mVarsColor[y2]) {
					mClash[y1][cur_pos] -= edges[edge_id].w;
					mClash[y1][pos] += edges[edge_id].w;
					if (mVarsColor[y1] == cur_pos && mClash[y1][cur_pos] == 0) {
						mClashVars[mClashVars_pos[y1]] = mClashVars[--mClashVars_length];
						mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[y1];
					} else if (mVarsColor[y1] == pos && mClashVars[mClashVars_pos[y1]] != y1) {
						mClashVars[mClashVars_length] = y1;
						mClashVars_pos[y1] = mClashVars_length++;
					}
				}
				if (cur_pos == mVarsColor[y1]) {
					mClash[x2][mVarsColor[y2]] -= edges[edge_id].w;
					mClash[y2][mVarsColor[x2]] -= edges[edge_id].w;
					if (mVarsColor[x2] == mVarsColor[y2]) {
						if (mClash[x2][mVarsColor[y2]] == 0) {
							mClashVars[mClashVars_pos[x2]] = mClashVars[--mClashVars_length];
							mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[x2];
						}
						if (mClash[y2][mVarsColor[x2]] == 0) {
							mClashVars[mClashVars_pos[y2]] = mClashVars[--mClashVars_length];
							mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[y2];
						}
					}

				} else if (pos == mVarsColor[y1]) {
					mClash[x2][mVarsColor[y2]] += edges[edge_id].w;
					mClash[y2][mVarsColor[x2]] += edges[edge_id].w;
					if (mVarsColor[x2] == mVarsColor[y2]) {
						if (mClashVars[mClashVars_pos[x2]] != x2) {
							mClashVars[mClashVars_length] = x2;
							mClashVars_pos[x2] = mClashVars_length++;
						}
						if (mClashVars[mClashVars_pos[y2]] != y2) {
							mClashVars[mClashVars_length] = y2;
							mClashVars_pos[y2] = mClashVars_length++;
						}
					}
				}
				if (mExpsColor[tmp_v] == cur_color) {
					mClashEdges[mClashEdges_pos[edge_id]] = mClashEdges[--mClashEdges_length];
					mClashEdges_pos[mClashEdges[mClashEdges_length]] = mClashEdges_pos[edge_id];
				} else if (mExpsColor[tmp_v] == mExpsColor[exp]) {
					mClashEdges[mClashEdges_length] = edge_id;
					mClashEdges_pos[edge_id] = mClashEdges_length++;
				}
			}


			// int var;
			// for (int j=0; j<exp_var_length[exp]; j++) {
			// 	if (exp_var[exp][j] == id) continue;
			// 	var = exp_var[exp][j];
			// }
			// for (int i=0; i<mNeighbours_length[i]; i++) {
			// 	int tmp_e = mNeighbours[exp][i];
			// 	int edge_id = edge_map[exp][tmp_e];
				
			// 	int c1 = mExpsColor[tmp_e]-cur_color;
			// 	int c2 = mExpsColor[tmp_e]-mExpsColor[exp];
			// 	mClash[var][c1] -= edges[edge_id].w;
			// 	mClash[var][c2] += edges[edge_id].w;

			// 	if (mVarsColor[var] == c1 && mClash[var][c1] == 0) {
			// 		mClashVars[mClashVars_pos[var]] = mClashVars[--mClashVars_length];
			// 		mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[var];
			// 	} else if (mVarsColor[var] == c2 && mClashVars[mClashVars_pos[var]] != var) {
			// 		mClashVars[mClashVars_length] = var;
			// 		mClashVars_pos[var] = mClashVars_length++;
			// 	}

			// 	for (int j=0; j<exp_var_length[tmp_e]; j++) {
			// 		int tmp_v = exp_var[tmp_e][j];
			// 		int tmp_c1 = cur_color - mExpsColor[tmp_e] + mVarsColor[tmp_v];
			// 		int tmp_c2 = mExpsColor[exp] - mExpsColor[tmp_e] + mVarsColor[tmp_v];
			// 		mClash[tmp_v][tmp_c1] -= edges[edge_id].w;
			// 		mClash[tmp_v][tmp_c2] += edges[edge_id].w;
			// 		if (mVarsColor[tmp_v] == tmp_c1 && mClash[tmp_v][tmp_c1] == 0) {
			// 			mClashVars[mClashVars_pos[tmp_v]] = mClashVars[--mClashVars_length];
			// 			mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[tmp_v];
			// 		} else if (mVarsColor[tmp_v] == tmp_c2 && mClashVars[mClashVars_pos[tmp_v]] != tmp_v) {
			// 			mClashVars[mClashVars_length] = tmp_v;
			// 			mClashVars_pos[tmp_v] = mClashVars_length++;
			// 		}
			// 	}

			// 	if (mExpsColor[tmp_e] == cur_color) {
			// 		mClashEdges[mClashEdges_pos[edge_id]] = mClashEdges[--mClashEdges_length];
			// 		mClashEdges_pos[mClashEdges[mClashEdges_length]] = mClashEdges_pos[edge_id];
			// 	} else if (mExpsColor[tmp_e] == mExpsColor[exp]) {
			// 		mClashEdges[mClashEdges_length] = edge_id;
			// 		mClashEdges_pos[edge_id] = mClashEdges_length++;
			// 	}
			// }
		}
	}
	
	if (mClash[id][pos] == 0 && mClashVars_pos[id] >= 0) {
		mClashVars[mClashVars_pos[id]] = mClashVars[--mClashVars_length];
		mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[id];
		mClashVars_pos[id] = -1;
	}
	return;
}

void build() {
	for(int i=0; i<mVars_length; i++) {
		mscore[mVars[i]] = INT_MAX;
		mTabuV[mVars[i]] = true;
		for(int j = 1; j <= color_size; j++) {
			mClash[mVars[i]][j] = 0;
			mTabu[mVars[i]][j] = 0;
		}
	}
	mode = 0;
	mClashEdges_length = 0;
	for(int i = 0; i < mEdges_length; i++) {
        int e = mEdges[i], x, y;
		x = edges[e].x;
		y = edges[e].y;
		if (exp_var_length[edges[e].x] == 1) {
			mClash[x][mExpsColor[y]] += edges[e].w;
			mClash[y][mExpsColor[x]] += edges[e].w;
		} else {
			int x1 = exp_var[x][0];
			int x2 = exp_var[x][1];
			int y1 = exp_var[y][0];
			int y2 = exp_var[y][1];
			if (mVarsColor[x1] == mVarsColor[y1]) {
				mClash[x2][mVarsColor[y2]] += edges[e].w;
				mClash[y2][mVarsColor[x2]] += edges[e].w;
			}
			if (mVarsColor[x2] == mVarsColor[y2]) {
				mClash[x1][mVarsColor[y1]] += edges[e].w;
				mClash[y1][mVarsColor[x1]] += edges[e].w;
			}
			// for (int j=0; j<exp_var_length[x]; j++) {
			// 	int tmp_v = exp_var[x][j];
			// 	int tmp_c = mExpsColor[y]-mExpsColor[x]+mVarsColor[tmp_v];
			// 	if (tmp_c > 0 && tmp_c <= color_size) mClash[tmp_v][tmp_c] += edges[e].w;
			// }
			// for (int j=0; j<exp_var_length[y]; j++) {
			// 	int tmp_v = exp_var[y][j];
			// 	int tmp_c = mExpsColor[x]-mExpsColor[y]+mVarsColor[tmp_v];
			// 	if (tmp_c > 0 && tmp_c <= color_size) mClash[tmp_v][tmp_c] += edges[e].w;
			// }
		}
		if (mExpsColor[edges[e].x] == mExpsColor[edges[e].y]) {
            mClashEdges[mClashEdges_length] = e;
            mClashEdges_pos[e] = mClashEdges_length++;
		}
	}

	mExcelVars_length = 0;
	mClashVars_length = 0;

	for (int i=0; i<mVars_length; i++) {
		int var_id = mVars[i];
		for (int j=0; j<var_color_length[var_id]; j++) {
			if (mVarsColor[var_id] == var_color[var_id][j]) continue;
			int tmp_cost = mClash[var_id][var_color[var_id][j]];
			if (tmp_cost < mscore[var_id]) mscore[var_id] = tmp_cost;
		}
		if (mClash[var_id][mVarsColor[var_id]] >= mscore[var_id]) {
			mExcelVars[mExcelVars_length] = var_id;
			mExcelVars_pos[var_id] = mExcelVars_length++;
		}
	}

	if (mClashEdges_length != 0 && mExcelVars_length == 0) {
		mode = 100;
		for (int i = 0; i < mVars_length; i++) {
			int var_id = mVars[i];
			if (mClash[var_id][mVarsColor[var_id]] > 0) {
				mClashVars[mClashVars_length] = var_id;
				mClashVars_pos[var_id] = mClashVars_length++;
			}
		}
	}
	return;
}

int mTabuSearch() {
	int tSminClash;
	build();
	tSminClash = mClashEdges_length;
	if (tSminClash == 0) return tSminClash;
	for(int i = 0; i < mVars_length; i++) {
        mVarsColor_tmp[mVars[i]] = mVarsColor[mVars[i]];
    }
	int r;
	int no_improve_iter = 0;
	int min_clash;
	int min_tabu_clash;
	int min_sub_clash;
	int min_sub_tabu_clash;
	int cnt;
	int tabu_cnt;
	int sub_cnt;
	int sub_tabu_cnt;
	long long max_cs;
	int max_bs;
	int v_cnt;
	int cs_cnt;
	int bs_cnt;
	int move_id;
	int move_to;
	int move_from;
	int flag = 0;
	for (int iters = 1; iters <= mPoolMaxIter[last_id]; iters++) {
		min_clash = INT_MAX;
		min_tabu_clash = INT_MAX;
		min_sub_clash = INT_MAX;
		min_sub_tabu_clash = INT_MAX;
		max_cs = INT_MIN;
		max_bs = INT_MIN;
		cnt = 0;
		tabu_cnt = 0;
		sub_cnt = 0;
		sub_tabu_cnt = 0;
		if (true) {
			for (int i = 0; i < mClashVars_length; i++) {
				int var_id = mClashVars[i];
				for(int j = 0; j < var_color_length[var_id]; j++) {
					int tmp_c = var_color[var_id][j];
					if(tmp_c == mVarsColor[var_id]) continue;
					int tmp_s = get_score(var_id, tmp_c);
					if(check_tabu(var_id, tmp_c, iters)) {
						if (tmp_s < min_clash) {
							cnt = 0;
							min_clash = tmp_s;
							tmp_st[cnt] = var_id;
							tmp_sub_st[cnt] = tmp_c;
							cnt++;
						} else if (tmp_s == min_clash) {
							tmp_st[cnt] = var_id;
							tmp_sub_st[cnt] = tmp_c;
							cnt++;						
						}
					} else {
						if (tmp_s < min_tabu_clash) {
							tabu_cnt = 0;
							min_tabu_clash = tmp_s;
							tmp_tabu_st[tabu_cnt] = var_id;
							tmp_sub_tabu_st[tabu_cnt] = tmp_c;
							tabu_cnt++;
						} else if (tmp_s == min_tabu_clash) {
							tmp_tabu_st[tabu_cnt] = var_id;
							tmp_sub_tabu_st[tabu_cnt] = tmp_c;
							tabu_cnt++;
						}
					}
				}
			}
			if (cnt == 0 && tabu_cnt == 0) {
				return tSminClash;
			} else if ((tabu_cnt > 0 && min_tabu_clash < min_clash && (mClashEdges_length + min_tabu_clash < tSminClash)) || (cnt == 0 && tabu_cnt > 0)) {
				int v_cnt = min(max_bms_length, tabu_cnt);
				for(int i = 0; i < v_cnt; i++) {
					r = rand() % tabu_cnt;
					long long tmp_cs = 1;
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
					long long tmp_cs = 1;
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
			mTabu[move_id][mVarsColor[move_id]] = iters + rand() % tabuStep + alpha2 * mClashVars_length;
			// cout << "=============================================" << endl;
			// cout << "loop: " << iters << ", min_clash: " << min_clash << ", min_tabu_clash: " << min_tabu_clash << endl;
			// cout << "choose vertex " << move_id << " change to " << move_to << endl;
			update_info(move_id, move_to);
			// cout << "mClashEdges_length: " << mClashEdges_length << ", mClashVars_length: " << mClashVars_length << endl;
		}

		if (mClashEdges_length <= tSminClash) {
			tSminClash = mClashEdges_length;
			for (int i=0; i<mVars_length; i++) mVarsColor_tmp[mVars[i]] = mVarsColor[mVars[i]];
			for (int i=0; i<mClashEdges_length; i++) mClashEdges_tmp[i] = mClashEdges[i];
			if(tSminClash == 0) return tSminClash;
		}
	}
	// cout << "=============================================" << endl;
	// for (int i=0; i<tSminClash; i++) {
	// 	cout << "mClashEdges: " << edges[mClashEdges_tmp[i]].x << "--" << edges[mClashEdges_tmp[i]].y << endl;
	// }

	return tSminClash;
}

bool check_diff() {
	bool flag = false;
	for (int j=0; j<mVars_length; j++) {
		if (mPoolVarsColor[last_id][mVars[j]] != mVarsColor_tmp[mVars[j]]) {
			flag = true;
			break;
		} 
	}
	if (!flag) return false;
	for (int i=0; i<mPool_length; i++) {
		flag = false;
		for (int j=0; j<mVars_length; j++) {
			if (mPoolVarsColor[i][mVars[j]] != mVarsColor_tmp[mVars[j]]) {
				flag = true;
				break;
			} 
		}
		if (!flag) return false;
	}
	return true;
}

bool mLocalSearch() {
	clash_cur = mTabuSearch();
	cout << clash_cur << endl;
	if(clash_cur < clash_best) {
		mPool_length = 0;
		clash_best = clash_cur;
		for (int i=0; i<mVars_length; i++) {
			mPoolVarsColor[mPool_length][mVars[i]] = mVarsColor_tmp[mVars[i]];
		}
		for (int i=0; i<mEdges_length; i++) edges[mEdges[i]].w = 1;
		mPoolMaxIter[mPool_length] = max_iter;
		mPool_length++;
	} else if (clash_cur == clash_best && check_diff()) {
		if (mPool_length < pool_size) {
			for (int i=0; i<mVars_length; i++) {
				mPoolVarsColor[mPool_length][mVars[i]] = mVarsColor_tmp[mVars[i]];
			}
			mPoolMaxIter[mPool_length] = max_iter;
			mPool_length++;
		} else {
			int r = rand()%mPool_length;
			for (int i=0; i<mVars_length; i++) {
				mPoolVarsColor[r][mVars[i]] = mVarsColor_tmp[mVars[i]];
			}
			mPoolMaxIter[r] = max_iter;
		}
		
		// for (int i=0; i<clash_cur; i++) {
		// 	if (rand()%alpha4 == 0) edges[mClashEdges_tmp[i]].w++;
		// }
	} else {
		mPoolMaxIter[last_id] += alpha3*max_iter;
	}

	last_id = rand()%mPool_length;
	for (int i=0; i<mVars_length; i++) {
		int v = mVars[i];
		int tmp_c = mPoolVarsColor[last_id][v];
		mVarsColor[v] = tmp_c;
	}
	for (int i=0; i<mExps_length; i++) {
		int exp = mExps[i];
		int val = 0;
		for (int j=0; j<exp_var_length[exp]; j++) val += mVarsColor[exp_var[exp][j]] * exp_coef[exp][j];
		mExpsColor[exp] = val;
	}
	if(clash_best == 0) return true;
	return false;
}

void mGenerate() {
	for (int i=0; i<pool_size; i++) mPoolMaxIter[i] = max_iter;
	for (int i=0; i<mVars_length; i++) {
		int v = mVars[i];
		int tmp_c = var_color[v][rand() % var_color_length[v]];
		mVarsColor[v] = tmp_c;
	}

	for (int i=0; i<mExps_length; i++) {
		int exp = mExps[i];
		int val = 0;
		for (int j=0; j<exp_var_length[exp]; j++) val += mVarsColor[exp_var[exp][j]] * exp_coef[exp][j];
		mExpsColor[exp] = val;
	}

	// for (int i=0; i<mExps_length; i++) {
	// 	cout << "exp_id: " << mExps[i] << ", color: " << mExpsColor[mExps[i]] << endl;
	// }

	for (int i=1; i<=var_size; i++) {
		// mVarsColor_tmp[i] = -1;
		mVarsColor_tmp[i] = mVarsColor[i];
	}
	last_id = 0;
	mPool_length = 0;
	clash_best = INT_MAX;
	no_improve_search = 0;
	if(mLocalSearch()) return; 
	return;
}

int main(int argc, char* argv[]) {
	clash_best = INT_MAX;
	filename = argv[1];
	seed = atoi(argv[2]);
	time_limit = atof(argv[3]);
	// filename = "5";
	// seed = 1;
	// time_limit = 60;
	srand(seed);
	mRead(filename);
	mReduceVertexes();

	mStartTime();
	mGenerate();
	// return 0;
	if(check_finish()) return 0;
	for(int iter = 0; total_time < time_limit; iter++) {
		mLocalSearch();
		if(check_finish()) break;
		mCurrentTime();
	}
	return 0;
}
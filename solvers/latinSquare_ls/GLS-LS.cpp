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
const double alpha1 = 0.9;
const double alpha2 = 0.6;
const double alpha3 = 4.0;
const int alpha4 = 5;
const int tabuStep = 10;
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
// int** cst_exp;
// int* cst_exp_length;

int** var_color;
int* var_color_length;
int** var_color_pos;
// int** color_var;
// int* color_var_length;
// int** color_var_pos;
// int* color_var_recent;

int* mVarsColor;
int* mVarsColor_tmp;
int* mExpsColor;

int clash_best;
int clash_cur;
int** mClash;
int** mTabu;
bool* mTabuV;
int* mClashEdges;
int* mClashEdges_tmp;
int* mClashEdges_pos;
int mClashEdges_length;
int mClashEdges_length_tmp;
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
	for(int i = 1; i <= exp_size; i++) mNeighbours[i] = (int *) malloc(sizeof(int) * (3 * color_size));
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
	mClash = (int **) malloc(sizeof(int *) * (exp_size + 1));
	for(int i = 1; i <= exp_size; i++) mClash[i] = (int *) malloc(sizeof(int) * (color_size + 1));
	mTabu = (int **) malloc(sizeof(int *) * (exp_size + 1));
	for(int i = 1; i <= exp_size; i++) mTabu[i] = (int *) malloc(sizeof(long long) * (color_size + 1));
	mTabuV = (bool *) malloc(sizeof(bool) * (exp_size + 1));
    mClashEdges = (int *) malloc(sizeof(int) * (edge_size + 1));
	mClashEdges_tmp = (int *) malloc(sizeof(int) * (edge_size + 1));
    mClashEdges_pos = (int *) malloc(sizeof(int) * (edge_size + 1));
	mClashVars = (int *) malloc(sizeof(int) * (exp_size + 1));
	mClashVars_pos = (int *) malloc(sizeof(int) * (exp_size + 1));

	mPoolVarsColor = (int **) malloc(sizeof(int *) * (pool_size));
	for(int i = 0; i < pool_size; i++) mPoolVarsColor[i] = (int *) malloc(sizeof(int) * (exp_size + 1));
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
	exp_size = order*order*3; // 前order*order*2个表达式一一对应，后面的表达式是两组拉丁方相应变量的和
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
		if (i<=order*order) tmp_exp = var_size + i;
		else tmp_exp = order*order + i;
		var_exp[i][var_exp_length[i]++] = tmp_exp;
		exp_var[tmp_exp][exp_var_length[tmp_exp]++] = i;
		exp_coef[tmp_exp][exp_coef_length[tmp_exp]++] = 1;
	}

	int index = 0;
	// Latin square constraints
	for (int i=1; i<=order; i++) {
		for (int x=1; x<order; x++) {
			int var1 = to_index(i, x, true);
			int var3 = to_index(x, i, true);
			int var5 = to_index(i, x, false);
			int var7 = to_index(x, i, false);
			for (int y=x+1; y<=order; y++) {
				int var2 = to_index(i, y, true);
				int var4 = to_index(y, i, true);
				int var6 = to_index(i, y, false);
				int var8 = to_index(y, i, false);

				mNeighbours[var1][mNeighbours_length[var1]++] = var2;
				mNeighbours[var2][mNeighbours_length[var2]++] = var1;
				edges[++index].x = var1;
				edges[index].y = var2;
				edges[index].w = 1;
				edge_map[var1][var2] = index;
				edge_map[var2][var1] = index;
				mNeighbours[var3][mNeighbours_length[var3]++] = var4;
				mNeighbours[var4][mNeighbours_length[var4]++] = var3;
				edges[++index].x = var3;
				edges[index].y = var4;
				edges[index].w = 1;
				edge_map[var3][var4] = index;
				edge_map[var4][var3] = index;
				mNeighbours[var5][mNeighbours_length[var5]++] = var6;
				mNeighbours[var6][mNeighbours_length[var6]++] = var5;
				edges[++index].x = var5;
				edges[index].y = var6;
				edges[index].w = 1;
				edge_map[var5][var6] = index;
				edge_map[var6][var5] = index;
				mNeighbours[var7][mNeighbours_length[var7]++] = var8;
				mNeighbours[var8][mNeighbours_length[var8]++] = var7;
				edges[++index].x = var7;
				edges[index].y = var8;
				edges[index].w = 1;
				edge_map[var7][var8] = index;
				edge_map[var8][var7] = index;
			}
		}
	}
	// Orthogonal constraints
	for (int i=1; i<order*order; i++) {
		int var1 = to_index(i);
		for (int j=i+1; j<=order*order; j++) {
			int var2 = to_index(j);
			mNeighbours[var1][mNeighbours_length[var1]++] = var2;
			mNeighbours[var2][mNeighbours_length[var2]++] = var1;
			edges[++index].x = var1;
			edges[index].y = var2;
			edges[index].w = 1;
			edge_map[var1][var2] = index;
			edge_map[var2][var1] = index;
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
	for (int i=1; i<=var_size; i++) {
		int tmp_c = mVarsColor_tmp[i];
		int v = i;
		int v1 = to_sum_index(v);
		int v2 = to_dif_index(v);
		mExpsColor[v] = tmp_c;
		mExpsColor[v1] = to_sum_color(v, tmp_c);
		mExpsColor[v2] = to_dif_color(v, tmp_c);
	}
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
	for (int i=1; i<=exp_size; i++) mExps[mEdges_length++] = i;
	for (int i=1; i<=edge_size; i++) mEdges[mEdges_length++] = i;
	return;
}

void update_info(int id, int pos) {
	int cur_pos = mVarsColor[id];
	int v = id;
	int v1 = to_sum_index(v);
	int v2 = to_dif_index(v);
	int cur_c1 = to_sum_color(id, cur_pos);
	int c1 = to_sum_color(id, pos);
	int cur_c2 = to_dif_color(id, cur_pos);
	int c2 = to_dif_color(id, pos);
	for (int i = 0; i < mNeighbours_length[v]; i++) {
		int tmp_v = mNeighbours[v][i];
		int v_index = to_index(tmp_v);
		int edge_id = edge_map[v][tmp_v];
		mClash[v_index][cur_pos] -= edges[edge_id].w;
		mClash[v_index][pos] += edges[edge_id].w;
		if (mVarsColor[v_index] == cur_pos) {
			if (mClash[v_index][cur_pos] == 0) {
				mClashVars[mClashVars_pos[v_index]] = mClashVars[--mClashVars_length];
				mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[v_index];
				mClashVars_pos[v_index] = -1;
			}
            mClashEdges[mClashEdges_pos[edge_id]] = mClashEdges[--mClashEdges_length];
            mClashEdges_pos[mClashEdges[mClashEdges_length]] = mClashEdges_pos[edge_id];
            mClashEdges_pos[edge_id] = -1;
		} else if (mVarsColor[v_index] == pos) {
			if (mClashVars_pos[v_index] == -1) {
				mClashVars[mClashVars_length] = v_index;
				mClashVars_pos[v_index] = mClashVars_length++;
			}
            mClashEdges[mClashEdges_length] = edge_id;
			mClashEdges_pos[edge_id] = mClashEdges_length++;
		}
	}

	for (int i = 0; i < mNeighbours_length[v1]; i++) {
		int tmp_v = mNeighbours[v1][i];
		int v_index = to_index(tmp_v);
		int edge_id = edge_map[v1][tmp_v];
		int cur_c = to_pre_color(v_index, cur_c1, true);
		int c = to_pre_color(v_index, c1, true);
		if (cur_c > 0 && cur_c <= color_size) mClash[v_index][cur_c] -= edges[edge_id].w;
		if (c > 0 && c <= color_size) mClash[v_index][c] += edges[edge_id].w;
		if (mVarsColor[v_index] == cur_c) {
			if (mClash[v_index][cur_c] == 0) {
				mClashVars[mClashVars_pos[v_index]] = mClashVars[--mClashVars_length];
				mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[v_index];
				mClashVars_pos[v_index] = -1;
			}
            mClashEdges[mClashEdges_pos[edge_id]] = mClashEdges[--mClashEdges_length];
            mClashEdges_pos[mClashEdges[mClashEdges_length]] = mClashEdges_pos[edge_id];
            mClashEdges_pos[edge_id] = -1;
		} else if (mVarsColor[v_index] == c) {
			if (mClashVars_pos[v_index] == -1) {
				mClashVars[mClashVars_length] = v_index;
				mClashVars_pos[v_index] = mClashVars_length++;
			}
            mClashEdges[mClashEdges_length] = edge_id;
			mClashEdges_pos[edge_id] = mClashEdges_length++;
		}
	}

	for (int i = 0; i < mNeighbours_length[v2]; i++) {
		int tmp_v = mNeighbours[v2][i];
		int v_index = to_index(tmp_v);
		int edge_id = edge_map[v2][tmp_v];
		int cur_c = to_pre_color(v_index, cur_c2, false);
		int c = to_pre_color(v_index, c2, false);
		if (cur_c > 0 && cur_c <= color_size) mClash[v_index][cur_c] -= edges[edge_id].w;
		if (c > 0 && c <= color_size) mClash[v_index][c] += edges[edge_id].w;
		if (mVarsColor[v_index] == cur_c) {
			if (mClash[v_index][cur_c] == 0) {
				mClashVars[mClashVars_pos[v_index]] = mClashVars[--mClashVars_length];
				mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[v_index];
				mClashVars_pos[v_index] = -1;
			}
            mClashEdges[mClashEdges_pos[edge_id]] = mClashEdges[--mClashEdges_length];
            mClashEdges_pos[mClashEdges[mClashEdges_length]] = mClashEdges_pos[edge_id];
            mClashEdges_pos[edge_id] = -1;
		} else if (mVarsColor[v_index] == c) {
			if (mClashVars_pos[v_index] == -1) {
				mClashVars[mClashVars_length] = v_index;
				mClashVars_pos[v_index] = mClashVars_length++;
			}
            mClashEdges[mClashEdges_length] = edge_id;
			mClashEdges_pos[edge_id] = mClashEdges_length++;
		}
	}

	mVarsColor[id] = pos;
	
	if (mClash[id][pos] == 0 && mClashVars_pos[id] >= 0) {
		mClashVars[mClashVars_pos[id]] = mClashVars[--mClashVars_length];
		mClashVars_pos[mClashVars[mClashVars_length]] = mClashVars_pos[id];
		mClashVars_pos[id] = -1;
	}
	return;
}

void build() {
	for(int i=0; i<mVars_length; i++) {
		mTabuV[mVars[i]] = true;
		for(int j = 1; j <= color_size; j++) {
			mClash[mVars[i]][j] = 0;
			mTabu[mVars[i]][j] = 0;
		}
	}
	mClashEdges_length = 0;
	for(int i = 0; i < mEdges_length; i++) {
        int e = mEdges[i], x, y;
		int type = get_type(edges[e].x);
		if (type == 0) {
			x = edges[e].x;
			y = edges[e].y;
			mClash[x][mExpsColor[y]] += edges[e].w;
			mClash[y][mExpsColor[x]] += edges[e].w;
		} else if (type == 1) {
			x = to_index(edges[e].x);
			y = to_index(edges[e].y);
			mClash[x][to_pre_color(x, to_sum_color(y, mExpsColor[y]), true)] += edges[e].w;
			mClash[y][to_pre_color(y, to_sum_color(x, mExpsColor[x]), true)] += edges[e].w;
		} else {
			x = to_index(edges[e].x);
			y = to_index(edges[e].y);
			mClash[x][to_pre_color(x, to_dif_color(y, mExpsColor[y]), false)] += edges[e].w;
			mClash[y][to_pre_color(y, to_dif_color(x, mExpsColor[x]), false)] += edges[e].w;
		}
		if (mExpsColor[edges[e].x] == mExpsColor[edges[e].y]) {
            mClashEdges[mClashEdges_length] = e;
            mClashEdges_pos[e] = mClashEdges_length++;
		}
	}
	mClashVars_length = 0;
	for (int i = 0; i < mVars_length; i++) {
		int var_id = mVars[i];
		if (mClash[var_id][mVarsColor[var_id]] > 0) {
			mClashVars[mClashVars_length] = var_id;
			mClashVars_pos[var_id] = mClashVars_length++;
		} else mClashVars_pos[var_id] = -1;
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
			update_info(move_id, move_to);
			// flag--;
		} else {
			for (int i = 0; i < mClashVars_length; i++) {
				int tmp_v = mClashVars[i];
				int tmp_s = -mClash[tmp_v][mVarsColor[tmp_v]];
				if (true) {
					if (tmp_s < min_clash) {
						cnt = 0;
						min_clash = tmp_s;
						tmp_st[cnt++] = tmp_v;
					} else if (tmp_s == min_clash) {
						tmp_st[cnt++] = tmp_v;
					}
				} else {
					if (tmp_s < min_tabu_clash) {
						tabu_cnt = 0;
						min_tabu_clash = tmp_s;
						tmp_tabu_st[tabu_cnt++] = tmp_v;
					} else if (tmp_s == min_tabu_clash) {
						tmp_tabu_st[tabu_cnt++] = tmp_v;
					}
				}
			}
			if (cnt == 0 && tabu_cnt == 0) {
				return tSminClash;
			} else if (cnt == 0 && tabu_cnt > 0) {
				v_cnt = min(max_bms_length, tabu_cnt);
				for (int i = 0; i < v_cnt; i++) {
					r = rand() % tabu_cnt;
					long long tmp_cs = 1;
					if (max_cs < tmp_cs) {
						cs_cnt = 0;
						max_cs = tmp_cs;
						tmp_bms_st[cs_cnt++] = tmp_tabu_st[r];
					} else if(max_cs == tmp_cs) tmp_bms_st[cs_cnt++] = tmp_tabu_st[r];
				}
				r = rand() % cs_cnt;
				move_id = tmp_bms_st[r];
				move_from = mVarsColor[move_id];
				for (int j = 0; j < var_color_length[move_id]; j++) {
					int tmp_c = var_color[move_id][j];
					if(tmp_c == mVarsColor[move_id]) continue;
					int tmp_s = mClash[move_id][tmp_c];
					if (check_tabu(move_id, tmp_c, iters)) {
						if (tmp_s < min_sub_clash) {
							sub_cnt = 0;
							min_sub_clash = tmp_s;
							tmp_sub_st[sub_cnt++] = tmp_c;
						} else if (tmp_s == min_sub_clash) {
							tmp_sub_st[sub_cnt++] = tmp_c;
						}
					} else {
						if (tmp_s < min_sub_tabu_clash) {
							sub_tabu_cnt = 0;
							min_sub_tabu_clash = tmp_s;
							tmp_sub_tabu_st[sub_tabu_cnt++] = tmp_c;
						} else if (tmp_s == min_sub_tabu_clash) {
							tmp_sub_tabu_st[sub_tabu_cnt++] = tmp_c;
						}
					}
				}
			} else {
				v_cnt = min(max_bms_length, cnt);
				for (int i = 0; i < v_cnt; i++) {
					r = rand() % cnt;
					long long tmp_cs = 1;
					if (max_cs < tmp_cs) {
						cs_cnt = 0;
						max_cs = tmp_cs;
						tmp_bms_st[cs_cnt++] = tmp_st[r];
					} else if (max_cs == tmp_cs) tmp_bms_st[cs_cnt++] = tmp_st[r];
				}
				r = rand() % cs_cnt;
				move_id = tmp_bms_st[r];
				move_from = mVarsColor[move_id];
				for (int j = 0; j < var_color_length[move_id]; j++) {
					int tmp_c = var_color[move_id][j];
					if(tmp_c == mVarsColor[move_id]) continue;
					int tmp_s = mClash[move_id][tmp_c];
					if (check_tabu(move_id, tmp_c, iters)) {
						if (tmp_s < min_sub_clash) {
							sub_cnt = 0;
							min_sub_clash = tmp_s;
							tmp_sub_st[sub_cnt++] = tmp_c;
						} else if (tmp_s == min_sub_clash) {
							tmp_sub_st[sub_cnt++] = tmp_c;
						}
					} else {
						if (tmp_s < min_sub_tabu_clash) {
							sub_tabu_cnt = 0;
							min_sub_tabu_clash = tmp_s;
							tmp_sub_tabu_st[sub_tabu_cnt++] = tmp_c;
						} else if (tmp_s == min_sub_tabu_clash) {
							tmp_sub_tabu_st[sub_tabu_cnt++] = tmp_c;
						}
					}
				}
			}
			if (min_sub_clash-mClash[move_id][move_from]>0 && min_sub_tabu_clash-mClash[move_id][move_from]>0) {
				if (check_tabu(move_id, move_from, iters)) {
					mTabu[move_id][mVarsColor[move_id]] = iters + rand() % tabuStep + alpha2 * mClashVars_length;
					mTabuV[move_id] = false;
				} else flag = 100;
				continue;
			}
			if ((sub_tabu_cnt>0 && min_sub_tabu_clash<min_sub_clash && (mClashEdges_length-mClash[move_id][move_from]+min_sub_tabu_clash<tSminClash)) || (sub_cnt==0)) {
				v_cnt = min(max_bms_length, sub_tabu_cnt);
				for (int i = 0; i < v_cnt; i++) {
					r = rand() % sub_tabu_cnt;
					long long tmp_bs = 1;
					if (max_bs < tmp_bs) {
						bs_cnt = 0;
						max_bs = tmp_bs;
						tmp_bms_st[bs_cnt++] = tmp_sub_tabu_st[r];
					} else if (max_bs == tmp_bs) tmp_bms_st[bs_cnt++] = tmp_sub_tabu_st[r];
				}
				r = rand() % bs_cnt;
				move_to = tmp_bms_st[r];
			} else {
				v_cnt = min(max_bms_length, sub_cnt);
				for (int i = 0; i < v_cnt; i++) {
					r = rand() % sub_cnt;
					long long tmp_bs = 1;
					if (max_bs < tmp_bs) {
						bs_cnt = 0;
						max_bs = tmp_bs;
						tmp_bms_st[bs_cnt++] = tmp_sub_st[r];
					} else if (max_bs == tmp_bs) tmp_bms_st[bs_cnt++] = tmp_sub_st[r];
				}
				r = rand() % bs_cnt;
				move_to = tmp_bms_st[r];
			}
			mTabu[move_id][mVarsColor[move_id]] = iters + rand() % tabuStep + alpha2 * mClashVars_length;
			update_info(move_id, move_to);
		}

		if (mClashEdges_length <= tSminClash) {
			tSminClash = mClashEdges_length;
			for (int i=0; i<mVars_length; i++) mVarsColor_tmp[mVars[i]] = mVarsColor[mVars[i]];
			for (int i=0; i<mClashEdges_length; i++) mClashEdges_tmp[i] = mClashEdges[i];
			if(tSminClash == 0) return tSminClash;
		}
	}
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
		
		for (int i=0; i<clash_cur; i++) {
			if (rand()%alpha4 == 0) edges[mClashEdges_tmp[i]].w++;
		}
	} else {
		mPoolMaxIter[last_id] += alpha3*max_iter;
	}

	last_id = rand()%mPool_length;
	for (int i=0; i<mVars_length; i++) {
		int v = mVars[i];
		int tmp_c = mPoolVarsColor[last_id][v];
		mVarsColor[v] = tmp_c;
		int v1 = to_sum_index(v);
		int v2 = to_dif_index(v);
		mExpsColor[v] = tmp_c;
		mExpsColor[v1] = to_sum_color(v, tmp_c);
		mExpsColor[v2] = to_dif_color(v, tmp_c);
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
	// filename = argv[1];
	// seed = atoi(argv[2]);
	// time_limit = atof(argv[3]);
	filename = "5";
	seed = 1;
	time_limit = 60;
	srand(seed);
	mRead(filename);

	mReduceVertexes();

	mStartTime();
	mGenerate();
	if(check_finish()) return 0;
	for(int iter = 0; total_time < time_limit; iter++) {
		mLocalSearch();
		if(check_finish()) break;
		mCurrentTime();
	}
	return 0;
}
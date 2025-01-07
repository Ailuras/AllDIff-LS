include "alldifferent.mzn";
% 定义n，棋盘的大小和皇后的数量
int: n;

% 用一个整数数组queens表示每个皇后的位置，其中queens[i]表示第i行皇后的列数
array [1..n] of var 1..n: queens;

% 约束：每一对皇后不在同一列
constraint alldifferent(queens);

% 约束：每一对皇后不在同一对角线
constraint forall(i, j in 1..n where i < j) (
    abs(queens[i] - queens[j]) != j - i
);

% 求解
solve satisfy;
output ["SAT"];x
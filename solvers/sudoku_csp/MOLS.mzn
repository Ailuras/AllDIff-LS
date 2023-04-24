include "alldifferent.mzn";
% 输入参数：阶数n
int: n=7;

% 拉丁方矩阵A和B的定义
array[1..n, 1..n] of var 1..n: A;
array[1..n, 1..n] of var 1..n: B;

% 约束：行和列内的元素都不相同
constraint forall(i in 1..n) (
    alldifferent([A[i, j] | j in 1..n]) /\  % A的每一行
    alldifferent([A[j, i] | j in 1..n]) /\  % A的每一列
    alldifferent([B[i, j] | j in 1..n]) /\  % B的每一行
    alldifferent([B[j, i] | j in 1..n])      % B的每一列
);

% 约束：A和B正交
constraint forall(i, j, x, y in 1..n) (
    if i == x /\ j == y then
        true
    else
        A[i, j] * n + B[i, j] != A[x, y] * n + B[x, y]
    endif
);

% 求解
solve satisfy;

% 输出结果
output ["A:\n"] ++ [show(A[i, j]) ++ if j == n then "\n" else " " endif | i, j in 1..n] ++
       ["\nB:\n"] ++ [show(B[i, j]) ++ if j == n then "\n" else " " endif | i, j in 1..n];

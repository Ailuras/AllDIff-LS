include "alldifferent.mzn";
int: n;

array [1..n] of var 1..n: interval;

constraint alldifferent(interval);

constraint forall(i, j in 1..n-1 where i < j) (
    abs(interval[i] - interval[i+1]) != abs(interval[j] - interval[j+1])
);

% 求解
solve satisfy;
output ["SAT"];
% output [show(interval)];
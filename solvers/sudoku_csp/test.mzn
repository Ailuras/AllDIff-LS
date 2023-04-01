include "globals.mzn";

int: n;

% Decision Variables
array2d(1..n, 1..n) of var 1..n: grid;

% Constraints
constraint forall(i in 1..n)( all_different(grid[i,:]) /\ all_different(grid[:,i]) );

% Solve
solve satisfy;
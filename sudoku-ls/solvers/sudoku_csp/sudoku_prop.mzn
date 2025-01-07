include "alldifferent.mzn";

int: S;
int: N = S * S;
int: digs = ceil(log(10.0,int2float(N))); % digits for output

set of int: PuzzleRange = 1..N;
set of int: SubSquareRange = 1..S;

array[1..N,1..N] of -1..N: start; %% initial board -1 = empty
array[1..N,1..N] of var PuzzleRange: puzzle;
array[1..3*N] of 0..1: proportion;

% fill initial board
constraint forall(i,j in PuzzleRange)(
    if start[i,j] > 0 then puzzle[i,j] = start[i,j] else true endif );

% All different in rows 
constraint forall (i in PuzzleRange where proportion[i] = 1) (
                   alldifferent( [ puzzle[i,j] | j in PuzzleRange ]) ); 

% All different in columns.
constraint forall (j in PuzzleRange where proportion[N+j] = 1) (
                   alldifferent( [ puzzle[i,j] | i in PuzzleRange ]) ); 

% All different in sub-squares:
constraint forall (a, o in SubSquareRange where proportion[2*N+(o-1)*S+a] = 1)(
                alldifferent( [ puzzle[(a-1) *S + a1, (o-1)*S + o1] |
                                        a1, o1 in SubSquareRange ] ) );

solve satisfy;

output  ["SAT"];
% output  [a | a, o in SubSquareRange];

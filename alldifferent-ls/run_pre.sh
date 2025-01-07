# ./solvers/sudoku_acs/sudoku_acs --alg 0 --blank --order 7 --verbose > out.log

index=$[ $1 + 10 ]
path=benchmarks/N-Queens/INST_$2/100
if [ ! -d "$path" ];then
    mkdir -p $path
fi
path=benchmarks/N-Queens/INST_$2/100/inst$2_100_
for((i=$1;i<$index;i++));do
    ./solvers/nqueens_ls/NQueen-gen $2 $i 1000 > $path$i.txt
done
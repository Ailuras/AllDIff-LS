index=$[ $1 + 5 ]
for((i=$1;i<$index;i++));do
    ./solvers/sudoku_acs/sudokusolver --alg 0 --blank --order $2 --verbose > benchmarks/INST_36x36/100/inst36x36_100_$i.txt
    line=`cat benchmarks/INST_36x36/100/inst36x36_100_$i.txt | wc -l`
    while [ $line -lt 10 ]; do
        ./solvers/sudoku_acs/sudokusolver --alg 0 --blank --order $2 --verbose > benchmarks/INST_36x36/100/inst36x36_100_$i.txt
        line=`cat benchmarks/INST_36x36/100/inst36x36_100_$i.txt | wc -l`
    done
    sed -i 's/[-+|]//g; s/^ //g' benchmarks/INST_36x36/100/inst36x36_100_$i.txt
    sed -i '/^\s*$/d' benchmarks/INST_36x36/100/inst36x36_100_$i.txt
    sed -i "1c $2" benchmarks/INST_36x36/100/inst36x36_100_$i.txt
    sed -i '1a 1' benchmarks/INST_36x36/100/inst36x36_100_$i.txt
    sed -i '$d' benchmarks/INST_36x36/100/inst36x36_100_$i.txt
done
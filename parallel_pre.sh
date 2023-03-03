cd solvers/nqueens_ls/
g++ NQueen-gen.cpp -std=c++11 -O3 -o NQueen-gen
cd ../..

for i in {0..9};do
    nohup bash run_pre.sh $[$i * 10] $1 >> out.log 2>&1 &
done

python script/gen_completion_inst.py $1

# $1:
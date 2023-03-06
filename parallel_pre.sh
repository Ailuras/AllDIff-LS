# cd solvers/nqueens_ls/
# g++ NQueen-gen.cpp -std=c++11 -O3 -o NQueen-gen
# cd ../..

# for i in {0..9};do
#     nohup bash run_pre.sh $[$i * 10] $1 >> out.log 2>&1 &
# done 

python script/gen_completion_inst.py $1

# function parallel() {
#     for prop_folder in $@/*;do
#         nohup bash get_SAT.sh $prop_folder > out.log 2>&1 &
#     done
# }

# path=benchmarks/N-Queens/INST_$1
# parallel $path
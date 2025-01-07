g++ sudoku_lsc_new.cpp -std=c++11 -O3 -o sudoku_lsc_new
# time ./sudoku_lsc_new inst49x49_50_0.txt 1 50 > new.log
# g++ sudoku_lsc_re.cpp -std=c++11 -O3 -o sudoku_lsc_re
# time ./sudoku_lsc_re inst49x49_55_0.txt 1 50 > re.log
# echo > $1
# for((i=0;i<8;i++));do
#     ./sudoku_lsc_new inst49x49_50_0.txt $i 100 >> $1
# done
# g++ sudoku_lsc_re.cpp -std=c++11 -O3 -o sudoku_lsc_re
# time ./sudoku_lsc_new inst49x49_50_0.txt 1 50 > new.log
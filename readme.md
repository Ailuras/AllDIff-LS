To compile AllDiff-LS, on Ubuntu simply type:

```bash
./Scripts/pre.sh
```

To use them separately, use the following command: 

```bash
./Sudoku-LS $file $seed $time $display(0/1)
./NQueen-LS $order $seed $time $display(0/1)
./AllInterval-LS $order $seed $time $display(0/1)
./MOLS-LS $order $seed $time $display(0/1)
```
If you want to use tools such as ortools or cplex, you need to install the corresponding python package and apply for the corresponding license. If you want to use MiniZinc, you need to have it installed. Benchmarks can be downloaded from (https://anonymous.4open.science/r/Benchmarks-1E02/). Using the converter scripts in Scripts folder, you can convert examples such as Sudoku into input formats such as Minizinc and Kissat.


The batch run script can be found in the Scripts folder. For example, you can use the following command to batch solve Sudoku instances using Sudoku-LS, and the experimental results are saved in Results.

```bash
./parallel.sh ../benchmarks/Sudoku 1000 sudoku_AllDiff-LS 1
```
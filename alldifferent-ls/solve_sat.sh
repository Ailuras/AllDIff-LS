#!/bin/bash

# 创建结果目录（如果不存在）
mkdir -p results/sat_results

# 并行求解 CNF 文件
find benchmarks_sat -name "*.cnf" | while read -r file; do
    (
        # 获取结果文件的路径
        output_file="results/sat_results/$(basename "$file" .cnf).result"

        # 记录开始时间
        start=$[$(date +%s%N)/1000000]
        echo "--------------------------------------------------" >> "$output_file"

        # 运行求解器，并将输出重定向到结果文件
        timeout 1000 solvers/kissat/build/kissat -q "$file"

        # 记录结束时间
        end=$[$(date +%s%N)/1000000]
        take=$(( end - start ))

        # 将求解时间打印到结果文件
        echo "$file : ${take} ms." >> "$output_file"
    ) &
done

# 等待所有后台进程完成
wait
#!/bin/bash

# 获取文件夹中的文件名
folder_path="./benchmarks/N-Queens/" # 请根据实际情况修改文件夹路径
files=$(ls ${folder_path})

# 循环遍历文件
for file in $files; do
    # 检查文件名是否符合"inst_num.txt"格式
    if [[ $file =~ ^inst([0-9]+)\.txt$ ]]; then
        # 提取num
        num=${BASH_REMATCH[1]}

        # 创建一个临时文件
        temp_file=$(mktemp)

        # 将指定内容写入临时文件
        echo "$num" > $temp_file
        echo "1" >> $temp_file
        for ((i = 0; i < num; i++)); do
            echo "-1" >> $temp_file
        done

        # 替换原文件内容
        cat $temp_file > ${folder_path}${file}

        # 删除临时文件
        rm $temp_file
    fi
done

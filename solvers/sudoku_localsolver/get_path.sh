#!/bin/bash

# 定义一个函数来递归遍历文件夹
print_files() {
  local folder="$1"
  local printed=0

  # 遍历文件夹中的所有文件和子文件夹
  for item in "$folder"/*; do
    if [ -f "$item" ]; then
      # 如果是文件，打印文件路径
      if [ "$printed" = 0 ]; then
        echo "$item"
        printed=1
      elif [ "$printed" = 1 ]; then
        echo "$item"
        printed=2
      elif [ "$printed" = 2 ]; then
        echo "$item"
        printed=3
      elif [ "$printed" = 3 ]; then
        echo "$item"
        printed=4
      fi
    elif [ -d "$item" ]; then
      # 如果是子文件夹，递归调用函数
      print_files "$item"
    fi
  done
}

# 检查是否提供了文件夹作为参数
if [ -z "$1" ]; then
  echo "Usage: $0 <folder>"
  exit 1
fi

# 检查提供的路径是否是一个文件夹
if [ ! -d "$1" ]; then
  echo "Error: '$1' is not a directory."
  exit 1
fi

# 调用函数来打印文件路径
print_files "$1"

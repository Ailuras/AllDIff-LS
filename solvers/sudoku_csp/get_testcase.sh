origin_path=$1

target_path=$(basename $origin_path)
target_path=${target_path%\.*}.dzn

cp $origin_path $target_path

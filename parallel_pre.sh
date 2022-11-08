for i in {0..19};do
    nohup bash run_pre.sh $[$i * 5] 6 >> out.log 2>&1 &
done
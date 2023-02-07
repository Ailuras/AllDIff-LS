for i in {0..9};do
    nohup bash run_pre.sh $[$i * 10] $1 >> out.log 2>&1 &
done

# $1:
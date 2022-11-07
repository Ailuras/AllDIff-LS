for i in {0..99};do
    line=`cat benchmarks/INST_36x36/100/inst36x36_100_$i.txt | wc -l`
    echo $line
done
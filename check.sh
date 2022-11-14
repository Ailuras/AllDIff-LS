for i in {0..99};do
    line=`cat benchmarks/INST_36x36/100/inst36x36_100_$i.txt | wc -l`
    echo $line
done
for i in {0..99};do
    line=`cat benchmarks/INST_49x49/100/inst49x49_100_$i.txt | wc -l`
    echo $line
done
for i in {0..99};do
    line=`cat benchmarks/INST_64x64/100/inst64x64_100_$i.txt | wc -l`
    echo $line
done
for i in {0..99};do
    line=`cat benchmarks/INST_81x81/100/inst81x81_100_$i.txt | wc -l`
    echo $line
done
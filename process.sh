# for i in benchmarks/INST_36x36/100/*; do
#     sed -ie 's/[-+|]//g; s/^ //g' $i
#     sed -i '/^\s*$/d' $i
#     sed -i "1c $1" $i
#     sed -i '1a 1' $i
#     sed -i '$d' $i
# done

sed -i 's/[-+|]//g; s/^ //g' test.txt
sed -i '/^\s*$/d' test.txt
sed -i "1c $1" test.txt
sed -i '1a 1' test.txt
sed -i '$d' test.txt
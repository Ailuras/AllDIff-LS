compile: g++ FastLSC.cpp -std=c++11 -O3 -o FastLSC

input: ./FastLSC file_name seed_value time_limit

output: file_name 1/0 time/min_clash seed_value 

1 means the algorithm obtains a satisfied solution.

0 means the algorithm fails to obtain a satisfied solution.

time means we print the run time when a satisfied solution is obtained. 

min_clash means we output the minimum number of conflict edges when the algrotihm fails to obtain a satisfied solution.
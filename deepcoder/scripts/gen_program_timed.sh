#! /bin/bash

model=$1
example=$2
max_length=$3
method=$4
timesfile=$5
runs=$6

for i in $(seq 1 $runs)
do
    echo "running" $i
    starttime=$(date +%s%N)
    python3 $(dirname $0)/../python/predictor.py ${model} ${example} ${method} | sed -e 's/[ ]*$//g' | $(dirname $0)/../build/src/gen_program ${max_length} $method
    endtime=$(date +%s%N)
    echo $(($endtime - $starttime)) >> ${timesfile}
done
exit $?

#! /bin/bash
model=$1
example_dir=$2
parallel_num=$3
timeout_time=$4 #sec
max_length=$5

exec_test() {
    (
        bash -c "/usr/bin/time -f '%U'\
            timeout ${timeout_time} \
            $(dirname $0)/gen_program.sh ${model} ${example_dir}/$1-example ${max_length} $2" \
            2>&1 > /dev/null \
        || echo ${timeout_time}
    ) | tail -1
}

do_test() {
    id=$1
    index=$1
    while true
    do
        if [ -e "${example_dir}/${index}-example" ]
        then
            time_sae=$(exec_test $index)
            time_dfs=$(exec_test $index dfs)
            time_none=$(exec_test $index none)
            echo ${index} ${time_none} ${time_dfs} ${time_sae} >> $(dirname $0)/result-${id}
        else
            break
        fi
        index=$((${index} + ${parallel_num}))
    done
}

for i in $(seq 0 $((${parallel_num} - 1)))
do
    do_test $i &
done

wait

pkill $0

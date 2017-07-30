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
            echo "Id(${id})-${index}: Start"
            time_dfs=$(exec_test $index dfs)
            echo "Id(${id})-${index}: DFS ${time_dfs}"
            time_none=$(exec_test $index none)
            echo "Id(${id})-${index}: None ${time_none}"
            echo ${index} ${time_none} ${time_dfs} 2>> $(dirname $0)/result-${id}
            echo "Id(${id})-${index}: Finish"
        else
            break
        fi
        index=$((${index} + ${parallel_num}))
    done
}

for i in $(seq 1 ${parallel_num})
do
    do_test $i &
done

wait

pkill $0

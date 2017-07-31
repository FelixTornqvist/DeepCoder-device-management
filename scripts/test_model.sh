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
            2>&1 > /dev/null
        result=$?
        if [ $result -eq 124 ]
        then
            echo ${timeout_time}
        elif [ $result -ne 0 ]
        then
            echo "error"
        fi
    ) | tail -1
}

do_test() {
    id=$1
    while true
    do
        x=$(cat /tmp/job-queue)
        for i in ${x}
        do
            echo ${i} "#i"
            if [ $i = "END" ]
            then
                return 0
            fi
            if [ -e "${example_dir}/${i}-example" ]
            then
                echo "Id(${id})-${i}: Start"
                time_dfs=$(exec_test $i dfs)
                echo "Id(${id})-${i}: DFS ${time_dfs}"
                time_sae=$(exec_test $i)
                echo "Id(${id})-${i}: SAE ${time_sae}"
                time_none=$(exec_test $i none)
                echo "Id(${id})-${i}: None ${time_none}"
                echo ${i} ${time_none} ${time_sae} ${time_dfs} >> $(dirname $0)/result-${id}
                echo "Id(${id})-${i}: Finish"
            fi
        done
    done
}

enqueue_jobs() {
    for i in $(seq 0 $(( $(ls ${example_dir}/*-example | wc -l) - 1)))
    do
        echo $i > /tmp/job-queue
    done
    for i in $(seq 1 ${parallel_num})
    do
        echo END > /tmp/job-queue
    done
}



rm /tmp/job-queue
mkfifo /tmp/job-queue

for i in $(seq ${parallel_num})
do
    do_test ${i} &
done

enqueue_jobs

wait

pkill $0

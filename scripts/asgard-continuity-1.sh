n=10
for d in 1 2 3 4 5 6 7 8 9 10
do
    for l in 1 2 3
    do
        ./buildCPU-batched-DP/asgard -p continuity_1 -d ${d} -l ${l} -n ${n} 1>> cpu-batched-dp-continuity_1-d${d}-l${l}-n${n}.dat 2>> cpu-batched-dp-continuity_1-d${d}-l${l}-n${n}.err
    done
done

n=10
for l in 2 3
do
    for d in 2 3 4 5 6 7 8 9 10
    do
        for PDE in continuity_3 continuity_6 fokkerplanck_1d_4p1a fokkerplanck_1d_4p2 fokkerplanck_1d_4p3 fokkerplanck_1d_4p4 fokkerplanck_1d_4p5 fokkerplanck_2d_complete diffusion_1 diffusion_2
        do
            CURRENTDIR=/global/cscratch1/sd/hbrunie/AsgardBATCHEDGEMMprofile/${PDE}/
            #mkdir -p ${CURRENTDIR}
            OMP_NUM_THREADS=32 ./buildCPU-batched-DP/asgard -p ${PDE} -d ${d} -l ${l} -n ${n} 1>> ${CURRENTDIR}/cpu-batched-dp-${PDE}-d${d}-l${l}-n${n}.dat 2>> ${CURRENTDIR}/cpu-batched-dp-${PDE}-d${d}-l${l}-n${n}.err
        done
    done
done

#!/bin/bash
#SBATCH -N 1
#SBATCH -C haswell
#SBATCH --exclusive
#SBATCH -t 02:00:00
#SBATCH -q premium
#SBATCH --perf=vtune
#Load intel and CRAY modules
module purge
MAINDIR=/global/homes/h/hbrunie/applications/asgard-project/asgard/
source ${MAINDIR}module-source.sh
 #INTEL Vtune need dynamic links
export CRAYPE_LINK_TYPE=dynamic
datadir=/global/cscratch1/sd/hbrunie/AsgardProfile/
cd $MAINDIR
export OMP_NUM_THREADS=1
VTUNE_CMD=vtune -collect hpc-performance -knob enable-stack-collection=true -data-limit=0
for PDE in diffusion_1 diffusion_2 continuity_1 continuity_2 fokkerplanck_1d_4p1a fokkerplanck_1d_4p2 fokkerplanck_1d_4p3 fokkerplanck_1d_4p4 fokkerplanck_1d_4p5 continuity_3 fokkerplanck_2d_complete continuity_6
do
    for ASGARD_BUILD in buildCPU-SP buildCPU-DP
    do
        nsteps=10
        for lparam in 2 #3
        do
            for dparam in 1 #2 3 4
            do
                        VTUNEDIR=r001hpc-stackCollection-1thread-${ASGARD_BUILD}-continuity6-l${lparam}-d${dparam}
                        #srun -n 1 -c 1 --cpu_bind=cores ${VTUNE_CMD} -r $datadir/${VTUNEDIR} ./${ASGARD_BUILD}/asgard -p ${PDE} -l ${lparam} -d ${dparam} -n ${nsteps}
                        echo "srun -n 1 -c 1 --cpu_bind=cores ${VTUNE_CMD} -r $datadir/${VTUNEDIR} ./${ASGARD_BUILD}/asgard -p ${PDE} -l ${lparam} -d ${dparam} -n ${nsteps}"
            done
        done

        #nsteps=1
        #for lparam in 4 5 6
        #do
        #    for dparam in 10 15 20 30 40 60 80 100
        #    do
        #                VTUNEDIR=r001hpc-stackCollection-1thread-${ASGARD_BUILD}-continuity6-l${lparam}-d${dparam}
        #                srun -n 1 -c 1 --cpu_bind=cores ${VTUNE_CMD} -r $datadir/${VTUNEDIR} ./${ASGARD_BUILD}/asgard -p ${PDE} -l ${lparam} -d ${dparam} -n ${nsteps}
        #    done
        #done
    done
done

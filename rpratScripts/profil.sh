#!/bin/bash
#SBATCH -C gpu
#SBATCH -N 6
#SBATCH -n 48
#SBATCH -t 60
#SBATCH --gres=gpu:8
#SBATCH -A m1759  
#SBATCH -c 10

## m1759 = nersc

module load cuda
module load pgi
module load mvapich2
module load esslurm

export MV2_ENABLE_AFFINITY=0
export OMP_PROC_BIND=TRUE
export OMP_NUM_THREADS=1

NB_Node=6

# simulation name and .exe
NAME=Huge-2mpi
EXE=main.exe
INPUT_file=euler.inputs

# info simulation
FIRST_Device=0
NB_Device_Per_Node=8


MyPath=`pwd`

echo " This script is launched from $MyPath"


ROOT_dir=$MyPath/$NAME
WORK_dir=$ROOT_dir/work
OUTPUT_dir=$ROOT_dir/output

if [ ! -d "$ROOT_dir" ]; then
        mkdir $ROOT_dir
        mkdir $WORK_dir
        mkdir $OUTPUT_dir
fi

# assume that the directories are created if ROOT_dir exists
# copy exe and input file

cp $EXE $WORK_dir/$NAME.$EXE
cp $INPUT_file $WORK_dir/$NAME.$INPUT_file

if [ -f "$WORK_dir/$NAME.$EXE" ]; then
        echo " exe has been correctly copied"
else
        echo " problem copy: exe"
        exit 1
fi

if [ -f "$WORK_dir/$NAME.$INPUT_file" ]; then
        echo " exe has been correctly copied"
else
        echo " problem copy: input file"
        exit 1
fi



# Need that each node are fully allocated

ListOneNode=$FIRST_Device

for((c=$FIRST_Device + 1 ; c< $NB_Device_Per_Node ; c++))
do
        ListOneNode=$ListOneNode,$c
done

GPU_bind=$ListOneNode

for((c = 1 ; c < $NB_Node ; c++ ))
do
        GPU_bind=$GPU_bind,$ListOneNode
done

GPU_bind=--gpu-bind="map_gpu:$GPU_bind"


echo " number of node(s): $NB_Node"
echo " Gpu bind: $GPU_bind"


## simulation run
cd $WORK_dir

echo "srun $GPU_bind  ./$NAME.$EXE $NAME.$INPUT_file"
srun $GPU_bind ./$NAME.$EXE $NAME.$INPUT_file



cp $WORK_dir/euler.time* $OUTPUT_dir/
rm $WORK_dir/euler.time*

cd $MyPath

## profile with nvprof mpi

cp /global/homes/r/rprat/SCRIPT/wrap_nvprof_mpi $WORK_dir/wrap_nvprof_mpi

if [ -f "$WORK_dir/wrap_nvprof_mpi" ]; then
        echo " wrap nvprof mpi has been correctly copied"
else
        echo " problem copy: wrap nvprof mpi"
        exit 1
fi

cd $WORK_dir
echo "srun nvprof ./wrap_nvprof_mpi $NAME.$EXE $NAME.$INPUT_file"
srun $GPU_bind nvprof ./wrap_nvprof_mpi ./$NAME.$EXE $NAME.$INPUT_file

cp $WORK_dir/.nvprof-out.0 $OUTPUT_dir/trace-rank-0.nvprof

# clean trace
rm $WORK_dir/.nvprof-out*
rm $WORK_dir/euler.time*

cd $MyPath


## profile with nsight
exit 3


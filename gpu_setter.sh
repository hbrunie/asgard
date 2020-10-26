#! /bin/bash
​
# Adapted for CS-Storm from gpu_setter.sh, a rudimentary GPU affinity setter for Summit
# Usage example:
# srun -N 2 -n 16 --distrbution=block:block --cpu-bind=cores ./gpu_setter.sh gpu_app.x ...
​
# This script assumes your code does not attempt to set its own
# GPU affinity (e.g. with cudaSetDevice). Using this affinity script
# with a code that does its own internal GPU selection probably won't work!
​
# Compute device number from SLURM local rank environment variable
# This assumes 8 GPUs per node and allows for more than 1 process per GPU.
lrank=$((${SLURM_LOCALID} % 8))
​
# Use default sequence 0,1,2,3,4,5,6,7
mydevice=${lrank}
​
# Renumber to sequence 0,2,4,6,1,3,5,7
#mydevice=$((2*($mydevice%4)+$mydevice/4))
​
# CUDA_VISIBLE_DEVICES controls both what GPUs are visible to your process
# and the order they appear in. By putting "mydevice" first the in list, we
# make sure it shows up as device "0" to the process so it's automatically selected.
# The order of the other devices doesn't matter, only that all devices (0-7) are present.
CUDA_VISIBLE_DEVICES="${mydevice},0,1,2,3,4,5,6,7"
​
# Process with sed to remove the duplicate and reform the list, keeping the order we set
CUDA_VISIBLE_DEVICES=$(sed -r ':a; s/\b([[:alnum:]]+)\b(.*)\b\1\b/\1\2/g; ta; s/(,,)+/,/g; s/, *$//' <<< $CUDA_VISIBLE_DEVICES)
​
export CUDA_VISIBLE_DEVICES
​
# Verbose.
echo "Local rank $SLURM_LOCALID CUDA_VISIBLE_DEVICES=$CUDA_VISIBLE_DEVICES"
​
# Launch the application we were given
exec "$@"

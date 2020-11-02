DATADIR=${PWD}/data/gpu/
##Large problem
vtune -collect hotspots -r r000hs-dp ./buildCPU-DP/asgard -p continuity_6 -l 3 -d 4
vtune -collect hotspots -r r000hs-sp ./buildCPU-SP/asgard -p continuity_6 -l 3 -d 4
vtune -collect hpc-performance -knob enable-stack-collection=true -r001hpc-stackCollection-DP

##diffusion problem
#./build/asgard -p diffusion_1 -n 100 -l 5 -d 5 --cfl 0.001 >> ${DATADIR}/diffusion_1.dat
##advection problem
#./build/asgard -p continuity_1 -n 100 -l 5 -d 5 --cfl 0.001 >> ${DATADIR}/continuity_1.dat
#
##simple 2d problem
#./build/asgard -p diffusion_2 -n 100 -l 5 -d 5 --cfl 0.001 >> ${DATADIR}/diffusion_2.dat

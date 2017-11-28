filename=$1
source /afs/cern.ch/atlas/project/tdaq/cmake/cmake_tdaq/bin/cm_setup.sh tdaq-07-01-00 x86_64-slc6-gcc62-opt
eformat-decompress.py ${filename} dc${filename}

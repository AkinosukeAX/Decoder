#filepath=/eos/atlas/atlastier0/rucio/data17_cos/physics_Main/00316960/data17_cos.00316960.physics_Main.daq.RAW/data17_cos.00316960.physics_Main.daq.RAW._lb0824._SFO-7._0001.data
filepath=$1
dirnames=( `echo $filepath | tr -s '/' ' '`)
filename=${dirnames[8]}
filenames=( `echo ${dirnames[8]} | tr -s '.' ' '`)
Newname=${filenames[0]}.${filenames[1]}.${filenames[2]}.${filenames[3]}.${filenames[4]}.${filenames[5]}.${filenames[6]}${filenames[7]}.${filenames[8]}
Newname2=${filenames[0]}.${filenames[1]}.${filenames[2]}.${filenames[3]}.${filenames[4]}.${filenames[5]}.${filenames[6]}${filenames[7]}._0001.${filenames[8]}
eos cp ${filepath} ./${Newname}

sh efodecompress.sh ${Newname} 

source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh
lsetup root
make

./decoder -I dc${Newname2} -b -O ${Newname2}.root
rm -f ${Newname} dc${Newname2}




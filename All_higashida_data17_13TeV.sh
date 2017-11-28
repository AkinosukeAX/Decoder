RunNumber=$1
lbNumber=$2
verNumber=$3



for var in {1..8}
do
#sh all_higashida.sh /eos/atlas/atlastier0/rucio/data17_comm/physics_MinBias/${RunNumber}/data17_comm.${RunNumber}.physics_MinBias.daq.RAW/data17_comm.${RunNumber}.physics_MinBias.daq.RAW._lb0${lbNumber}._SFO-${var}._0001.data
#sh all_higashida.sh /eos/atlas/atlastier0/rucio/data17_13TeV/physics_MinBias/${RunNumber}/data17_13TeV.${RunNumber}.physics_MinBias.daq.RAW/data17_13TeV.${RunNumber}.physics_MinBias.daq.RAW._lb00${lbNumber}._SFO-${var}._0001.data
sh all.sh /eos/atlas/atlastier0/rucio/data17_13TeV/physics_Main/${RunNumber}/data17_13TeV.${RunNumber}.physics_Main.daq.RAW/data17_13TeV.${RunNumber}.physics_Main.daq.RAW._lb0${lbNumber}._SFO-${var}._000${verNumber}.data
done


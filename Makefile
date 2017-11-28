ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

EXES=decoder
OBJS=fileStartRecord.o fileNameStrings.o metadataStrings.o utils.o runParametersRecord.o dataSeparatorRecord.o fullEventData.o fileEndRecord.o roddata.o robdata.o tree.o tileCaloROD.o tgcROD.o algoBase.o

all: $(EXES)

utils.o : utils.cc utils.hh
	g++ -c -o $@ utils.cc $(ROOTFLAGS)

algoBase.o: algoBase.cc algoBase.hh
	g++ -c -o $@ algoBase.cc $(ROOTFLAGS)

tgcROD.o: tgcROD.cc tgcROD.hh utils.hh algoBase.o
	g++ -c -o $@ tgcROD.cc $(ROOTFLAGS)

tileCaloROD.o: tileCaloROD.cc tileCaloROD.hh utils.hh algoBase.o
	g++ -c -o $@ tileCaloROD.cc $(ROOTFLAGS)

roddata.o: roddata.cc roddata.hh utils.hh algoBase.o
	g++ -c -o $@ roddata.cc $(ROOTFLAGS)

robdata.o: robdata.cc robdata.hh utils.hh roddata.hh algoBase.o
	g++ -c -o $@ robdata.cc $(ROOTFLAGS)

fileEndRecord.o: fileEndRecord.cc fileEndRecord.hh utils.hh algoBase.o
	g++ -c -o $@ fileEndRecord.cc $(ROOTFLAGS)

fullEventData.o: fullEventData.cc fullEventData.hh utils.hh algoBase.o
	g++ -c -o $@ fullEventData.cc $(ROOTFLAGS)

dataSeparatorRecord.o: dataSeparatorRecord.cc dataSeparatorRecord.hh utils.hh algoBase.o
	g++ -c -o $@ dataSeparatorRecord.cc $(ROOTFLAGS)

runParametersRecord.o: runParametersRecord.cc runParametersRecord.hh utils.hh algoBase.o
	g++ -c -o $@ runParametersRecord.cc $(ROOTFLAGS)

fileStartRecord.o : fileStartRecord.cc fileStartRecord.hh utils.hh algoBase.o
	g++ -c -o $@ fileStartRecord.cc $(ROOTFLAGS)

fileNameStrings.o : fileNameStrings.cc fileNameStrings.hh utils.hh algoBase.o
	g++ -c -o $@ fileNameStrings.cc $(ROOTFLAGS)

metadatastrings.o : metadatastrings.cc metadatastrings.hh utils.hh algoBase.o
	g++ -c -o $@ metadatastrings.cc $(ROOTFLAGS)

tree.o : tree.cc tree.hh utils.hh fullEventData.hh robdata.hh roddata.hh tileCaloROD.hh
	g++ -c -o $@ tree.cc $(ROOTFLAGS)

decoder : decoder.cc $(OBJS)
	g++ -o $@ decoder.cc $(ROOTLIBS) $(ROOTFLAGS) $(OBJS)

clean:
	\rm -f $(OBJS) $(EXES) *~


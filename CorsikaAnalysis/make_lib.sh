#!/bin/sh

echo "Building library for CorsikaFileIO"
cd ../CorsikaFileIO
echo "Current DIR : ${PWD}"
g++ -shared -o lib_CORSIKA_FileIO.so -I$ROOTSYS/include -I../../include -Wall -D_FILE_OFFSET_BITS=64 -Wl,--no-as-needed -fPIC TCorsikaReader.cc TSubBlockIO.cc MParticleSubBlockOutput.cc MCorsikaReader.cc MLongfileReader.cc `root-config --ldflags --libs`
cd ../

echo "Building library for CorsikaIntern"
cd CorsikaIntern
echo "Current DIR : ${PWD}"
g++ -shared -o lib_CORSIKA_Intern.so -I$ROOTSYS/include -I../../include -Wall -D_FILE_OFFSET_BITS=64 -Wl,--no-as-needed -fPIC CParticle.cc CInteraction.cc IParticleReadout.cc MCherenkov.cc MEventEnd.cc MEventHeader.cc MLongitudinalBlock.cc MMuonProductionInfo.cc MParticle.cc MParticleBlock.cc MRunEnd.cc MRunHeader.cc TBlock.cc TLongitudinal.cc TParticleBlockEntry.cc TSubBlock.cc PersistentParticle.cc `root-config --ldflags --libs`
cd ../

echo "Building ROOT dictionary & library for CorsikaROOT"
cd CorsikaROOT
echo "Current DIR : ${PWD}"
rootcint -f ROOT_Dictionary.cxx -c -I../../include -Wall -D_FILE_OFFSET_BITS=64 -Wl,--no-as-needed -fPIC -p TCherenkov.h TLongitudinal.h TParticle.h TRun.h TShower.h DirectROOTLinkDef.h

g++ -shared -o lib_ROOT_Dict.so -Wall -D_FILE_OFFSET_BITS=64 -Wl,--no-as-needed -fPIC -I$ROOTSYS/include -I../../include ROOT_Dictionary.cxx TCherenkov.cc TLongitudinal.cc TParticle.cc TRun.cc TShower.cc `root-config --cflags --ldflags --glibs --libs`
# g++ -shared -o lib_CORSIKA_ROOT.so -Wall -D_FILE_OFFSET_BITS=64 -Wl,--no-as-needed -fPIC -I$ROOTSYS/include -I../../include TCherenkov.cc TLongitudinal.cc TParticle.cc TRun.cc TShower.cc `root-config --libs --glibs --cflags --ldflags`
cd ../

cd CorsikaAnalysis
echo "Compiling CorsikaAnalysis.exe"
g++ -I../../include -I./ -L ../../lib/unknown ../CorsikaROOT/lib_ROOT_Dict.so ../CorsikaFileIO/lib_CORSIKA_FileIO.so ../CorsikaIntern/lib_CORSIKA_Intern.so  `root-config --cflags --libs` CorsikaAnalysis.cc -o CorsikaAnaylsis.exe

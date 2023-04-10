#!/bin/sh

cd ../
echo "Compiling CorsikaAnalysis.exe"
g++ -I../include -I./CorsikaAnalysis -L ../../lib/unknown ./CorsikaROOT/lib_ROOT_Dict.so ./CorsikaFileIO/lib_CORSIKA_FileIO.so ./CorsikaIntern/lib_CORSIKA_Intern.so  `root-config --cflags --libs` CorsikaAnalysis.cc -o CorsikaAnaylsis.exe

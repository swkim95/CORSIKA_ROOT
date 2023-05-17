#!/bin/sh

echo "Compiling CorsikaAnalysis.exe"
g++ -I../../include -I./ -L ../../lib/unknown ../CorsikaROOT/lib_ROOT_Dict.so ../CorsikaFileIO/lib_CORSIKA_FileIO.so ../CorsikaIntern/lib_CORSIKA_Intern.so  `root-config --cflags --libs` CorsikaAnalysis.cc -o CorsikaAnalysis.exe

echo "Compiling CorsikaToFlat.exe"
g++ -I../../include -I./ -L ../../lib/unknown ../CorsikaROOT/lib_ROOT_Dict.so ../CorsikaFileIO/lib_CORSIKA_FileIO.so ../CorsikaIntern/lib_CORSIKA_Intern.so  `root-config --cflags --libs` CorsikaToFlat.cc -o CorsikaToFlat.exe
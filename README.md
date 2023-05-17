# CORSIKA_ROOT
Modified ROOT output for CORSIKA to Geant4 DRC simulation

# Prerequisites
- ROOT 5.34 (only tested ROOT 5.34.36)
- CORSIKA ver.77100

# Environment setting (when using cvmfs)
```
# Source ROOT 5.34.36 from CVMFS

source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/5.34.36/x86_64-centos7-gcc48-opt/root/bin/thisroot.sh 
```

# How to setup

1. Download CORSIKA version 77100
2. Unzip it in your working directory using `tar -zvxf corsika-77100.tar.gzip`
3. Inside `corsika-77100` directory, remove default `coast` directory by `rm -rf coast`
4. Clone this package inside `corsika-77100` with name `coast` 
    - `git clone https://github.com/swkim95/CORSIKA_ROOT.git coast`
5. Run environment setting (source ROOT 5.34.36 from CVMFS)
6. Compile CORSIKA with ROOTOUT option (d2 option) using `./coconut`
7. After compiling CORSIKA, run simulation (output file assumed to be produced under `/corsika-77100/run/out/`)
8. Move to `/corsika-77100/coast/CorsikaAnalysis` directory, fix CorsikaAnalysis.h `getInputFile` function to provide proper CORSIKA output ROOT file path.
9. In `/corsika-77100/coast/CorsikaAnalysis` directory, compile analysis scripts using `source make_lib.sh`
10. CorsikaAnalysis.exe, CorsikaToFlat.exe, AnalysisFlat.exe should be installed in `~/corsika-77100/coast/CorsikaAnalysis/`
11. One can create ROOT file with histograms by running `./CorsikaAnalysis.exe {RunNumber}`
    - Ex) To analyze `DAT000001.root` created under `~/corsika-77100/run/out/`, type `./CorsikaAnalysis.exe 1`
    - This will take huge time if the number of particles in each shower gets too large
12. One can create Flat Ntuple ROOT file with only necessary information for GEANT4 simulation in it by running `./CorsikaToFlat.exe {RunNumber}`
    - Ex) To make flat Ntuple ROOT file of `DAT000001.root` created under `~/corsika-77100/run/out/`, type `./CorsikaToFlat.exe 1`
    - The output file will contain each shower particle information in std::vector container
13. 


# Caution

This package will change the default ROOT dictionary library provided by CORSIKA.\
So the ROOT output files created before the change won't be able to opened & analyzed by scripts in this package.
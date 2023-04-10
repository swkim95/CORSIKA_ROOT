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
5. Compile CORSIKA with ROOTOUT option (d2 option)
6. After compiling CORSIKA, run simulation (output file assumed to be produced under `/corsika-77100/run/out/`)
7. Move to `/corsika-77100/coast/CorsikaAnalysis` directory, and compile analysis script using `source make_lib.sh` (Fix output ROOT path in CorsikaAnalysis.h before compiling it)
8. CorsikaAnalysis.exe should be installed in `~/corsika-77100/coast/CorsikaAnalysis/`
9. One can create ROOT file with histograms by running `./CorsikaAnalysis.exe {RunNumber}`
    - Ex) To analyze `DAT000001.root` created under `~/corsika-77100/run/out/`, type `./CorsikaAnalysis.exe 1`

# Caution

This package will change the default ROOT dictionary library provided by CORSIKA.\
So the ROOT output files created before the change won't be able to opened & analyzed by scripts in this package.

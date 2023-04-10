# CORSIKA_ROOT
Modified ROOT output for CORSIKA

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
5. Compile CORSIKA with ROOTOUT option
6. After compiling CORSIKA, run simulation
7. Move to `/coast/CorsikaAnalysis` directory, and compile analysis script using `source make_lib.sh` (Fix output ROOT path in CorsikaAnalysis.h before compiling it)


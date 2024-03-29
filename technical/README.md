# [Technical] 

## Downloads
NVIDIA HPC SDK <br>
https://developer.nvidia.com/nvidia-hpc-sdk-downloads

NVIDIA HPC SDK Installation <br>
https://docs.nvidia.com/hpc-sdk/hpc-sdk-install-guide/index.html#install-linux-end-usr-env-settings:~:text=In%20bash%2C%20sh%2C%20or%20ksh%2C%20use%20these%20commands%3A


## post-install
In bash, sh, or ksh, use these commands:
```bash
NVARCH=`uname -s`_`uname -m`; export NVARCH
NVCOMPILERS=/opt/nvidia/hpc_sdk; export NVCOMPILERS 
MANPATH=$MANPATH:$NVCOMPILERS/$NVARCH/24.3/compilers/man; export MANPATH
PATH=$NVCOMPILERS/$NVARCH/24.3/compilers/bin:$PATH; export PATH
``` 

make the OpenMPI commands and man pages accessible using these commands.
```bash
export PATH=$NVCOMPILERS/$NVARCH/23.1/comm_libs/mpi/bin:$PATH
export MANPATH=$MANPATH:$NVCOMPILERS/$NVARCH/23.1/comm_libs/mpi/ma
```
if properly setup: `which nvc++` should return a path

in wsl, when gpu is not visible, try
```bash
export LD_LIBRARY_PATH=/usr/lib/wsl/lib
```

## Compiling
### Normal compile
```bash 
gcc file.cpp -o file 
```

### Nvidia compile 
```bash
nvc++ file.cpp -o file
```

### OpenACC compile CPU host 
```bash
nvc++ -acc=host openacc-no-gpu.cpp -o openacc-no-gpu
```
### OpenACC compile GPU host 
```bash
nvc++ -acc=gpu -gpu=<DEFAULT_TARGET> file.cpp -o file
# To find Default Target, run the following command in the terminal: 
# nvaccelinfo
```

Example compilation:
```bash
nvc++ -acc=gpu -gpu=cc50 openacc-gpu-memory.cpp -o openacc-gpu-memory
```
Open the Task Manager to see the spike when Intel or Nvidia is ran.
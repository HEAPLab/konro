# konro
A modern resource manager

## Status

[![Building](https://github.com/HEAPLab/konro/actions/workflows/main.yml/badge.svg)](https://github.com/HEAPLab/konro/actions)

# Compilation instructions
* `mkdir build && cd build`
* `cmake ..`
* `make`

# Dependencies
- cmake
- pkg-config
- liblog4cpp5-dev
- libhwloc-dev
- libsensors-dev
- libopencv-dev
- Cuda Toolkit 

OpenCV is required only for the peopledetect demo.
If it is not installed, peopledetect is not compiled.
# Optional: Nvidia GPU support
nvidia-cuda-toolkit is required in order to compile with NVIDIA GPU monitoring. It can be installed from your package manager or by manually by following this link https://developer.nvidia.com/cuda-downloads.
In order to compile, you need to add the NVIDIA flag to cmake:
* `mkdir build && cd build`
* `cmake -DNVIDIA=ON ..`  
* `make`

If no NVIDIA GPUs are present, the manager will still work, but the monitor will just skip them.  
If nothing is specified, NVIDIA GPU support will be disabled.
# Development

QtCreator has been used as IDE for development on Ubuntu 22.04.2 LTS.
It has also been tested on Ubuntu 18.04 LTS with an updated kernel (5.4.0-135-generic.)

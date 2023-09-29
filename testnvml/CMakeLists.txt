cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 14)
file(GLOB testnvml_SOURCES "*.cpp")
file(GLOB testnvml_HEADERS "*.h")

# Find NVML library without specifying the name
#find_library(NVML_LIBRARY REQUIRED PATHS "/usr/lib/nvidia")
find_package(CUDAToolkit REQUIRED)

add_executable(testnvml ${testnvml_SOURCES} ${testnvml_HEADERS})
#target_include_directories(testnvml PRIVATE ${CUDA_INCLUDE_DIRS})
target_link_libraries(testnvml PRIVATE CUDA::nvml)

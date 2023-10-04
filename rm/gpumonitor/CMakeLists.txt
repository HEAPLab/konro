set(CMAKE_CXX_STANDARD 14)

find_package(CUDAToolkit REQUIRED)

file(GLOB gpumonitor_SOURCES "*.cpp")
file(GLOB gpumonitor_HEADERS "*.h")
add_library(gpumonitor
            ${gpumonitor_HEADERS}
            ${gpumonitor_SOURCES}
)

target_include_directories(gpumonitor INTERFACE .)
target_link_libraries(gpumonitor PUBLIC policymanager CUDA::nvml)

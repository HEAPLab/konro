set(CMAKE_CXX_STANDARD 14)

find_package(CUDAToolkit REQUIRED)

file(GLOB nvidiamonitor_SOURCES "*.cpp")
file(GLOB nvidiamonitor_HEADERS "*.h")
add_library(nvidiamonitor
            ${nvidiamonitor_HEADERS}
            ${nvidiamonitor_SOURCES}
)

target_include_directories(nvidiamonitor INTERFACE .)
target_link_libraries(nvidiamonitor PUBLIC policymanager CUDA::nvml)

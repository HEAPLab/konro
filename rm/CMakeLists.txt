set (CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LOG4CPP REQUIRED log4cpp)

add_subdirectory (platformdescription)
add_subdirectory (librmcommon)
add_subdirectory (platformcontrol)
add_subdirectory (workloadmanager)
add_subdirectory (policymanager)
add_subdirectory (platformmonitor)
if(NVIDIA)
    # Enable nvml code
    add_subdirectory (nvidiamonitor)
endif(NVIDIA)

file(GLOB konro_SOURCES "src/*.cpp")
file(GLOB konro_HEADERS "src/*.h")

include_directories(${PROJECT_SOURCE_DIR}/rm/include)

add_executable (konro ${konro_SOURCES} ${konro_HEADERS})
target_compile_definitions(konro PUBLIC KONRO_RM)

if(NVIDIA)
target_link_libraries(konro PUBLIC
    konro-common
    rmcommon
    platformcontrol
    workloadmanager
    policymanager
    platformmonitor
        nvidiamonitor
    ${LOG4CPP_LIBRARIES})
else(NVIDIA)
target_link_libraries(konro PUBLIC
    konro-common
    rmcommon
    platformcontrol
    workloadmanager
    policymanager
    platformmonitor
    ${LOG4CPP_LIBRARIES})
endif(NVIDIA)

configure_file(konro.ini.in konro.ini)

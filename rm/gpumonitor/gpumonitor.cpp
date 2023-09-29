#include "gpumonitor.h"
#include <chrono>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include "monitorevent.h"
#include "platformtemperature.h"
#include "platformpower.h"
#include "platformload.h"
#include "threadname.h"
#include "tsplit.h"
#include "monitorgpuevent.h"

using namespace std;

struct GpuMonitor::GpuMonitorImpl {
    unsigned int nvml_devices_;
    bool initialized = false;
    vector<string> gpuChips;
    nvmlReturn_t result;


    GpuMonitorImpl() {
        init();
    }

    ~GpuMonitorImpl() {
        fini();
    }

    void init() {

        result = nvmlInit_v2();
        if (result != NVML_SUCCESS)
        {
            log4cpp::Category::getRoot().info( "Failed to initialize NVML: %s", nvmlErrorString(result));
            this->initialized = false;
        }else{
            this->initialized = true;
            result = nvmlDeviceGetCount(&nvml_devices_);
            log4cpp::Category::getRoot().info( "Found %d NVML devices", nvml_devices_);
            for (unsigned int i = 0; i < nvml_devices_; i++)
            {
                nvmlDevice_t device;
                result = nvmlDeviceGetHandleByIndex(i, &device);
                if (result != NVML_SUCCESS)
                {
                    log4cpp::Category::getRoot().info( "Failed to get device handle: %s", nvmlErrorString(result));
                    continue; // Continue with the next device on error
                }
                char name[NVML_DEVICE_NAME_BUFFER_SIZE];
                result = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);

                gpuChips.emplace_back(name);
            }
        }
    }

    void fini() {
        if (this->initialized){
            result = nvmlShutdown();
            if (result != NVML_SUCCESS)
            {
                log4cpp::Category::getRoot().info( "Failed to initialize NVML: %s", nvmlErrorString(result));
            }
        }
    }

//    void setCpuModuleNames(const std::string &names) {
//        cpuChips = rmcommon::tsplit(names, ",");
//    }

//    void setBatteryModuleNames(const std::string &names) {
//        batteryChips = rmcommon::tsplit(names, ",");
//        log4cpp::Category::getRoot().info("setBatteryModuleNames: %s", names.c_str());
//        if (batteryChips.empty()) {
//            log4cpp::Category::getRoot().info("setBatteryModuleNames: no battery names");
//        } else {
//            log4cpp::Category::getRoot().info("setBatteryModuleNames: %s", batteryChips[0].c_str());
//        }
//    }

    bool isGpuChip(string chip) {
        return find(begin(gpuChips), end(gpuChips), chip) != end(gpuChips);
    }

    void handleGpuTemp(nvmlDevice_t device, rmcommon::GpuTemperature &gpuTemp) {
        unsigned int temp;
        result = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temp);
        if (result == NVML_SUCCESS) {
//            rmcommon::ComponentTemperature componentTemp;
//            componentTemp.label_ = "GPU";
//            componentTemp.temp_ = temp;
            gpuTemp.addGpuTemperature(temp);
        }
    }

    void handleGpuPower(nvmlDevice_t device, rmcommon::GpuPower &gpuPower) {
        unsigned int power;
        result = nvmlDeviceGetPowerUsage(device, &power);
        if (result == NVML_SUCCESS) {
            gpuPower.addGpuPower(power);
        }
    }

    void handleGpuLoad(nvmlDevice_t device, rmcommon::GpuLoad &gpuLoad) {
        nvmlUtilization_t utilization;
        result = nvmlDeviceGetUtilizationRates(device, &utilization);
        if (result == NVML_SUCCESS) {
            gpuLoad.addGpuLoad(utilization.gpu);
        }
    }




    /*!
     * Detect each gpu available on the machine and calls the
     * appropriate handler function to store information about its
     * current status.
     */
     void handleGPUs(rmcommon::GpuLoad &gpuLoad, rmcommon::GpuPower &gpuPower, rmcommon::GpuTemperature &gpuTemperature){
        if (this->initialized){
            for (unsigned int i = 0; i < nvml_devices_; i++)
            {
                nvmlDevice_t device;
                result = nvmlDeviceGetHandleByIndex(i, &device);
                if (result != NVML_SUCCESS)
                {
                    log4cpp::Category::getRoot().info( "Failed to get device handle: %s", nvmlErrorString(result));
                    continue; // Continue with the next device on error
                }
                handleGpuTemp(device, gpuTemperature);
                handleGpuPower(device, gpuPower);
                handleGpuLoad(device, gpuLoad);
            }
        }
     }
};

GpuMonitor::GpuMonitor(rmcommon::EventBus &eventBus, int monitorPeriod) :
    pimpl_(new GpuMonitorImpl()),
    cat_(log4cpp::Category::getRoot()),
    monitorPeriod_(monitorPeriod),
    bus_(eventBus)
{
}

GpuMonitor::~GpuMonitor()
{
}

//void PlatformMonitor::setCpuModuleNames(const std::string &names)
//{
//    pimpl_->setCpuModuleNames(names);
//}
//
//void PlatformMonitor::setBatteryModuleNames(const std::string &names)
//{
//    pimpl_->setBatteryModuleNames(names);
//}

void GpuMonitor::run()
{
    setThreadName("GPUMONITOR");
    cat_.info("GPUMONITOR running");
    while (!stopped()) {
        for (int i = 0; i < monitorPeriod_ && !stopped() ; ++i) {
            this_thread::sleep_for(chrono::seconds(1));
        }
        if (!stopped()) {
            rmcommon::GpuTemperature gpuTemp;
            rmcommon::GpuPower gpuPower;
            rmcommon::GpuLoad gpuLoad;
            pimpl_->handleGPUs(gpuLoad, gpuPower, gpuTemp);
            bus_.publish(new rmcommon::MonitorGpuEvent(gpuTemp, gpuPower, gpuLoad));
        }
    }
    cat_.info("GPUMONITOR exiting");
}

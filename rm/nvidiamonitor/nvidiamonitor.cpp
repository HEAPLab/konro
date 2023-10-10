#include "nvidiamonitor.h"
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

struct NvidiaMonitor::NvidiaMonitorImpl {
    unsigned int nvml_devices_;
    bool initialized = false;
    vector <string> nvidiaChips;
    nvmlReturn_t result;

    NvidiaMonitorImpl() {
        init();
    }

    ~NvidiaMonitorImpl() {
        fini();
    }

    void init() {
        result = nvmlInit_v2();
        if (result != NVML_SUCCESS) {
            log4cpp::Category::getRoot().info("Failed to initialize NVML: %s", nvmlErrorString(result));
            this->initialized = false;
        } else {
            this->initialized = true;
            result = nvmlDeviceGetCount(&nvml_devices_);
            log4cpp::Category::getRoot().info("Found %d NVML devices", nvml_devices_);
            for (unsigned int i = 0; i < nvml_devices_; i++) {
                nvmlDevice_t device;
                result = nvmlDeviceGetHandleByIndex(i, &device);
                if (result != NVML_SUCCESS) {
                    log4cpp::Category::getRoot().info("Failed to get device handle: %s", nvmlErrorString(result));
                    continue; // Continue with the next device on error
                }
                char name[NVML_DEVICE_NAME_BUFFER_SIZE];
                result = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);

                nvidiaChips.emplace_back(name);
            }
        }
    }

    void fini() {
        if (this->initialized) {
            result = nvmlShutdown();
            if (result != NVML_SUCCESS) {
                log4cpp::Category::getRoot().info("Failed to initialize NVML: %s", nvmlErrorString(result));
            }
        }
    }

    bool isNvidiaChip(string chip) {
        return find(begin(nvidiaChips), end(nvidiaChips), chip) != end(nvidiaChips);
    }

    void handleNvidiaTemp(nvmlDevice_t device, rmcommon::GpuTemperature &gpuTemp) {
        unsigned int temp;
        result = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temp);
        if (result == NVML_SUCCESS) {
            gpuTemp.addGpuTemperature(temp);
        }
    }

    void handleNvidiaPower(nvmlDevice_t device, rmcommon::GpuPower &gpuPower) {
        unsigned int power;
        result = nvmlDeviceGetPowerUsage(device, &power);
        if (result == NVML_SUCCESS) {
            gpuPower.addGpuPower(power);
        }
    }

    void handleNvidiaLoad(nvmlDevice_t device, rmcommon::GpuLoad &gpuLoad) {
        nvmlUtilization_t utilization;
        result = nvmlDeviceGetUtilizationRates(device, &utilization);
        if (result == NVML_SUCCESS) {
            gpuLoad.addGpuLoad(utilization.gpu);
        }
    }

    /*!
     * Detect each nvidia gpu available on the machine and calls the
     * appropriate handler function to store information about its
     * current status.
     */
    void
    handleNVGPUs(rmcommon::GpuLoad &gpuLoad, rmcommon::GpuPower &gpuPower, rmcommon::GpuTemperature &gpuTemperature) {
        if (this->initialized) {
            for (unsigned int i = 0; i < nvml_devices_; i++) {
                nvmlDevice_t device;
                result = nvmlDeviceGetHandleByIndex(i, &device);
                if (result != NVML_SUCCESS) {
                    log4cpp::Category::getRoot().info("Failed to get device handle: %s", nvmlErrorString(result));
                    continue; // Continue with the next device on error
                }
                handleNvidiaTemp(device, gpuTemperature);
                handleNvidiaPower(device, gpuPower);
                handleNvidiaLoad(device, gpuLoad);
            }
        }
    }
};

NvidiaMonitor::NvidiaMonitor(rmcommon::EventBus &eventBus, int monitorPeriod) :
        pimpl_(new NvidiaMonitorImpl()),
        cat_(log4cpp::Category::getRoot()),
        monitorPeriod_(monitorPeriod),
        bus_(eventBus) {
}

NvidiaMonitor::~NvidiaMonitor() {
}

void NvidiaMonitor::run() {
    setThreadName("NVIDIAMONITOR");
    cat_.info("NVIDIAMONITOR running");
    while (!stopped()) {
        for (int i = 0; i < monitorPeriod_ && !stopped(); ++i) {
            this_thread::sleep_for(chrono::seconds(1));
        }
        if (!stopped()) {
            rmcommon::GpuTemperature gpuTemp;
            rmcommon::GpuPower gpuPower;
            rmcommon::GpuLoad gpuLoad;
            pimpl_->handleNVGPUs(gpuLoad, gpuPower, gpuTemp);
            bus_.publish(new rmcommon::MonitorGpuEvent(gpuTemp, gpuPower, gpuLoad));
        }
    }
    cat_.info("NVIDIAMONITOR exiting");
}

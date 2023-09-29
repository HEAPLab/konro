#include <iostream>
#include <nvml.h>

// Function to handle device-specific operations
void performDeviceOperations(nvmlDevice_t device, unsigned int index)
{
    nvmlReturn_t result;

    char name[NVML_DEVICE_NAME_BUFFER_SIZE];
    result = nvmlDeviceGetName(device, name, NVML_DEVICE_NAME_BUFFER_SIZE);
    if (result != NVML_SUCCESS)
    {
        std::cout << "Device " << index << " - Failed to get device name: " << nvmlErrorString(result) << std::endl;
    }
    else
    {
        std::cout << "Device " << index << " name: " << name << std::endl;

//        // turn on the GPU
//        result = nvmlDeviceSetPersistenceMode(device, nvmlEnableState_enum::NVML_FEATURE_DISABLED);


        unsigned int temperature;
        result = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temperature);
        if (result != NVML_SUCCESS)
        {
            std::cout << "Device " << index << " - Failed to get device temperature: " << nvmlErrorString(result) << std::endl;
        }
        else
        {
            std::cout << "Device " << index << " temperature: " << temperature << " [C]" << std::endl;
        }

        // Fan speed
        unsigned int speed;
        result = nvmlDeviceGetFanSpeed(device, &speed);
        if (result != NVML_SUCCESS)
        {
            std::cout << "Device " << index << " - Failed to get device fan speed: " << nvmlErrorString(result) << std::endl;
        }
        else
        {
            std::cout << "Device " << index << " fan speed: " << speed << std::endl;
        }
        // Utilization rate
        nvmlUtilization_t utilization;
        result = nvmlDeviceGetUtilizationRates(device, &utilization);
        if (result != NVML_SUCCESS)
        {
            std::cout << "Device " << index << " - Failed to get device utilization: " << nvmlErrorString(result) << std::endl;
        }
        else
        {
            std::cout << "Device " << index << " utilization: " << utilization.gpu << " [%GPU] " << utilization.memory << " [%memory]" << std::endl;
        }

        // Battery info
        nvmlPstates_t powerState;
        nvmlDeviceGetPowerState(device, &powerState);
        if (result != NVML_SUCCESS)
        {
            std::cout << "Device " << index << " - Failed to get device power state: " << nvmlErrorString(result) << std::endl;
        }
        else
        {
            std::cout << "Device " << index << " power state: " << powerState << " [0-15(minimum performance]" << std::endl;
        }


        unsigned int power;
        result = nvmlDeviceGetPowerUsage(device, &power);
        if (result != NVML_SUCCESS)
        {
            std::cout << "Device " << index << " - Failed to get device power usage: " << nvmlErrorString(result) << std::endl;
        }
        else
        {
            std::cout << "Device " << index << " power usage: " << power << "[mW]" << std::endl;
        }

        unsigned int power_limit;
        result = nvmlDeviceGetPowerManagementLimit(device, &power_limit);
        if (result != NVML_SUCCESS)
        {
            std::cout << "Device " << index << " - Failed to get device power limit: " << nvmlErrorString(result) << std::endl;
        }
        else
        {
            std::cout << "Device " << index << " power limit: " << power_limit << "[mW]" << std::endl;
        }

        unsigned int clock;
        result = nvmlDeviceGetClockInfo(device, NVML_CLOCK_GRAPHICS, &clock);
        if (result != NVML_SUCCESS)
        {
            std::cout << "Device " << index << " - Failed to get device clock: " << nvmlErrorString(result) << std::endl;
        }
        else
        {
            std::cout << "Device " << index << " clock: " << clock << "[MHz]" << std::endl;
        }

        // PCI info
        nvmlPciInfo_t pci;
        result = nvmlDeviceGetPciInfo(device, &pci);
        if (result != NVML_SUCCESS)
        {
            std::cout << "Device " << index << " - Failed to get device PCI info: " << nvmlErrorString(result) << std::endl;
        }
        else
        {
            std::cout << "Device " << index << " PCI BUS id: " << pci.busId << std::endl;
        }

        // Compute mode
        nvmlComputeMode_t compute_mode;
        result = nvmlDeviceGetComputeMode(device, &compute_mode);
        if (result == NVML_ERROR_NOT_SUPPORTED)
        {
            std::cout << "This is not a CUDA-capable device" << std::endl;
        }
        else if (result != NVML_SUCCESS)
        {
            std::cout << "Device " << index << " - Failed to get device compute mode: " << nvmlErrorString(result) << std::endl;
        }
        else
        {
            std::cout << "Device " << index << " compute mode: " << compute_mode << std::endl;

            // Try to change compute mode
            std::cout<< "Changing device's compute mode from '" << compute_mode << "' to '" << NVML_COMPUTEMODE_PROHIBITED << "'" << std::endl;

            result = nvmlDeviceSetComputeMode(device, NVML_COMPUTEMODE_PROHIBITED);
            if (NVML_ERROR_NO_PERMISSION == result)
                std::cout<< "\t\t Need root privileges to do that: " << nvmlErrorString(result) << std::endl;
            else if (NVML_ERROR_NOT_SUPPORTED == result)
                std::cout<< " Compute mode prohibited not supported. You might be running on\n"
                            "windows in WDDM driver model or on non-CUDA capable GPU.\n";
            else if (NVML_SUCCESS != result)
            {
                std::cout << "\t\t Failed to set compute mode for device %i: %s\n" << index << nvmlErrorString(result) << std::endl;
            }
            else
            {
                std::cout << "\t\t restored compute mode to '" << compute_mode << "'" << std::endl;
                result = nvmlDeviceSetComputeMode(device, compute_mode);
                if (NVML_SUCCESS != result)
                {
                    std::cout << "\t\t Failed to restore compute mode for device %i: %s\n" << index << nvmlErrorString(result) << std::endl;
                }
            }
        }
    }
}

int main()
{
    nvmlReturn_t result;

    result = nvmlInit_v2();
    if (result != NVML_SUCCESS)
    {
        std::cout << "Failed to initialize NVML: " << nvmlErrorString(result) << std::endl;
        return 1;
    }

    // Get the number of devices
    unsigned int nvml_devices;
    result = nvmlDeviceGetCount(&nvml_devices);
    std::cout << "Found " << nvml_devices << " devices" << std::endl;

    for (unsigned int i = 0; i < nvml_devices; i++)
    {
        nvmlDevice_t device;
        result = nvmlDeviceGetHandleByIndex(i, &device);
        if (result != NVML_SUCCESS)
        {
            std::cout << "Failed to get device handle: " << nvmlErrorString(result) << std::endl;
            continue; // Continue with the next device on error
        }

        // Perform device-specific operations
        performDeviceOperations(device, i);
    }

    result = nvmlShutdown();

    if (result != NVML_SUCCESS)
    {
        std::cout << "Failed to initialize NVML: " << nvmlErrorString(result) << std::endl;
        return 1;
    }

    return 0;
}
//
// Copyright (c) 2018 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL.h>
#include <nanoHAL_v2.h>
#include <nanoWeak.h>
#include <Esp32_os.h>
#include <nvs.h>

// initialization of configuration manager
// provided as weak so it can be replaced at target level, if required because of the target implementing the storage with a mechanism other then saving to flash
void ConfigurationManager_Initialize()
{
    // enumerate the blocks
    ConfigurationManager_EnumerateConfigurationBlocks();
};

// Enumerates the configuration blocks from the configuration flash sector 
// it's implemented with 'weak' attribute so it can be replaced at target level if a different persistance mechanism is used
void ConfigurationManager_EnumerateConfigurationBlocks()
{

}

// Gets a configuration block from the configuration block stored in the NVS block, 
// maybe better to store each config item under a separate key which would work better if config block changes
bool ConfigurationManager_GetConfigurationBlock(void* configurationBlock, DeviceConfigurationOption configuration, uint32_t configurationIndex)
{
	bool result = FALSE;
	nvs_handle out_handle;
	size_t     blobSize;

    // validate if the requested block exists
    if(configuration == DeviceConfigurationOption_Network)
    {
        // set blob size
        blobSize = sizeof(HAL_Configuration_NetworkInterface);
    }
    else if(configuration == DeviceConfigurationOption_Wireless80211Network)
    {
        // set blob size
        blobSize = sizeof(Configuration_Wireless80211NetworkInterface);
    }

	// Open NVS storage using NanoFramework namespace
	esp_err_t ret = nvs_open( NVS_NAMESPACE, NVS_READONLY, &out_handle);
	if ( ret == ESP_OK )
	{
	   // copy the config block content to the pointer in the argument
		ret = nvs_get_blob(out_handle, NVS_NETWORK_CONFIG1, (void *)configurationBlock, &blobSize);
		if (ret == ESP_OK) result = TRUE;
		nvs_close(out_handle);
	}

    return result;
}

// Stores the network configuration block to the EPS32 storage 
bool ConfigurationManager_StoreConfigurationBlock(void* configurationBlock, DeviceConfigurationOption configuration, uint32_t configurationIndex, uint32_t blockSize)
{
	bool result = FALSE;
	nvs_handle out_handle;
    bool requiresEnumeration = FALSE;
	size_t     blobSize;

    if(configuration == DeviceConfigurationOption_Network)
    {
        // set blob size
        blobSize = sizeof(HAL_Configuration_NetworkInterface);
    }
    else if(configuration == DeviceConfigurationOption_Wireless80211Network)
    {
        // set blob size
        blobSize = sizeof(Configuration_Wireless80211NetworkInterface);
    }
    else if(configuration == DeviceConfigurationOption_All)
    {
        // particular situation where we are receiving the full configuration block

        // always enumerate the blocks again after storing it
        // requiresEnumeration = TRUE;
    }

    // copy the config block content to the pointer in the argument
	esp_err_t ret = nvs_open( NVS_NAMESPACE, NVS_READWRITE, &out_handle);
	if ( ret == ESP_OK )
	{
		ret = nvs_set_blob(out_handle, NVS_NETWORK_CONFIG1, (void *)configurationBlock, blobSize);
		if ( ret == ESP_OK ) 
		{
			ret = nvs_commit(out_handle);
			if (ret == ESP_OK) result = TRUE;
		}
		nvs_close(out_handle);
	}

    if(requiresEnumeration)
    {
        // perform enumeration of configuration blocks
        ConfigurationManager_EnumerateConfigurationBlocks();
    }

    return result;
}

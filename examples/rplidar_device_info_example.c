#include <rtthread.h>
#include "rplidar.h"

#define RPLIDAR_DEVICE_NAME    "rplidar"    /* 设备名称 */

static int rplidar_device_info_example(int argc, char *argv[])
{
    rt_err_t ret;

    // Find lidar device
    rt_device_t lidar = rp_lidar_create(RPLIDAR_DEVICE_NAME);
    if(lidar == RT_NULL)
    {
        rt_kprintf("Failed to find device %s\n", RPLIDAR_DEVICE_NAME);
        return -1;
    }

    // Init lidar
    ret = rp_lidar_init(lidar);
    if(ret != RT_EOK)
    {
        rt_kprintf("Failed to init lidar device\n");    
        return -1;
    }

    // Read device info
    rplidar_response_device_info_t* info = (rplidar_response_device_info_t*) rt_malloc(sizeof(rplidar_response_device_info_t));
    if(info == RT_NULL)
    {
        rt_kprintf("Failed to malloc memory for device info data\n");
    }
    ret = rp_lidar_get_device_info(lidar, info, 1000);
    if(ret == RT_EOK)
    {
        rt_kprintf("Model           : A%dM%d\n", (info->model >> 4) & 0x0F, info->model & 0x0F);
        rt_kprintf("Firmware Version: %d.%d\n", (info->firmware_version >> 8) & 0x00FF, info->firmware_version & 0x00FF);
        rt_kprintf("Hardware Version: %d\n", info->hardware_version);
        rt_kprintf("Serial Number   : ");
        for(size_t i = 0; i < 16; i++)
        {
            rt_kprintf("%02X", info->serialnum[i]);
        }
        rt_kprintf("\n");
    }
    else
    {
        rt_kprintf("Failed to communicate with lidar device\n");
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(rplidar_device_info_example, rplidar get device info example);

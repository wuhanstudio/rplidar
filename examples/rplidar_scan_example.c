#include <rtthread.h>
#include "rplidar.h"

#define RPLIDAR_DEVICE_NAME    "rplidar"    /* 设备名称 */

static int rplidar_scan_example(int argc, char *argv[])
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

    // Stop lidar
    ret = rp_lidar_scan(lidar, 1000);
    if(ret == RT_EOK)
    {
        rt_kprintf("Lidar started scanning\n");
    }
    else
    {
        rt_kprintf("Failed to communicate with lidar device\n");
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(rplidar_scan_example, rplidar scan example);

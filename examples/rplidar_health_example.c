#include <rtthread.h>
#include "rplidar.h"

#define RPLIDAR_DEVICE_NAME    "rplidar"    /* 设备名称 */

static int rplidar_health_example(int argc, char *argv[])
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

    // Read health info
	rplidar_response_device_health_t* health = (rplidar_response_device_health_t*) rt_malloc(sizeof(rplidar_response_device_health_t));
	if(health == RT_NULL)
	{
	    rt_kprintf("Failed to malloc memory for health data\n");
	}
	ret = rp_lidar_get_health(lidar, health);
	if(ret == RT_EOK)
	{
        rt_kprintf("Health Status: %s\n", health->status == 0? "OK" : "Error");
	}

	return RT_EOK;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(rplidar_health_example, rplidar get health info example);

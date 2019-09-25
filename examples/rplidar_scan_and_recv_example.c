#include <rtthread.h>
#include <stdio.h>
#include "rplidar.h"

#define RPLIDAR_DEVICE_NAME    "rplidar"    /* 设备名称 */

void rplidar_scan_and_recv_entry(void* parameters)
{
    rt_err_t ret;

    // Find lidar device
    rt_device_t lidar = rp_lidar_create(RPLIDAR_DEVICE_NAME);
    if(lidar == RT_NULL)
    {
        printf("Failed to find device %s\n", RPLIDAR_DEVICE_NAME);
        return;
    }

    // Init lidar
    ret = rp_lidar_init(lidar);
    if(ret != RT_EOK)
    {
        printf("Failed to init lidar device\n");    
        return;
    }

    // Start lidar
    ret = rp_lidar_scan(lidar, 1000);
    if(ret == RT_EOK)
    {
        printf("Lidar started scanning\n");
    }
    else
    {
        printf("Failed to communicate with lidar device\n");
    }

    // Read scan data
    
    for(int i = 0; i < 30; i++)
    {
        rplidar_response_measurement_node_t* node = (rplidar_response_measurement_node_t*) rt_malloc(sizeof(rplidar_response_measurement_node_t));
        if(node == RT_NULL)
        {
            printf("Failed to malloc memory for lidar data\n");
        }
        ret = rp_lidar_get_scan_data(lidar, node, 1000);
        if(ret == RT_EOK)
        {
            printf("%s Theta: %03.2f Dist: %08.2f Q: %d \n", 
                (node->sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT) ?"S":" ",
                (node->angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0f,
                node->distance_q2/4.0f,
                node->sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
        }
        else
        {
            printf("Failed to communicate with lidar device\n");
        }
    }

    // Stop lidar
    ret = rp_lidar_stop(lidar);
    if(ret == RT_EOK)
    {
        rt_kprintf("Lidar stopped scanning\n");
    }
    else
    {
        rt_kprintf("Failed to communicate with lidar device\n");
    }
}

static void rplidar_scan_and_recv_example(int argc,char *argv[])
{
    rt_thread_t thread = rt_thread_create("rplidar", rplidar_scan_and_recv_entry, RT_NULL, 1024, 8, 100);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[rplidar] New thread rplidar\n");
    }
    else
    {
        rt_kprintf("[rplidar] Failed to create thread rplidar\n");
    }
}
MSH_CMD_EXPORT(rplidar_scan_and_recv_example, rplidar scan and recev example);

#include "rplidar.h"
#include <stdio.h>
#include <string.h>

//#define DRV_DEBUG
#define LOG_TAG             "rplidar"
#include <drv_log.h>

rt_device_t rp_lidar_create(const char* lidar_name)
{
    rt_device_t lidar = rt_device_find(lidar_name);
    if (!lidar)
    {
        LOG_E("Find %s failed!", lidar_name);
        return RT_NULL;
    }
    return lidar;
}

rt_err_t rp_lidar_init(rt_device_t lidar)
{
    rt_err_t ret = rt_device_init(lidar);
    if (ret != RT_EOK)
    {
        LOG_E("Initialize device failed!\n");
        return -RT_ERROR;
    }
    return RT_EOK;
}

static u_result rp_lidar_recev_data(rt_device_t lidar, _u8* buffer, size_t len, _u32 timeout)
{
    int  recvPos = 0;
    _u32 startTs = rt_tick_get();
    _u32 waitTime;

    LOG_I("%d bytes to receive", len);
    while ((waitTime=rt_tick_get() - startTs) <= rt_tick_from_millisecond(timeout)) 
    {
        buffer[recvPos] = rp_lidar_get_char(lidar);
        LOG_I("Received %02X", buffer[recvPos]);
        recvPos++;
        if (recvPos == len)
        {
            LOG_I("Received content\n");
            return RESULT_OK;
        }    
    }
    return RESULT_OPERATION_TIMEOUT;
}

static u_result rp_lidar_wait_resp_header(rt_device_t lidar, rplidar_ans_header_t * header, _u32 timeout)
{
    int  recvPos = 0;
    _u8  recvBuffer[sizeof(rplidar_ans_header_t)];

    _u32 startTs = rt_tick_get();
    _u32 waitTime;

    while ((waitTime=rt_tick_get() - startTs) <= rt_tick_from_millisecond(timeout)) 
    {
        size_t remainSize = sizeof(rplidar_ans_header_t) - recvPos;
        LOG_I("%d bytes to receive", remainSize);
        for(size_t i = 0; i < remainSize; i++)
        {
            recvBuffer[i] = rp_lidar_get_char(lidar);
            LOG_I("Received %02X", recvBuffer[i]);
            switch (recvPos) 
            {
            case 0:
                if (recvBuffer[i] != RPLIDAR_ANS_SYNC_BYTE1) 
                {
                   continue;
                }
                
                break;
            case 1:
                if (recvBuffer[i] != RPLIDAR_ANS_SYNC_BYTE2) 
                {
                    recvPos = 0;
                    continue;
                }
                break;
            }
            recvPos++;
            if (recvPos == sizeof(rplidar_ans_header_t)) 
            {
                LOG_I("Received header\n");
                _u8* header_temp = (_u8*) header;
                memcpy(header_temp, recvBuffer, sizeof(rplidar_ans_header_t));
                return RESULT_OK;
            }
        }
    }

    return RESULT_OPERATION_TIMEOUT;
}

rt_err_t rp_lidar_get_health(rt_device_t lidar, rplidar_response_device_health_t* health)
{
    u_result res;
    rt_device_t serial = (rt_device_t)lidar->user_data;
    char health_cmd[] = {RPLIDAR_CMD_SYNC_BYTE, RPLIDAR_CMD_GET_DEVICE_HEALTH};
    rt_device_write(serial, 0, health_cmd, (sizeof(health_cmd)));
    
    rplidar_ans_header_t* header = (rplidar_ans_header_t*) rt_malloc(sizeof(rplidar_ans_header_t));
    res = rp_lidar_wait_resp_header(lidar, header, 1000);

    if(res != RESULT_OK)
    {
        return RESULT_OPERATION_TIMEOUT;
    }
    res = rp_lidar_recev_data(lidar, (_u8*) health, sizeof(rplidar_response_device_health_t), 1000);
    if(res != RESULT_OK)
    {
        return RESULT_OPERATION_TIMEOUT;
    }

    return RT_EOK;
}

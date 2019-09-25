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
        LOG_E("Initialize device failed!");
        return -RT_ERROR;
    }

    return rt_device_open(lidar, RT_NULL);
}

static u_result rp_lidar_recev_data(rt_device_t lidar, _u8* buffer, size_t len, _u32 timeout)
{
    int  recvPos = 0;
    _u32 startTs = rt_tick_get();
    _u32 waitTime;

    LOG_I("%d bytes to receive", len);
    while ((waitTime=rt_tick_get() - startTs) <= rt_tick_from_millisecond(timeout)) 
    {
        rt_uint8_t ch;
        rt_device_read(lidar, 0, &ch, 1);
        buffer[recvPos] = ch;
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
            rt_uint8_t ch;
            if(rt_device_read(lidar, 0, &ch, 1) != 1)
            {
                return RESULT_OPERATION_TIMEOUT;
            };
            recvBuffer[i] = ch;
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
    rt_err_t res;

    // Write get health command
    char health_cmd[] = {RPLIDAR_CMD_SYNC_BYTE, RPLIDAR_CMD_GET_DEVICE_HEALTH};
    rt_device_write(lidar, 0, (void*)health_cmd , (sizeof(health_cmd)));

    // Maloc meory
    rplidar_ans_header_t* header = (rplidar_ans_header_t*) rt_malloc(sizeof(rplidar_ans_header_t));
    if(header == RT_NULL)
    {
        LOG_E("Out of memory");
        return -RT_ERROR;
    }

    // Receive header
    res = rp_lidar_wait_resp_header(lidar, header, 1000);
    if(res != RESULT_OK)
    {
        LOG_E("Read Timout");
        return RESULT_OPERATION_TIMEOUT;
    }

    // Receive data
    res = rp_lidar_recev_data(lidar, (_u8*) health, sizeof(rplidar_response_device_health_t), 1000);
    if(res != RESULT_OK)
    {
        return RESULT_OPERATION_TIMEOUT;
    }

    return RT_EOK;
}

rt_err_t rp_lidar_device_info(rt_device_t lidar, rplidar_response_device_info_t* info)
{
    rt_err_t res;

    // Write get info command
    char info_cmd[] = {RPLIDAR_CMD_SYNC_BYTE, RPLIDAR_CMD_GET_DEVICE_INFO};
    rt_device_write(lidar, 0, (void*)info_cmd , (sizeof(info_cmd)));

    // Maloc meory
    rplidar_ans_header_t* header = (rplidar_ans_header_t*) rt_malloc(sizeof(rplidar_ans_header_t));
    if(header == RT_NULL)
    {
        LOG_E("Out of memory");
        return -RT_ERROR;
    }

    // Receive header
    res = rp_lidar_wait_resp_header(lidar, header, 1000);
    if(res != RESULT_OK)
    {
        LOG_E("Read Timout");
        return RESULT_OPERATION_TIMEOUT;
    }

    // Receive data
    res = rp_lidar_recev_data(lidar, (_u8*) info, sizeof(rplidar_response_device_info_t), 1000);
    if(res != RESULT_OK)
    {
        return RESULT_OPERATION_TIMEOUT;
    }

    return RT_EOK;
}

rt_err_t rp_lidar_stop(rt_device_t lidar)
{
    rt_err_t res;

    // Write stop scanning command
    char stop_cmd[] = {RPLIDAR_CMD_SYNC_BYTE, RPLIDAR_CMD_STOP};
    if( rt_device_write(lidar, 0, (void*)stop_cmd , sizeof(stop_cmd)) == sizeof(stop_cmd) )
    {
        res = RT_EOK;
    }

    return res;
}

rt_err_t rp_lidar_reset(rt_device_t lidar)
{
    rt_err_t res;

    // Write soft reset command
    char reset_cmd[] = {RPLIDAR_CMD_SYNC_BYTE, RPLIDAR_CMD_RESET};
    if( rt_device_write(lidar, 0, (void*)reset_cmd , sizeof(reset_cmd)) == sizeof(reset_cmd) )
    {
        res = RT_EOK;
    }

    return res;
}

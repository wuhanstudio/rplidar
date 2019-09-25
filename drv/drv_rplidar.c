#include "rt_rplidar.h"
#include "rplidar_cmd.h"

//#define DRV_DEBUG
#define LOG_TAG             "drv.rplidar"
#include <drv_log.h>

#ifndef RPLIDAR_UART_NAME
#define RPLIDAR_UART_NAME     "uart3"
#endif

/* 用于接收消息的信号量 */
struct rt_semaphore rx_sem;

/* 接收数据回调函数 */
static rt_err_t rp_lidar_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    if(size>0)
    {
        rt_sem_release(&rx_sem);
    }

    return RT_EOK;
}

rt_err_t rp_lidar_get_char(struct rt_rplidar_device *rplidar, char* ch, rt_uint32_t timeout)
{
    rt_device_t serial = rplidar->parent.user_data;
    while (rt_device_read(serial, 0, ch, 1) == 0)
    {
        rt_sem_control(&rx_sem, RT_IPC_CMD_RESET, RT_NULL);
        // rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        if(rt_sem_take(&rx_sem, rt_tick_from_millisecond(timeout)) != RT_EOK)
        {
            return -RT_ETIMEOUT;
        }
        else
        {
            continue;
        }
    }
    return RT_EOK;
}

struct rplidar_device
{
    struct rt_rplidar_device rplidar;
    char *name;
};

static struct rplidar_device rplidar_obj =
{
    .name = "rplidar"
};

rt_err_t rplidar_init(struct rt_rplidar_device *rplidar)
{
    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    return RT_EOK;
}

rt_err_t rplidar_open(struct rt_rplidar_device *rplidar)
{
    rt_device_t serial = rplidar->parent.user_data;
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, rp_lidar_input);
    return RT_EOK;
}

rt_err_t rplidar_close(struct rt_rplidar_device *rplidar)
{

    return RT_EOK;
}

rt_size_t rplidar_read(struct rt_rplidar_device *rplidar, rt_off_t pos, void *buffer, rt_size_t size)
{
    size_t read_count = 0;
    char* buffer_ch = (char*) buffer;
    for(size_t i = 0; i < size; i++)
    {
        char ch = 0;
        if(rp_lidar_get_char(rplidar, &ch, 1000) == RT_EOK)
        {
            buffer_ch[pos + read_count] = ch;
            LOG_I("Read %02X", buffer_ch[pos + read_count]);
            read_count++;
        }
        else
        {
            LOG_E("Read error");
        }
    }
    return read_count;
}

rt_size_t rplidar_write(struct rt_rplidar_device *rplidar, rt_off_t pos, const void *buffer, rt_size_t size)
{
    rt_device_t serial = (rt_device_t)(rplidar->parent.user_data);
    rt_size_t ret = rt_device_write(serial, 0, buffer, size);

    return ret;
}

rt_err_t rplidar_control(struct rt_rplidar_device *rplidar, rt_uint32_t cmd, void *args)
{
    /*
    rt_err_t result;
    TIM_HandleTypeDef *tim_handler = (TIM_HandleTypeDef *)rplidar->parent.user_data;

    result = RT_EOK;

    switch (cmd)
    {
    case rplidar_CMD_ENABLE:
        HAL_TIM_Encoder_Start(tim_handler, TIM_CHANNEL_ALL);
        break;
    case rplidar_CMD_DISABLE:
        HAL_TIM_Encoder_Stop(tim_handler, TIM_CHANNEL_ALL);
        break;
    case rplidar_CMD_CLEAR_COUNT:
        __HAL_TIM_SET_COUNTER(tim_handler, 0);
        break;
    default:
        result = -RT_ENOSYS;
        break;
    }

    return result;
    */
    return RT_EOK;
}

static const struct rt_rplidar_ops _ops =
{
    .init = rplidar_init,
    .open = rplidar_open,
    .close = rplidar_close,
    .read = rplidar_read,
    .write = rplidar_write,
    .control = rplidar_control,
};

int hw_rplidar_init(void)
{
    int result;

    rt_device_t serial;

    result = RT_EOK;
    rplidar_obj.rplidar.type = UNKNOWN_RPLIDAR_TYPE;
    rplidar_obj.rplidar.ops = &_ops;

    serial = rt_device_find(RPLIDAR_UART_NAME);
    if (!serial)
    {
        LOG_E("find %s failed!\n", RPLIDAR_UART_NAME);
        return -RT_ERROR;
    }

    if (rt_device_rplidar_register(&rplidar_obj.rplidar, rplidar_obj.name, serial) != RT_EOK)
    {
        LOG_E("%s register failed", rplidar_obj.name);
        result = -RT_ERROR;
    }

    return result;
}
INIT_BOARD_EXPORT(hw_rplidar_init);

#ifndef __RT_RPLIDAR_H__
#define __RT_RPLIDAR_H__

#include <rtthread.h>
#include <rtdevice.h>

#ifdef __cplusplus
extern "C" {
#endif

/* rplidar control command */
#define RPLIDAR_CMD_GET_TYPE       (128 + 0)    /* get a rplidar type information */
#define RPLIDAR_CMD_ENABLE         (128 + 1)    /* enable rplidar */
#define RPLIDAR_CMD_DISABLE        (128 + 2)    /* disable rplidar */

/* rplidar type */
enum rt_rplidar_type
{
    UNKNOWN_RPLIDAR_TYPE = 0x00    /* Unknown rplidar type */

};

struct rt_rplidar_device;

struct rt_rplidar_ops
{
    rt_err_t (*init)(struct rt_rplidar_device *rplidar);
    rt_err_t (*open)(struct rt_rplidar_device *rplidar);
    rt_err_t (*close)(struct rt_rplidar_device *rplidar);
    rt_size_t (*read)(struct rt_rplidar_device *rplidar, rt_off_t pos, void *buffer, rt_size_t size);
    rt_size_t (*write)(struct rt_rplidar_device *rplidar, rt_off_t pos, const void *buffer, rt_size_t size);
    rt_err_t (*control)(struct rt_rplidar_device *rplidar, rt_uint32_t cmd, void *args);
};

struct rt_rplidar_device
{
    struct rt_device parent;
    const struct rt_rplidar_ops *ops;
    enum rt_rplidar_type type;
};

rt_err_t rt_device_rplidar_register(struct rt_rplidar_device *rplidar, const char *name, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* __RPLIDAR_H__ */

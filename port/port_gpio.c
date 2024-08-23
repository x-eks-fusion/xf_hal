/**
 * @file port_gpio.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */
#include "xf_hal_port.h"
#include <stdio.h>
#include <stdlib.h>

/* ==================== [Defines] =========================================== */

#define XF_HAL_GPIO_DEFAULT_DIRECTION   XF_HAL_GPIO_DIR_IN
#define XF_HAL_GPIO_DEFAULT_SPEED       1000000
#define XF_HAL_GPIO_DEFAULT_PULL        XF_HAL_GPIO_PULL_NONE
#define XF_HAL_GPIO_DEFAULT_INTR_ENABLE  0
#define XF_HAL_GPIO_DEFAULT_INTR_TYPE   XF_HAL_GPIO_INTR_TYPE_DISABLE
#define XF_HAL_GPIO_DEFAULT_CALLBACK    xf_hal_gpio_intr_type_cb
#define XF_HAL_GPIO_DEFAULT_USER_DATA   NULL

/* ==================== [Typedefs] ========================================== */

typedef struct _port_gpio_t {
    uint32_t port;
    const char *config1;
    const char *config2;
} port_gpio_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static int port_gpio_open(xf_hal_dev_t *dev);
static int port_gpio_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_gpio_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_gpio_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static int port_gpio_close(xf_hal_dev_t *dev);

// 用户实现id的转换方式
static uint32_t _gpio_id_to_port(uint32_t id);

// 底层sdk的操作函数
static void _gpio_init(uint32_t gpio_port, uint8_t direction, uint8_t pull);
static void _gpio_deinit(uint32_t gpio_port);
static void _gpio_set_isr(uint32_t gpio_port, uint8_t edge,
                          void (*callback)(uint32_t gpio_port, uint8_t edge));
static void _gpio_set_speed(uint32_t gpio_port, uint32_t speed);
static void _gpio_set_config1(uint32_t gpio_port, const char *config1);
static void _gpio_set_config2(uint32_t gpio_port, const char *config2);
static void _gpio_set_level(uint32_t gpio_port, bool level);
static bool _gpio_get_level(uint32_t gpio_port);

// 转换对接函数
static void _callback(uint32_t gpio_port, uint8_t edge);

// 默认回调函数
static void xf_hal_gpio_intr_type_cb(xf_gpio_num_t gpio_num, bool level, void *user_data);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

#define XF_HAL_GPIO_UNUSED(x)   (void)(x)

/* ==================== [Global Functions] ================================== */

void xf_hal_GPIO_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_gpio_open,
        .ioctl = port_gpio_ioctl,
        .write = port_gpio_write,
        .read = port_gpio_read,
        .close = port_gpio_close,
    };
    xf_hal_gpio_register(&ops);
}

/* ==================== [Static Functions] ================================== */
static int port_gpio_open(xf_hal_dev_t *dev)
{
    port_gpio_t *gpio = (port_gpio_t *)malloc(sizeof(port_gpio_t));
    if (gpio == NULL) {
        return -1;
    }

    gpio->port = _gpio_id_to_port(dev->id);
    gpio->config1 = "这里用于保存一些上层用不到的参数";
    gpio->config2 = "我也不知道写啥，随便写写咯";

    dev->platform_data = gpio;

    return 0;
}

static int port_gpio_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_gpio_config_t *gpio_config = (xf_hal_gpio_config_t *)config;
    port_gpio_t *gpio = (port_gpio_t *)dev->platform_data;

    if (cmd & XF_HAL_GPIO_CMD_DIRECTION || cmd & XF_HAL_GPIO_CMD_PULL) {
        _gpio_init(gpio->port, gpio_config->direction, gpio_config->pull);
    }

    if (cmd & XF_HAL_GPIO_CMD_SPEED) {
        _gpio_set_speed(gpio->port, gpio_config->speed);
    }

    if (cmd & XF_HAL_GPIO_CMD_INTR_ENABLE || cmd & XF_HAL_GPIO_CMD_INTR_TYPE) {
        // 理论上要创建一个task等待中断回调产生中断
        bool level = _gpio_get_level(gpio->port);
        gpio_config->isr.callback(dev->id, level, gpio_config->isr.user_data);
        _gpio_set_isr(gpio->port, gpio_config->intr_type, _callback);
    }

    if (cmd == XF_HAL_GPIO_CMD_ALL) {
        _gpio_set_config1(gpio->port, gpio->config1);
        _gpio_set_config2(gpio->port, gpio->config2);
    }

    if (cmd == XF_HAL_GPIO_CMD_DEFAULT) {
        gpio_config->direction      = XF_HAL_GPIO_DEFAULT_DIRECTION;
        gpio_config->speed          = XF_HAL_GPIO_DEFAULT_SPEED;
        gpio_config->pull           = XF_HAL_GPIO_DEFAULT_PULL;
        gpio_config->intr_enable     = XF_HAL_GPIO_DEFAULT_INTR_ENABLE;
        gpio_config->intr_type      = XF_HAL_GPIO_DEFAULT_INTR_TYPE;
        gpio_config->isr.callback   = XF_HAL_GPIO_DEFAULT_CALLBACK;
        gpio_config->isr.user_data  = XF_HAL_GPIO_DEFAULT_USER_DATA;
    }


    return 0;
}

static int port_gpio_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    bool *level = (bool *)buf;
    port_gpio_t *gpio = (port_gpio_t *)dev->platform_data;
    *level = _gpio_get_level(gpio->port);

    return 0;
}

static int port_gpio_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    bool level = *(bool *)buf;
    port_gpio_t *gpio = (port_gpio_t *)dev->platform_data;
    _gpio_set_level(gpio->port, level);

    return 0;
}

static int port_gpio_close(xf_hal_dev_t *dev)
{
    port_gpio_t *gpio = (port_gpio_t *)dev->platform_data;
    _gpio_deinit(gpio->port);
    free(gpio);
    return 0;
}

static uint32_t _gpio_id_to_port(uint32_t id)
{
    return id;
}

static void _gpio_init(uint32_t gpio_port, uint8_t direction, uint8_t pull)
{
    printf("\ngpio init\n");
    printf("\ndirection:%d\n", direction);
    printf("\npull:%d\n", pull);
}

static void _gpio_deinit(uint32_t gpio_port)
{
    printf("\ngpio deinit\n");
}

static void _gpio_set_isr(uint32_t gpio_port, uint8_t edge,
                          void (*callback)(uint32_t gpio_port, uint8_t edge))
{
    printf("\ngpio set isr:%d\n", edge);
    callback(gpio_port, edge);
}

static void _gpio_set_speed(uint32_t gpio_port, uint32_t speed)
{
    printf("\ngpio set speed:%d\n", speed);
}

static void _gpio_set_config1(uint32_t gpio_port, const char *config1)
{
    printf("\ngpio set config1:%s\n", config1);
}

static void _gpio_set_config2(uint32_t gpio_port, const char *config2)
{
    printf("\ngpio set config2:%s\n", config2);
}

static void _gpio_set_level(uint32_t gpio_port, bool level)
{
    printf("\ngpio set level:%d\n", level);
}

static bool _gpio_get_level(uint32_t gpio_port)
{
    printf("\ngpio get level\n");
    return 1;
}

static void _callback(uint32_t gpio_port, uint8_t edge)
{
    // 中断回调中发送给上层的callback任务响应
}

static void xf_hal_gpio_intr_type_cb(xf_gpio_num_t gpio_num, bool level, void *user_data)
{
    XF_HAL_GPIO_UNUSED(gpio_num);
    XF_HAL_GPIO_UNUSED(level);
    XF_HAL_GPIO_UNUSED(user_data);
}

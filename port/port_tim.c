/**
 * @file port_tim.c
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

#define XF_HAL_TIM_DEFAULT_TICK_FREQ_HZ 1000*100
#define XF_HAL_TIM_DEFAULT_COUNT_DIR    XF_HAL_TIM_COUNT_DIR_UP
#define XF_HAL_TIM_DEFAULT_ACTIVE       false
#define XF_HAL_TIM_DEFAULT_CALLBACK     xf_hal_tim_event_cb
#define XF_HAL_TIM_DEFAULT_USER_DATA    NULL

/* ==================== [Typedefs] ========================================== */

typedef struct _port_tim_t {
    uint32_t port;
    const char *config1;
    const char *config2;
} port_tim_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static int port_tim_open(xf_hal_dev_t *dev);
static int port_tim_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_tim_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_tim_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static int port_tim_close(xf_hal_dev_t *dev);

// 用户实现id的转换方式
static uint32_t _tim_id_to_port(uint32_t id);

// 底层sdk的操作函数
static void _tim_init(uint32_t tim_port, bool count_dir, uint32_t tick_freq_hz);
static void _tim_deinit(uint32_t tim_port);
static void _tim_active(uint32_t tim_port);
static void _tim_callback_disable(uint32_t tim_port);
static void _tim_set_callback(uint32_t tim_port,
                              void (*callback)(uint32_t tim_port, void *user_data),
                              void *user_data);
static void _tim_set_config1(uint32_t tim_port, const char *config1);
static void _tim_set_config2(uint32_t tim_port, const char *config2);
static uint32_t _tim_read_ms(uint32_t tim_port);

// 转换对接函数
static void _callback(uint32_t tim_port, void *user_data);

// 默认回调函数
static void xf_hal_tim_event_cb(xf_tim_num_t tim_num, uint32_t ticks, void *user_data);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */
#define XF_HAL_TIM_UNUSED(x)   (void)(x)


/* ==================== [Global Functions] ================================== */

void xf_hal_TIM_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_tim_open,
        .ioctl = port_tim_ioctl,
        .write = port_tim_write,
        .read = port_tim_read,
        .close = port_tim_close,
    };
    xf_hal_tim_register(&ops);
}

/* ==================== [Static Functions] ================================== */

static int port_tim_open(xf_hal_dev_t *dev)
{
    port_tim_t *tim = (port_tim_t *)malloc(sizeof(port_tim_t));
    if (tim == NULL) {
        return -1;
    }

    tim->port = _tim_id_to_port(dev->id);
    tim->config1 = "这里用于保存一些上层用不到的参数";
    tim->config2 = "我也不知道写啥，随便写写咯";

    dev->platform_data = tim;

    return 0;
}

static int port_tim_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_tim_config_t *tim_config = (xf_hal_tim_config_t *)config;
    port_tim_t *tim = (port_tim_t *)dev->platform_data;

    if (cmd & XF_HAL_TIM_CMD_TICK_FREQ_HZ || cmd & XF_HAL_TIM_CMD_COUNT_DIR) {
        _tim_init(tim->port, tim_config->count_dir, tim_config->tick_freq_hz);
    }

    if (cmd & XF_HAL_TIM_CMD_ACTIVE) {
        if (tim_config->active) {
            _tim_active(tim->port);
        } else {
            _tim_callback_disable(tim->port);
        }
    }

    if (cmd & XF_HAL_TIM_CMD_CB) {
        uint32_t ms = _tim_read_ms(tim->port);
        tim_config->cb.callback(dev->id, ms, tim_config->cb.user_data);
        _tim_set_callback(tim->port, _callback, NULL);
    }

    if (cmd == XF_HAL_TIM_CMD_ALL) {
        _tim_set_config1(dev->id, tim->config1);
        _tim_set_config2(dev->id, tim->config2);
    }

    if (cmd == XF_HAL_TIM_CMD_DEFAULT) {
        tim_config->tick_freq_hz    = XF_HAL_TIM_DEFAULT_TICK_FREQ_HZ;
        tim_config->count_dir       = XF_HAL_TIM_DEFAULT_COUNT_DIR;
        tim_config->active          = XF_HAL_TIM_DEFAULT_ACTIVE;
        tim_config->cb.callback     = XF_HAL_TIM_DEFAULT_CALLBACK;
        tim_config->cb.user_data    = XF_HAL_TIM_DEFAULT_USER_DATA;
    }

    return 0;
}

static int port_tim_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    uint32_t ms = 0;
    port_tim_t *tim = (port_tim_t *)dev->platform_data;
    ms = _tim_read_ms(tim->port);
    *(uint32_t *)buf = ms;
    return 0;
}

static int port_tim_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    return 0;
}

static int port_tim_close(xf_hal_dev_t *dev)
{
    port_tim_t *tim = (port_tim_t *)dev->platform_data;
    _tim_deinit(tim->port);
    free(tim);
    return 0;
}

static uint32_t _tim_id_to_port(uint32_t id)
{
    return id;
}

static void _tim_init(uint32_t tim_port, bool count_dir, uint32_t tick_freq_hz)
{
    printf("\ntim init\n");
}

static void _tim_deinit(uint32_t tim_port)
{
    printf("\ntim deinit\n");
}

static void _tim_active(uint32_t tim_port)
{
    printf("\ntim callback enable\n");
}

static void _tim_callback_disable(uint32_t tim_port)
{
    printf("\ntim callback disable\n");
}

static void _tim_set_callback(uint32_t tim_port,
                              void (*callback)(uint32_t tim_port, void *user_data), void *user_data)
{
    printf("\ntim set callback\n");
    callback(tim_port, user_data);
}

static void _tim_set_config1(uint32_t tim_port, const char *config1)
{
    printf("\ntim set config1:%s\n", config1);
}

static void _tim_set_config2(uint32_t tim_port, const char *config2)
{
    printf("\ntim set config2:%s\n", config2);
}

static uint32_t _tim_read_ms(uint32_t tim_port)
{
    return 123456;
}

static void _callback(uint32_t tim_port, void *user_data)
{
    // 中断回调中发送给上层的callback任务响应
}

static void xf_hal_tim_event_cb(xf_tim_num_t tim_num, uint32_t ticks, void *user_data)
{
    XF_HAL_TIM_UNUSED(tim_num);
    XF_HAL_TIM_UNUSED(ticks);
    XF_HAL_TIM_UNUSED(user_data);
}


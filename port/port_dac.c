/**
 * @file port_dac.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-15
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_port.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================== [Defines] =========================================== */

#define XF_HAL_DAC_DEFAULT_ENABLE       false
#define XF_HAL_DAC_DEFAULT_RESOLUTION   10
#define XF_HAL_DAC_DEFAULT_SPEED        1000000
#define XF_HAL_DAC_DEFAULT_VALUE_MAX    3300000
#define XF_HAL_DAC_DEFAULT_VERF_MV      3300

/* ==================== [Typedefs] ========================================== */

typedef struct _port_dac_t {
    uint32_t port;
    const char *config1;
    const char *config2;
} port_dac_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static int port_dac_open(xf_hal_dev_t *dev);
static int port_dac_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_dac_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_dac_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static int port_dac_close(xf_hal_dev_t *dev);

// 用户实现id的转换方式
static uint32_t _dac_id_to_port(uint32_t id);

// 底层sdk的操作函数
static void _dac_init(uint32_t dac_port);
static void _dac_deinit(uint32_t dac_port);
static void _dac_set_speed(uint32_t dac_port, uint32_t speed);
static void _dac_set_resolution(uint32_t dac_port, uint32_t resolution);
static uint32_t _dac_get_value_max(uint32_t dac_port);
static uint32_t _dac_get_verf_mv(uint32_t dac_port);
static void _dac_enable(uint32_t dac_port, const char *config1,
                        const char *config2);
static void _dac_disable(uint32_t dac_port);
static void _dac_write(uint32_t dac_port, uint32_t value);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_hal_DAC_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_dac_open,
        .ioctl = port_dac_ioctl,
        .write = port_dac_write,
        .read = port_dac_read,
        .close = port_dac_close,
    };
    xf_hal_dac_register(&ops);
}

/* ==================== [Static Functions] ================================== */

static int port_dac_open(xf_hal_dev_t *dev)
{
    port_dac_t *dac = (port_dac_t *)malloc(sizeof(port_dac_t));
    if (dac == NULL) {
        return -1;
    }

    dac->port = _dac_id_to_port(dev->id);
    dac->config1 = "这里用于保存一些上层用不到的参数";
    dac->config2 = "我也不知道写啥，随便写写咯";

    dev->platform_data = dac;

    _dac_init(dac->port);

    return 0;
}

static int port_dac_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_dac_config_t *dac_config = (xf_hal_dac_config_t *)config;
    port_dac_t *dac = (port_dac_t *)dev->platform_data;

    if (cmd == XF_HAL_DAC_CMD_DEFAULT) {
        dac_config->enable = XF_HAL_DAC_DEFAULT_ENABLE;
        dac_config->resolution = XF_HAL_DAC_DEFAULT_RESOLUTION;
        dac_config->speed = XF_HAL_DAC_DEFAULT_SPEED;
        dac_config->value_max = _dac_get_value_max(dac->port);
        dac_config->verf_mv = _dac_get_verf_mv(dac->port);
    }

    if (cmd & XF_HAL_DAC_CMD_ENABLE) {
        if (dac_config->enable) {
            _dac_enable(dac->port, dac->config1, dac->config2);
        } else {
            _dac_disable(dac->port);
        }
    }

    if (cmd & XF_HAL_DAC_CMD_RESOLUTION) {
        _dac_set_resolution(dac->port, dac_config->resolution);
    }

    if (cmd & XF_HAL_DAC_CMD_SPEED) {
        _dac_set_speed(dac->port, dac_config->speed);
    }

    return 0;
}

static int port_dac_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    // no need
    return 0;
}

static int port_dac_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_dac_t *dac = (port_dac_t *)dev->platform_data;
    _dac_write(dac->port, *(uint32_t *)buf);
    return 0;
}

static int port_dac_close(xf_hal_dev_t *dev)
{
    port_dac_t *dac = (port_dac_t *)dev->platform_data;
    _dac_deinit(dac->port);
    free(dac);
    return 0;
}

static uint32_t _dac_id_to_port(uint32_t id)
{
    return id;
}

static void _dac_init(uint32_t dac_port)
{
    printf("\ndac init!\n");
}

static void _dac_deinit(uint32_t dac_port)
{
    printf("\ndac deinit!\n");
}

static void _dac_set_speed(uint32_t dac_port, uint32_t speed)
{
    printf("\ndac set speed: %d!\n", speed);
}

static void _dac_set_resolution(uint32_t dac_port, uint32_t resolution)
{
    printf("\ndac set resolution: %d!\n", resolution);
}

static uint32_t _dac_get_value_max(uint32_t dac_port)
{
    return XF_HAL_DAC_DEFAULT_VALUE_MAX;
}

static uint32_t _dac_get_verf_mv(uint32_t dac_port)
{
    return XF_HAL_DAC_DEFAULT_VERF_MV;
}

static void _dac_enable(uint32_t dac_port, const char *config1,
                        const char *config2)
{
    printf("\ndac set config1: %s!\n", config1);
    printf("\ndac set config2: %s!\n", config2);
    printf("\ndac enable\n");
}

static void _dac_disable(uint32_t dac_port)
{
    printf("\ndac disable\n");
}

static void _dac_write(uint32_t dac_port, uint32_t value)
{
    printf("\ndac write value:%d\n", value);
}

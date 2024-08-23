/**
 * @file port_adc.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-13
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

#define XF_HAL_ADC_DEFAULT_ENABLE       false
#define XF_HAL_ADC_DEFAULT_MODE         XF_HAL_ADC_MODE_SIGLE
#define XF_HAL_ADC_DEFAULT_RESOLUTION   10
#define XF_HAL_ADC_DEFAULT_SAMPLE_RATE  1000

/* ==================== [Typedefs] ========================================== */

typedef struct _port_adc_t {
    uint32_t port;
    const char *config1;
    const char *config2;
} port_adc_t;


/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static int port_adc_open(xf_hal_dev_t *dev);
static int port_adc_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_adc_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_adc_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static int port_adc_close(xf_hal_dev_t *dev);

// 用户实现id的转换方式
static uint32_t _adc_id_to_port(uint32_t id);

// 底层sdk的操作函数
static void _adc_init(uint32_t adc_port);
static void _adc_deinit(uint32_t adc_port);
static void _adc_set_resolution(uint32_t adc_port, uint32_t resolution);
static void _adc_set_sample_rate(uint32_t adc_port, uint32_t sample_rate);
static void _adc_enable(uint32_t adc_port, const char *config1,
                        const char *config2);
static void _adc_disable(uint32_t adc_port);
static void _adc_read(uint32_t adc_port, uint32_t *buffer, uint32_t count);
/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_hal_ADC_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_adc_open,
        .ioctl = port_adc_ioctl,
        .write = port_adc_write,
        .read = port_adc_read,
        .close = port_adc_close,
    };
    xf_hal_adc_register(&ops);
}


/* ==================== [Static Functions] ================================== */
static int port_adc_open(xf_hal_dev_t *dev)
{
    port_adc_t *adc = (port_adc_t *)malloc(sizeof(port_adc_t));
    if (adc == NULL) {
        return -1;
    }

    adc->port = _adc_id_to_port(dev->id);
    adc->config1 = "这里用于保存一些上层用不到的参数";
    adc->config2 = "我也不知道写啥，随便写写咯";

    dev->platform_data = adc;

    _adc_init(adc->port);

    return 0;
}

static int port_adc_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_adc_config_t *adc_config = (xf_hal_adc_config_t *)config;
    port_adc_t *adc = (port_adc_t *)dev->platform_data;

    if (cmd & XF_HAL_ADC_CMD_ENABLE) {
        if (adc_config->enable) {
            _adc_enable(adc->port, adc->config1, adc->config2);
        } else {
            _adc_disable(adc->port);
        }

    }

    if (cmd & XF_HAL_ADC_CMD_RESOLUTION) {
        _adc_set_resolution(adc->port, adc_config->resolution);
    }

    if (cmd & XF_HAL_ADC_CMD_SAMPLE_RATE) {
        _adc_set_sample_rate(adc->port, adc_config->sample_rate);
    }

    if (cmd == XF_HAL_ADC_CMD_DEFAULT) {
        adc_config->enable = XF_HAL_ADC_DEFAULT_ENABLE;
        adc_config->resolution = XF_HAL_ADC_DEFAULT_RESOLUTION;
        adc_config->sample_rate = XF_HAL_ADC_DEFAULT_SAMPLE_RATE;
    }

    return 0;
}

static int port_adc_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_adc_t *adc = (port_adc_t *)dev->platform_data;
    _adc_read(adc->port, buf, count);
    return 0;
}

static int port_adc_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    // no need
    return 0;
}

static int port_adc_close(xf_hal_dev_t *dev)
{
    port_adc_t *adc = (port_adc_t *)dev->platform_data;
    _adc_deinit(adc->port);
    free(adc);
    return 0;
}

static uint32_t _adc_id_to_port(uint32_t id)
{
    return id;
}

static void _adc_init(uint32_t adc_port)
{
    printf("\nadc init!\n");
}

static void _adc_deinit(uint32_t adc_port)
{
    printf("\nadc deinit!\n");
}

static void _adc_set_resolution(uint32_t adc_port, uint32_t resolution)
{
    printf("\nadc set resolution: %d!\n", resolution);
}

static void _adc_set_sample_rate(uint32_t adc_port, uint32_t sample_rate)
{
    printf("\nadc set sample rate: %d!\n", sample_rate);
}


static void _adc_enable(uint32_t adc_port, const char *config1,
                        const char *config2)
{
    printf("\nadc set config1: %s!\n", config1);
    printf("\nadc set config2: %s!\n", config2);
    printf("\nadc enabled\n");
}

static void _adc_disable(uint32_t adc_port)
{
    printf("\nadc disable\n");
}

static void _adc_read(uint32_t adc_port, uint32_t *buffer, uint32_t count)
{
    static uint32_t _buffer[128];

    memset(_buffer, 0xff, sizeof(_buffer));

    memcpy(buffer, _buffer, count);
}

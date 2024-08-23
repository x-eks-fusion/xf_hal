/**
 * @file port_pwm.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-11
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */
#include "xf_hal_port.h"
#include <stdio.h>
#include <stdlib.h>

/* ==================== [Defines] =========================================== */

#define XF_HAL_PWM_DEFAULT_FREQ 1000
#define XF_HAL_PWM_DEFAULT_DUTY_US 500
#define XF_HAL_PWM_DEFAULT_ENABLE false

/* ==================== [Typedefs] ========================================== */

typedef struct _port_pwm_t {
    uint32_t port;
    const char *config1;
    const char *config2;
} port_pwm_t;


/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static int port_pwm_open(xf_hal_dev_t *dev);
static int port_pwm_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_pwm_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_pwm_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static int port_pwm_close(xf_hal_dev_t *dev);

// 用户实现id的转换方式
static uint32_t _pwm_id_to_port(uint32_t id);

// 底层sdk的操作函数
static void _pwm_init(uint32_t pwm_port);
static void _pwm_deinit(uint32_t pwm_port);
static void _pwm_set_duty(uint32_t pwm_port, uint32_t duty_ms);
static void _pwm_set_freq(uint32_t pwm_port, uint32_t freq_hz);
static void _pwm_set_config1(uint32_t pwm_port, const char *config1);
static void _pwm_set_config2(uint32_t pwm_port, const char *config2);
static void _pwm_enable(uint32_t pwm_port);
static void _pwm_disable(uint32_t pwm_port);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_hal_PWM_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_pwm_open,
        .ioctl = port_pwm_ioctl,
        .write = port_pwm_write,
        .read = port_pwm_read,
        .close = port_pwm_close,
    };
    xf_hal_pwm_register(&ops);
}

/* ==================== [Static Functions] ================================== */

static int port_pwm_open(xf_hal_dev_t *dev)
{
    port_pwm_t *pwm = (port_pwm_t *)malloc(sizeof(port_pwm_t));
    if (pwm == NULL) {
        return -1;
    }

    pwm->port = _pwm_id_to_port(dev->id);
    pwm->config1 = "这里用于保存一些上层用不到的参数";
    pwm->config2 = "我也不知道写啥，随便写写咯";

    dev->platform_data = pwm;

    _pwm_init(pwm->port);
    _pwm_set_config1(pwm->port, pwm->config1);
    _pwm_set_config2(pwm->port, pwm->config2);

    return 0;
}

static int port_pwm_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_pwm_config_t *pwm_config = (xf_hal_pwm_config_t *)config;
    port_pwm_t *pwm = (port_pwm_t *)dev->platform_data;

    if (cmd & XF_HAL_PWM_CMD_ENABLE) {
        if (pwm_config->enable) {
            _pwm_enable(pwm->port);
        } else {
            _pwm_disable(pwm->port);
        }

    }

    if (cmd & XF_HAL_PWM_CMD_FREQ) {
        _pwm_set_freq(pwm->port, pwm_config->freq / 1000);
    }

    if (cmd & XF_HAL_PWM_CMD_DUTY) {
        _pwm_set_duty(pwm->port, pwm_config->duty / 1000);
    }

    if (cmd == XF_HAL_ADC_CMD_DEFAULT) {
        pwm_config->freq = XF_HAL_PWM_DEFAULT_FREQ;
        pwm_config->duty = XF_HAL_PWM_DEFAULT_DUTY_US;
        pwm_config->enable = XF_HAL_PWM_DEFAULT_ENABLE;
    }


    return 0;
}

static int port_pwm_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    // no need
    return 0;
}

static int port_pwm_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    // no need
    return 0;
}

static int port_pwm_close(xf_hal_dev_t *dev)
{
    port_pwm_t *pwm = (port_pwm_t *)dev->platform_data;
    _pwm_deinit(pwm->port);
    free(pwm);
    return 0;
}

static uint32_t _pwm_id_to_port(uint32_t id)
{
    return id;
}

static void _pwm_init(uint32_t pwm_port)
{
    printf("\npwm init!\n");
}

static void _pwm_deinit(uint32_t pwm_port)
{
    printf("\npwm deinit!\n");
}

static void _pwm_set_duty(uint32_t pwm_port, uint32_t duty_ms)
{
    printf("\npwm set duty: %d ms!\n", duty_ms);
}

static void _pwm_set_freq(uint32_t pwm_port, uint32_t freq_hz)
{
    printf("\npwm set freq: %d ms!\n", freq_hz);
}

static void _pwm_set_config1(uint32_t pwm_port, const char *config1)
{
    printf("\npwm set config1: %s!\n", config1);
}

static void _pwm_set_config2(uint32_t pwm_port, const char *config2)
{
    printf("\npwm set config2: %s!\n", config2);
}

static void _pwm_enable(uint32_t pwm_port)
{
    printf("\npwm enabled\n");
}

static void _pwm_disable(uint32_t pwm_port)
{
    printf("\npwm disable\n");
}

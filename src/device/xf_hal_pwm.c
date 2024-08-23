/**
 * @file xf_hal_pwm.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_pwm.h"

#if XF_HAL_PWM_IS_ENABLE

#include "../kernel/xf_hal_dev.h"

/* ==================== [Defines] =========================================== */

#define TAG "hal_pwm"
#define XF_HAL_PWM_TYPE XF_HAL_PWM

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_pwm_t {
    xf_hal_dev_t dev; // 一定要放到第一个，以便后续close一起free
    xf_hal_pwm_config_t config;
} xf_hal_pwm_t;

/* ==================== [Static Prototypes] ================================= */

static xf_hal_dev_t *pwm_constructor(xf_pwm_num_t pwm_num);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

#define XF_HAL_PWM_CHECK(condition, retval,  format, ...) \
    XF_CHECK(condition, retval, TAG, format, ##__VA_ARGS__)

/* ==================== [Global Functions] ================================== */

xf_err_t xf_hal_pwm_register(const xf_driver_ops_t *driver_ops)
{
    return xf_hal_driver_register(XF_HAL_PWM_TYPE, XF_HAL_FLAG_ONLY_READ, pwm_constructor, driver_ops);
}

xf_err_t xf_hal_pwm_init(xf_pwm_num_t pwm_num, uint32_t freq, uint32_t duty)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    xf_hal_dev_t *dev = xf_hal_driver_create(XF_HAL_PWM_TYPE, pwm_num);
    XF_HAL_PWM_CHECK(!dev, XF_ERR_NOT_FOUND, "pwm init failed!");

    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    dev_pwm->config.freq = freq;
    dev_pwm->config.duty = duty;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_PWM_CMD_FREQ | XF_HAL_PWM_CMD_DUTY, &dev_pwm->config);
    XF_HAL_PWM_CHECK(err, err, "enable failed!");

    return err;
}

xf_err_t xf_hal_pwm_deinit(xf_pwm_num_t pwm_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, XF_ERR_INVALID_ARG, "pwm is not init!");

    err = xf_hal_driver_close(dev);
    XF_HAL_PWM_CHECK(err, err, "close failed!");

    return err;
}

xf_err_t xf_hal_pwm_enable(xf_pwm_num_t pwm_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, XF_ERR_INVALID_ARG, "pwm is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    dev_pwm->config.enable = true;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_PWM_CMD_ENABLE, &dev_pwm->config);
    XF_HAL_PWM_CHECK(err, err, "enable failed!");

    return err;
}

xf_err_t xf_hal_pwm_disable(xf_pwm_num_t pwm_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, XF_ERR_INVALID_ARG, "pwm is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    dev_pwm->config.enable = false;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_PWM_CMD_ENABLE, &dev_pwm->config);
    XF_HAL_PWM_CHECK(err, err, "disable failed!");

    return err;
}

xf_err_t xf_hal_pwm_set_freq(xf_pwm_num_t pwm_num, uint32_t freq)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, XF_ERR_INVALID_ARG, "pwm is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    dev_pwm->config.freq = freq;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_PWM_CMD_FREQ, &dev_pwm->config);
    XF_HAL_PWM_CHECK(err, err, "config failed!");

    return XF_OK;
}

xf_err_t xf_hal_pwm_set_duty(xf_pwm_num_t pwm_num, uint32_t duty)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, XF_ERR_INVALID_ARG, "pwm is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    dev_pwm->config.duty = duty;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_PWM_CMD_DUTY, &dev_pwm->config);
    XF_HAL_PWM_CHECK(err, err, "config failed!");

    return XF_OK;
}

xf_err_t xf_hal_pwm_set_duty_resolution(xf_pwm_num_t pwm_num, uint32_t duty_resolution)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, XF_ERR_INVALID_ARG, "pwm is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    dev_pwm->config.duty_resolution = duty_resolution;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_PWM_CMD_DUTY_RESOLUTION, &dev_pwm->config);
    XF_HAL_PWM_CHECK(err, err, "config failed!");

    return XF_OK;
}

xf_err_t xf_hal_pwm_set_gpio(xf_pwm_num_t pwm_num, xf_gpio_num_t io_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, XF_ERR_INVALID_ARG, "pwm is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    dev_pwm->config.io_num = io_num;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_PWM_CMD_IO_NUM, &dev_pwm->config);
    XF_HAL_PWM_CHECK(err, err, "config failed!");

    return XF_OK;
}

uint32_t xf_hal_pwm_get_freq(xf_pwm_num_t pwm_num)
{
    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, 0, "pwm is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    uint32_t freq = dev_pwm->config.freq;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    return freq;
}

uint32_t xf_hal_pwm_get_duty(xf_pwm_num_t pwm_num)
{
    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, 0, "pwm is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    uint32_t duty = dev_pwm->config.duty;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    return duty;
}

bool xf_hal_pwm_is_enable(xf_pwm_num_t pwm_num)
{
    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_PWM_TYPE, pwm_num);
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)dev;
    XF_HAL_PWM_CHECK(!dev_pwm, false, "pwm is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_pwm->dev.mutex);
#endif

    bool enable = dev_pwm->config.enable;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_pwm->dev.mutex);
#endif

    return enable;
}
/* ==================== [Static Functions] ================================== */

static xf_hal_dev_t *pwm_constructor(xf_pwm_num_t pwm_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    xf_hal_dev_t *dev = NULL;
    xf_hal_pwm_t *dev_pwm = (xf_hal_pwm_t *)xf_malloc(sizeof(xf_hal_pwm_t));
    XF_ASSERT(dev_pwm, NULL, TAG, "memory alloc failed!");

    dev = (xf_hal_dev_t *)dev_pwm;

    err = xf_hal_driver_open(dev, XF_HAL_PWM_TYPE, pwm_num);

    if (err) {
        XF_LOGE(TAG, "open failed!");
        xf_free(dev);
        dev = NULL;
    }

    return dev;
}

#endif

/**
 * @file xf_hal_dac.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_dac.h"

#if XF_HAL_DAC_IS_ENABLE

#include "../kernel/xf_hal_dev.h"

/* ==================== [Defines] =========================================== */

#define TAG "hal_dac"
#define XF_HAL_DAC_TYPE XF_HAL_DAC

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_dac_t {
    xf_hal_dev_t dev; // 一定要放到第一个，以便后续close一起free
    xf_hal_dac_config_t config;
} xf_hal_dac_t;

/* ==================== [Static Prototypes] ================================= */

static xf_hal_dev_t *dac_constructor(xf_dac_num_t dac_num);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

#define XF_HAL_DAC_CHECK(condition, retval,  format, ...) \
    XF_CHECK(condition, retval, TAG, format, ##__VA_ARGS__)

/* ==================== [Global Functions] ================================== */

xf_err_t xf_hal_dac_register(const xf_driver_ops_t *driver_ops)
{
    return xf_hal_driver_register(XF_HAL_DAC_TYPE, XF_HAL_FLAG_ONLY_WRITE, dac_constructor, driver_ops);
}

xf_err_t xf_hal_dac_init(xf_dac_num_t dac_num)
{
    xf_hal_dev_t *dev = xf_hal_driver_create(XF_HAL_DAC_TYPE, dac_num);
    XF_HAL_DAC_CHECK(!dev, XF_ERR_NOT_FOUND, "dac init failed!");

    return XF_OK;
}

xf_err_t xf_hal_dac_deinit(xf_dac_num_t dac_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_DAC_TYPE, dac_num);
    xf_hal_dac_t *dev_dac = (xf_hal_dac_t *)dev;
    XF_HAL_DAC_CHECK(!dev_dac, XF_ERR_UNINIT, "dac is not init!");

    err = xf_hal_driver_close(dev);
    XF_HAL_DAC_CHECK(err, err, "deinit failed!");

    return XF_OK;
}

xf_err_t xf_hal_dac_enable(xf_dac_num_t dac_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_DAC_TYPE, dac_num);
    xf_hal_dac_t *dev_dac = (xf_hal_dac_t *)dev;
    XF_HAL_DAC_CHECK(!dev_dac, XF_ERR_UNINIT, "dac is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_dac->dev.mutex);
#endif

    dev_dac->config.enable = true;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_dac->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_DAC_CMD_ENABLE, &dev_dac->config);
    XF_HAL_DAC_CHECK(err, err, "set resolution failed!");

    return XF_OK;
}

xf_err_t xf_hal_dac_disable(xf_dac_num_t dac_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_DAC_TYPE, dac_num);
    xf_hal_dac_t *dev_dac = (xf_hal_dac_t *)dev;
    XF_HAL_DAC_CHECK(!dev_dac, XF_ERR_UNINIT, "dac is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_dac->dev.mutex);
#endif

    dev_dac->config.enable = false;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_dac->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_DAC_CMD_ENABLE, &dev_dac->config);
    XF_HAL_DAC_CHECK(err, err, "set resolution failed!");

    return XF_OK;
}

xf_err_t xf_hal_dac_set_resolution(xf_dac_num_t dac_num, uint8_t resolution)
{
    XF_HAL_DAC_CHECK(resolution > 32, XF_ERR_INVALID_ARG, "dac resolution must less than 32");

    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_DAC_TYPE, dac_num);
    xf_hal_dac_t *dev_dac = (xf_hal_dac_t *)dev;
    XF_HAL_DAC_CHECK(!dev_dac, XF_ERR_UNINIT, "dac is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_dac->dev.mutex);
#endif

    dev_dac->config.resolution = resolution;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_dac->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_DAC_CMD_RESOLUTION, &dev_dac->config);
    XF_HAL_DAC_CHECK(err, err, "set resolution failed!");

    return XF_OK;
}

xf_err_t xf_hal_dac_set_speed(xf_dac_num_t dac_num, uint32_t speed)
{
    XF_HAL_DAC_CHECK(speed > 50000000, XF_ERR_INVALID_ARG, "dac sample_rate must less than 50000000");

    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_DAC_TYPE, dac_num);
    xf_hal_dac_t *dev_dac = (xf_hal_dac_t *)dev;
    XF_HAL_DAC_CHECK(!dev_dac, XF_ERR_UNINIT, "dac is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_dac->dev.mutex);
#endif

    dev_dac->config.speed = speed;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_dac->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_DAC_CMD_RESOLUTION, &dev_dac->config);
    XF_HAL_DAC_CHECK(err, err, "set resolution failed!");

    return XF_OK;
}

xf_err_t xf_hal_dac_write(xf_dac_num_t dac_num, uint32_t value)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_DAC_TYPE, dac_num);
    xf_hal_dac_t *dev_dac = (xf_hal_dac_t *)dev;
    XF_HAL_DAC_CHECK(!dev_dac, XF_ERR_UNINIT, "dac is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_dac->dev.mutex);
#endif

    XF_ASSERT(value <= dev_dac->config.value_max, XF_ERR_INVALID_ARG, TAG, "value must less than %d", value);

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_dac->dev.mutex);
#endif

    err = xf_hal_driver_write(dev, &value, 1);
    // 此处返回正错误码（-err）即可，无需像其他真正的读写那样返回负值错误码
    XF_HAL_DAC_CHECK(err, -err, "dac write failed!");

    return XF_OK;
}

xf_err_t xf_hal_dac_write_mv(xf_dac_num_t dac_num, uint32_t mv)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    uint32_t value = 0;

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_DAC_TYPE, dac_num);
    xf_hal_dac_t *dev_dac = (xf_hal_dac_t *)dev;
    XF_HAL_DAC_CHECK(!dev_dac, XF_ERR_UNINIT, "dac is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_dac->dev.mutex);
#endif

    XF_HAL_DAC_CHECK(mv > dev_dac->config.verf_mv, XF_ERR_INVALID_ARG, "mv must less than %d", dev_dac->config.verf_mv);

    value = dev_dac->config.value_max * mv / dev_dac->config.verf_mv;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_dac->dev.mutex);
#endif

    err = xf_hal_driver_write(dev, &value, 1);
    // 此处返回正错误码（-err）即可，无需像其他真正的读写那样返回负值错误码
    XF_HAL_DAC_CHECK(err, -err, "dac write failed!");

    return XF_OK;
}

/* ==================== [Static Functions] ================================== */

static xf_hal_dev_t *dac_constructor(xf_dac_num_t dac_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    xf_hal_dev_t *dev = NULL;
    xf_hal_dac_t *dev_dac = (xf_hal_dac_t *)xf_malloc(sizeof(xf_hal_dac_t));
    XF_ASSERT(dev_dac, NULL, TAG, "memory alloc failed!");

    dev = (xf_hal_dev_t *)dev_dac;

    err = xf_hal_driver_open(dev, XF_HAL_DAC_TYPE, dac_num);

    if (err) {
        XF_LOGE(TAG, "open failed!");
        xf_free(dev);
        dev = NULL;
    }

    return dev;
}

#endif


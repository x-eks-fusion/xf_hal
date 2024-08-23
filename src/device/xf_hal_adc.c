/**
 * @file xf_hal_adc.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-13
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_adc.h"

#if XF_HAL_ADC_IS_ENABLE

#include "../kernel/xf_hal_dev.h"

/* ==================== [Defines] =========================================== */

#define TAG "hal_adc"
#define XF_HAL_ADC_TYPE XF_HAL_ADC

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_adc_t {
    xf_hal_dev_t dev; // 一定要放到第一个，以便后续close一起free
    xf_hal_adc_config_t config;
} xf_hal_adc_t;

/* ==================== [Static Prototypes] ================================= */

static xf_hal_dev_t *adc_constructor(xf_adc_num_t adc_num);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

#define XF_HAL_ADC_CHECK(condition, retval,  format, ...) \
    XF_CHECK(condition, retval, TAG, format, ##__VA_ARGS__)

/* ==================== [Global Functions] ================================== */

xf_err_t xf_hal_adc_register(const xf_driver_ops_t *driver_ops)
{
    return xf_hal_driver_register(XF_HAL_ADC_TYPE, XF_HAL_FLAG_ONLY_READ, adc_constructor, driver_ops);
}

xf_err_t xf_hal_adc_init(xf_adc_num_t adc_num)
{
    xf_hal_dev_t *dev = xf_hal_driver_create(XF_HAL_ADC_TYPE, adc_num);
    XF_HAL_ADC_CHECK(!dev, XF_ERR_NOT_FOUND, "adc init failed!");

    return XF_OK;
}

xf_err_t xf_hal_adc_deinit(xf_adc_num_t adc_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_ADC_TYPE, adc_num);
    XF_HAL_ADC_CHECK(!dev, XF_ERR_UNINIT, "adc is not init!");

    err = xf_hal_driver_close(dev);
    XF_HAL_ADC_CHECK(err, err, "adc deinit failed!");

    return XF_OK;
}

xf_err_t xf_hal_adc_enable(xf_adc_num_t adc_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_ADC_TYPE, adc_num);
    xf_hal_adc_t *dev_adc = (xf_hal_adc_t *)dev;
    XF_HAL_ADC_CHECK(!dev_adc, XF_ERR_UNINIT, "adc is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_adc->dev.mutex);
#endif

    dev_adc->config.enable = true;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_adc->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_ADC_CMD_ENABLE, &dev_adc->config);
    XF_HAL_ADC_CHECK(err, err, "enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_adc_disable(xf_adc_num_t adc_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_ADC_TYPE, adc_num);
    xf_hal_adc_t *dev_adc = (xf_hal_adc_t *)dev;
    XF_HAL_ADC_CHECK(!dev_adc, XF_ERR_UNINIT, "adc is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_adc->dev.mutex);
#endif

    dev_adc->config.enable = false;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_adc->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_ADC_CMD_ENABLE, &dev_adc->config);
    XF_HAL_ADC_CHECK(err, err, "disable failed!");

    return XF_OK;
}

xf_err_t xf_hal_adc_set_resolution(xf_adc_num_t adc_num, uint8_t resolution)
{
    XF_HAL_ADC_CHECK(resolution > 32, XF_ERR_INVALID_ARG, "adc resolution must less than 32");

    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_ADC_TYPE, adc_num);
    xf_hal_adc_t *dev_adc = (xf_hal_adc_t *)dev;
    XF_HAL_ADC_CHECK(!dev_adc, XF_ERR_UNINIT, "adc is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_adc->dev.mutex);
#endif

    dev_adc->config.resolution = resolution;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_adc->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_ADC_CMD_RESOLUTION, &dev_adc->config);
    XF_HAL_ADC_CHECK(err, err, "set resolution failed!");

    return XF_OK;
}

xf_err_t xf_hal_adc_set_sample_rate(xf_adc_num_t adc_num, uint32_t sample_rate)
{
    XF_HAL_ADC_CHECK(sample_rate > 10000000, XF_ERR_INVALID_ARG, "adc sample_rate must less than 10000000");

    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_ADC_TYPE, adc_num);
    xf_hal_adc_t *dev_adc = (xf_hal_adc_t *)dev;
    XF_HAL_ADC_CHECK(!dev_adc, XF_ERR_UNINIT, "adc is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_adc->dev.mutex);
#endif

    dev_adc->config.sample_rate = sample_rate;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_adc->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_ADC_CMD_SAMPLE_RATE, &dev_adc->config);
    XF_HAL_ADC_CHECK(err, err, "set samle rate failed!");

    return XF_OK;
}

uint32_t xf_hal_adc_read_raw(xf_adc_num_t adc_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    uint32_t data = 0;

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_ADC_TYPE, adc_num);
    xf_hal_adc_t *dev_adc = (xf_hal_adc_t *)dev;
    XF_HAL_ADC_CHECK(!dev_adc, XF_ERR_UNINIT, "adc is not init!");

    err = xf_hal_driver_read(dev, (void *)&data, 1);
    XF_HAL_ADC_CHECK(err < XF_OK, 0, "adc read failed!:%d!", -err);

    return data;
}

/* ==================== [Static Functions] ================================== */

static xf_hal_dev_t *adc_constructor(xf_adc_num_t adc_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    xf_hal_dev_t *dev = NULL;
    xf_hal_adc_t *dev_adc = (xf_hal_adc_t *)xf_malloc(sizeof(xf_hal_adc_t));
    XF_ASSERT(dev_adc, NULL, TAG, "memory alloc failed!");

    dev = (xf_hal_dev_t *)dev_adc;

    err = xf_hal_driver_open(dev, XF_HAL_ADC_TYPE, adc_num);

    if (err) {
        XF_LOGE(TAG, "open failed!");
        xf_free(dev);
        dev = NULL;
    }

    return dev;
}

#endif

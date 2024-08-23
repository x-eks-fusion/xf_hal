/**
 * @file xf_hal_tim.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_tim.h"

#if XF_HAL_TIM_IS_ENABLE

#include "../kernel/xf_hal_dev.h"

/* ==================== [Defines] =========================================== */

#define TAG "hal_tim"

#define XF_HAL_TIM_TYPE XF_HAL_TIM

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_tim_t {
    xf_hal_dev_t dev; // 一定要放到第一个，以便后续close一起free
    xf_hal_tim_config_t config;
} xf_hal_tim_t;

/* ==================== [Static Prototypes] ================================= */

static xf_hal_dev_t *tim_constructor(xf_tim_num_t tim_num);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

#define XF_HAL_TIM_CHECK(condition, retval,  format, ...) \
    XF_CHECK(condition, retval, TAG, format, ##__VA_ARGS__)

/* ==================== [Global Functions] ================================== */

xf_err_t xf_hal_tim_register(const xf_driver_ops_t *driver_ops)
{
    return xf_hal_driver_register(XF_HAL_TIM_TYPE, XF_HAL_FLAG_READ_WRITE, tim_constructor, driver_ops);
}

xf_err_t xf_hal_tim_init(xf_tim_num_t tim_num, uint32_t tick_freq_hz, xf_hal_tim_count_dir_t count_dir,
                         bool auto_reload)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_driver_create(XF_HAL_TIM_TYPE, tim_num);
    XF_HAL_TIM_CHECK(!dev, XF_ERR_NOT_FOUND, "tim init failed!");

    xf_hal_tim_t *dev_tim = (xf_hal_tim_t *)dev;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_tim->dev.mutex);
#endif

    dev_tim->config.tick_freq_hz = tick_freq_hz;
    dev_tim->config.auto_reload = auto_reload;
    dev_tim->config.count_dir = count_dir;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_tim->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_TIM_CMD_TICK_FREQ_HZ | XF_HAL_TIM_CMD_AUTO_RELOAD | XF_HAL_TIM_CMD_COUNT_DIR,
                              &dev_tim->config);
    XF_HAL_TIM_CHECK(err, err, "tim init failed!");

    return XF_OK;
}

xf_err_t xf_hal_tim_deinit(xf_tim_num_t tim_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_TIM_TYPE, tim_num);
    xf_hal_tim_t *dev_tim = (xf_hal_tim_t *)dev;
    XF_HAL_TIM_CHECK(!dev_tim, XF_ERR_UNINIT, "tim is not init!");

    err = xf_hal_driver_close(dev);
    XF_HAL_TIM_CHECK(err, err, "deinit failed!");

    return XF_OK;
}

xf_err_t xf_hal_tim_set_cb(xf_tim_num_t tim_num, xf_hal_tim_cb_t callback, void *user_data)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_TIM_TYPE, tim_num);
    xf_hal_tim_t *dev_tim = (xf_hal_tim_t *)dev;
    XF_HAL_TIM_CHECK(!dev_tim, XF_ERR_UNINIT, "tim is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_tim->dev.mutex);
#endif

    dev_tim->config.cb.callback = callback;
    dev_tim->config.cb.user_data = user_data;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_tim->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_TIM_CMD_CB, &dev_tim->config);
    XF_HAL_TIM_CHECK(err, err, "callback set failed!");

    return XF_OK;
}

xf_err_t xf_hal_tim_set_isr(xf_tim_num_t tim_num, xf_hal_tim_cb_t callback, void *user_data)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_TIM_TYPE, tim_num);
    xf_hal_tim_t *dev_tim = (xf_hal_tim_t *)dev;
    XF_HAL_TIM_CHECK(!dev_tim, XF_ERR_UNINIT, "tim is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_tim->dev.mutex);
#endif

    dev_tim->config.isr.callback = callback;
    dev_tim->config.isr.user_data = user_data;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_tim->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_TIM_CMD_ISR, &dev_tim->config);
    XF_HAL_TIM_CHECK(err, err, "callback set failed!");

    return XF_OK;
}

xf_err_t xf_hal_tim_start(xf_tim_num_t tim_num, uint32_t target_ticks)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_TIM_TYPE, tim_num);
    xf_hal_tim_t *dev_tim = (xf_hal_tim_t *)dev;
    XF_HAL_TIM_CHECK(!dev_tim, XF_ERR_UNINIT, "tim is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_tim->dev.mutex);
#endif

    dev_tim->config.active = XF_HAL_TIM_ACTIVE_START;
    dev_tim->config.target_ticks = target_ticks;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_tim->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_TIM_CMD_TARGET_TICKS | XF_HAL_TIM_CMD_ACTIVE, &dev_tim->config);

    XF_HAL_TIM_CHECK(err, err, "tim start failed!");

    return XF_OK;
}

xf_err_t xf_hal_tim_stop(xf_tim_num_t tim_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_TIM_TYPE, tim_num);
    xf_hal_tim_t *dev_tim = (xf_hal_tim_t *)dev;
    XF_HAL_TIM_CHECK(!dev_tim, XF_ERR_UNINIT, "tim is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_tim->dev.mutex);
#endif

    dev_tim->config.active = XF_HAL_TIM_ACTIVE_STOP;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_tim->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_TIM_CMD_ACTIVE, &dev_tim->config);
    XF_HAL_TIM_CHECK(err, err, "tim stop failed!");

    return XF_OK;
}

xf_err_t xf_hal_tim_set_raw_ticks(xf_tim_num_t tim_num, uint32_t ticks)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_TIM_TYPE, tim_num);
    xf_hal_tim_t *dev_tim = (xf_hal_tim_t *)dev;

    XF_HAL_TIM_CHECK(!dev_tim, XF_ERR_UNINIT, "tim is not init!");

    err = xf_hal_driver_write(dev, &ticks, 1);
    // 此处返回正错误码（-err）即可，无需像其他真正的读写那样返回负值错误码
    XF_HAL_TIM_CHECK(err < XF_OK, -err, "tim write failed!");

    return err;
}

uint32_t xf_hal_tim_get_raw_ticks(xf_tim_num_t tim_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    uint32_t ticks = 0;

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_TIM_TYPE, tim_num);
    xf_hal_tim_t *dev_tim = (xf_hal_tim_t *)dev;
    XF_HAL_TIM_CHECK(!dev_tim, XF_ERR_UNINIT, "tim is not init!");

    err = xf_hal_driver_read(dev, &ticks, 1);
    XF_HAL_TIM_CHECK(err < XF_OK, 0, "tim read failed!");

    return ticks;
}

/* ==================== [Static Functions] ================================== */

static xf_hal_dev_t *tim_constructor(xf_tim_num_t tim_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    xf_hal_dev_t *dev = NULL;
    xf_hal_tim_t *dev_tim = (xf_hal_tim_t *)xf_malloc(sizeof(xf_hal_tim_t));
    XF_ASSERT(dev_tim, NULL, TAG, "memory alloc failed!");

    dev = (xf_hal_dev_t *)dev_tim;

    err = xf_hal_driver_open(dev, XF_HAL_TIM_TYPE, tim_num);

    if (err) {
        XF_LOGE(TAG, "open failed!");
        xf_free(dev);
        dev = NULL;
    }

    return dev;
}

#endif

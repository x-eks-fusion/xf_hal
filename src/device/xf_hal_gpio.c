/**
 * @file xf_hal_gpio.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-13
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_gpio.h"

#if XF_HAL_GPIO_IS_ENABLE

#include "../kernel/xf_hal_dev.h"

/* ==================== [Defines] =========================================== */

#define TAG "hal_gpio"
#define XF_HAL_GPIO_TYPE XF_HAL_GPIO

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_gpio_t {
    xf_hal_dev_t dev; // 一定要放到第一个，以便后续close一起free
    xf_hal_gpio_config_t config;
} xf_hal_gpio_t;

/* ==================== [Static Prototypes] ================================= */

static xf_hal_dev_t *gpio_constructor(xf_gpio_num_t gpio_num);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

#define XF_HAL_GPIO_CHECK(condition, retval,  format, ...) \
    XF_CHECK(condition, retval, TAG, format, ##__VA_ARGS__)

/* ==================== [Global Functions] ================================== */

xf_err_t xf_hal_gpio_register(const xf_driver_ops_t *driver_ops)
{
    return xf_hal_driver_register(XF_HAL_GPIO_TYPE, XF_HAL_FLAG_READ_WRITE, gpio_constructor, driver_ops);
}

xf_err_t xf_hal_gpio_init(xf_gpio_num_t gpio_num, xf_hal_gpio_dir_t direction)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_driver_create(XF_HAL_GPIO_TYPE, gpio_num);
    XF_HAL_GPIO_CHECK(!dev, XF_ERR_NOT_FOUND, "gpio init failed!");

    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)dev;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_gpio->dev.mutex);
#endif

    dev_gpio->config.direction = direction;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_gpio->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_GPIO_CMD_DIRECTION, &dev_gpio->config);
    XF_HAL_GPIO_CHECK(err, err, "enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_deinit(xf_gpio_num_t gpio_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    XF_HAL_GPIO_CHECK(!dev, XF_ERR_UNINIT, "gpio is not init!");

    err = xf_hal_driver_close(dev);
    XF_HAL_GPIO_CHECK(err, err, "deinit failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_set_direction(xf_gpio_num_t gpio_num, xf_hal_gpio_dir_t direction)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)dev;
    XF_HAL_GPIO_CHECK(!dev_gpio, XF_ERR_UNINIT, "gpio is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_gpio->dev.mutex);
#endif

    dev_gpio->config.direction = direction;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_gpio->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_GPIO_CMD_DIRECTION, &dev_gpio->config);
    XF_HAL_GPIO_CHECK(err, err, "set direction failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_set_pull(xf_gpio_num_t gpio_num, xf_hal_gpio_pull_t pull)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)dev;
    XF_HAL_GPIO_CHECK(!dev_gpio, XF_ERR_UNINIT, "gpio is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_gpio->dev.mutex);
#endif

    dev_gpio->config.pull = pull;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_gpio->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_GPIO_CMD_PULL, &dev_gpio->config);
    XF_HAL_GPIO_CHECK(err, err, "set pull failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_set_speed(xf_gpio_num_t gpio_num, uint32_t speed)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)dev;
    XF_HAL_GPIO_CHECK(!dev_gpio, XF_ERR_UNINIT, "gpio is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_gpio->dev.mutex);
#endif

    dev_gpio->config.speed = speed;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_gpio->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_GPIO_CMD_SPEED, &dev_gpio->config);
    XF_HAL_GPIO_CHECK(err, err, "set speed failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_set_intr_cb(xf_gpio_num_t gpio_num, xf_hal_gpio_irq_cb_t callback, void *user_data)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)dev;
    XF_HAL_GPIO_CHECK(!dev_gpio, XF_ERR_UNINIT, "gpio is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_gpio->dev.mutex);
#endif

    dev_gpio->config.cb.callback = callback;
    dev_gpio->config.cb.user_data = user_data;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_gpio->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_GPIO_CMD_INTR_CB, &dev_gpio->config);
    XF_HAL_GPIO_CHECK(err, err, "set callback failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_set_intr_isr(xf_gpio_num_t gpio_num, xf_hal_gpio_irq_cb_t callback, void *user_data)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)dev;
    XF_HAL_GPIO_CHECK(!dev_gpio, XF_ERR_UNINIT, "gpio is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_gpio->dev.mutex);
#endif

    dev_gpio->config.isr.callback = callback;
    dev_gpio->config.isr.user_data = user_data;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_gpio->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_GPIO_CMD_INTR_ISR, &dev_gpio->config);
    XF_HAL_GPIO_CHECK(err, err, "set isr failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_set_intr_type(xf_gpio_num_t gpio_num, xf_hal_gpio_intr_type_t intr_type)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)dev;
    XF_HAL_GPIO_CHECK(!dev_gpio, XF_ERR_UNINIT, "gpio is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_gpio->dev.mutex);
#endif

    dev_gpio->config.intr_type = intr_type;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_gpio->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_GPIO_CMD_INTR_TYPE, &dev_gpio->config);
    XF_HAL_GPIO_CHECK(err, err, "set intr type failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_set_intr_enable(xf_gpio_num_t gpio_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)dev;
    XF_HAL_GPIO_CHECK(!dev_gpio, XF_ERR_UNINIT, "gpio is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_gpio->dev.mutex);
#endif

    dev_gpio->config.intr_enable = true;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_gpio->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_GPIO_CMD_INTR_ENABLE, &dev_gpio->config);
    XF_HAL_GPIO_CHECK(err, err, "irq enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_set_intr_disable(xf_gpio_num_t gpio_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)dev;
    XF_HAL_GPIO_CHECK(!dev_gpio, XF_ERR_UNINIT, "gpio is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_gpio->dev.mutex);
#endif

    dev_gpio->config.intr_enable = false;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_gpio->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_GPIO_CMD_INTR_ENABLE, &dev_gpio->config);
    XF_HAL_GPIO_CHECK(err, err, "irq disable failed!");

    return XF_OK;
}

xf_err_t xf_hal_gpio_set_level(xf_gpio_num_t gpio_num, bool level)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    XF_HAL_GPIO_CHECK(!dev, XF_ERR_UNINIT, "gpio is not init!");

    err = xf_hal_driver_write(dev, &level, 1);
    // 此处返回正错误码（-err）即可，无需像其他真正的读写那样返回负值错误码
    XF_HAL_GPIO_CHECK(err < XF_OK, -err, "gpio write failed!");

    return err;
}

bool xf_hal_gpio_get_level(xf_gpio_num_t gpio_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    bool level = 0;

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_GPIO_TYPE, gpio_num);
    XF_HAL_GPIO_CHECK(!dev, XF_ERR_UNINIT, "gpio is not init!");

    err = xf_hal_driver_read(dev, &level, 1);
    XF_HAL_GPIO_CHECK(err < XF_OK, 0, "gpio read failed!");

    return level;
}
/* ==================== [Static Functions] ================================== */

static xf_hal_dev_t *gpio_constructor(xf_gpio_num_t gpio_num)
{
    xf_err_t err = XF_OK;
    xf_hal_dev_t *dev = NULL;
    xf_hal_gpio_t *dev_gpio = (xf_hal_gpio_t *)xf_malloc(sizeof(xf_hal_gpio_t));
    XF_ASSERT(dev_gpio, NULL, TAG, "memory alloc failed!");

    dev = (xf_hal_dev_t *)dev_gpio;

    err = xf_hal_driver_open(dev, XF_HAL_GPIO_TYPE, gpio_num);

    if (err) {
        XF_LOGE(TAG, "open failed!");
        xf_free(dev);
        dev = NULL;
    }

    return dev;
}

#endif


/**
 * @file xf_hal_spi.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-17
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_spi.h"

#if XF_HAL_SPI_IS_ENABLE

#include "../kernel/xf_hal_dev.h"

/* ==================== [Defines] =========================================== */

#define TAG "hal_spi"
#define XF_HAL_SPI_TYPE XF_HAL_SPI

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_spi_t {
    xf_hal_dev_t dev; // 一定要放到第一个，以便后续close一起free
    xf_hal_spi_config_t config;
} xf_hal_spi_t;

/* ==================== [Static Prototypes] ================================= */

static xf_hal_dev_t *spi_constructor(xf_spi_num_t spi_num);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */


#define XF_HAL_SPI_CHECK(condition, retval,  format, ...) \
    XF_CHECK(condition, retval, TAG, format, ##__VA_ARGS__)

/* ==================== [Global Functions] ================================== */

xf_err_t xf_hal_spi_register(const xf_driver_ops_t *driver_ops)
{
    return xf_hal_driver_register(XF_HAL_SPI_TYPE, XF_HAL_FLAG_READ_WRITE, spi_constructor, driver_ops);
}

xf_err_t xf_hal_spi_init(xf_spi_num_t spi_num, xf_hal_spi_hosts_t hosts, uint32_t speed)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_driver_create(XF_HAL_SPI_TYPE, spi_num);
    XF_HAL_SPI_CHECK(!dev, XF_ERR_NOT_FOUND, "spi init failed!");

    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.hosts = hosts;
    dev_spi->config.speed = speed;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_HOSTS | XF_HAL_SPI_CMD_SPEED, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_deinit(xf_spi_num_t spi_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

    err = xf_hal_driver_close(dev);
    XF_HAL_SPI_CHECK(err, err, "deinit failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_enable(xf_spi_num_t spi_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.enable = true;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_ENABLE, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "spi set enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_disable(xf_spi_num_t spi_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.enable = false;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_ENABLE, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "spi set enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_set_prev_cb(xf_spi_num_t spi_num, xf_hal_spi_cb_t callback, void *user_data)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.prev_cb.callback = callback;
    dev_spi->config.prev_cb.user_data = user_data;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_PREV_CB, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "spi set enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_set_post_cb(xf_spi_num_t spi_num, xf_hal_spi_cb_t callback, void *user_data)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.post_cb.callback = callback;
    dev_spi->config.post_cb.user_data = user_data;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_POST_CB, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "spi set enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_set_gpio(xf_spi_num_t spi_num, const xf_hal_spi_gpio_t *spi_gpio)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.gpio = *spi_gpio;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_GPIO, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "spi set gpio failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_set_bit_order(xf_spi_num_t spi_num, xf_hal_spi_bit_order_t bit_order)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.bit_order = bit_order;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_BIT_ORDER, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "spi set bit order failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_set_mode(xf_spi_num_t spi_num, xf_hal_spi_mode_t mode)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.mode = mode;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_MODE, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "spi set mode failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_set_data_width(xf_spi_num_t spi_num, xf_hal_spi_data_width_t data_width)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.data_width = data_width;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_DATA_WIDTH, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "spi set data width failed!");

    return XF_OK;
}

xf_err_t xf_hal_spi_set_speed(xf_spi_num_t spi_num, uint32_t speed)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

    dev_spi->config.speed = speed;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_SPEED, &dev_spi->config);
    XF_HAL_SPI_CHECK(err, err, "spi set speed failed!");

    return XF_OK;
}

int xf_hal_spi_write(xf_spi_num_t spi_num, const uint8_t *buffer, uint32_t size, uint32_t timeout_ms)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

    if (dev_spi->config.timeout_ms != timeout_ms) {

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_lock(dev_spi->dev.mutex);
#endif

        dev_spi->config.timeout_ms = timeout_ms;

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_unlock(dev_spi->dev.mutex);
#endif

        err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_TIMEOUT, &dev_spi->config);
        XF_HAL_SPI_CHECK(err, err, "set timeout_ms failed!");
    }

    err = xf_hal_driver_write(dev, buffer, size);
    XF_HAL_SPI_CHECK(err < XF_OK, err,  "spi write failed!:%d!", -err);

    return err;
}

int xf_hal_spi_read(xf_spi_num_t spi_num, uint8_t *buffer, uint32_t size, uint32_t timeout_ms)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_SPI_TYPE, spi_num);
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)dev;
    XF_HAL_SPI_CHECK(!dev_spi, XF_ERR_UNINIT, "spi is not init!");

    if (dev_spi->config.timeout_ms != timeout_ms) {
#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_spi->dev.mutex);
#endif

        dev_spi->config.timeout_ms = timeout_ms;
        err = xf_hal_driver_ioctl(dev, XF_HAL_SPI_CMD_TIMEOUT, &dev_spi->config);
        XF_HAL_SPI_CHECK(err, err, "set timeout_ms failed!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_spi->dev.mutex);
#endif
    }

    err = xf_hal_driver_read(dev, buffer, size);
    XF_HAL_SPI_CHECK(err < XF_OK, err,  "spi read failed!:%d!", -err);

    return err;
}

/* ==================== [Static Functions] ================================== */

static xf_hal_dev_t *spi_constructor(xf_spi_num_t spi_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    xf_hal_dev_t *dev = NULL;
    xf_hal_spi_t *dev_spi = (xf_hal_spi_t *)xf_malloc(sizeof(xf_hal_spi_t));
    XF_ASSERT(dev_spi, NULL, TAG, "memory alloc failed!");

    dev = (xf_hal_dev_t *)dev_spi;

    err = xf_hal_driver_open(dev, XF_HAL_SPI_TYPE, spi_num);

    if (err) {
        XF_LOGE(TAG, "open failed!");
        xf_free(dev);
        dev = NULL;
    }

    return dev;
}

#endif

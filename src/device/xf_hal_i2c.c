/**
 * @file xf_hal_i2c.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-16
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_i2c.h"

#if XF_HAL_I2C_IS_ENABLE

#include "../kernel/xf_hal_dev.h"

/* ==================== [Defines] =========================================== */

#define TAG "hal_i2c"
#define XF_HAL_I2C_TYPE XF_HAL_I2C

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_i2c_t {
    xf_hal_dev_t dev; // 一定要放到第一个，以便后续close一起free
    xf_hal_i2c_config_t config;
} xf_hal_i2c_t;

/* ==================== [Static Prototypes] ================================= */

static xf_hal_dev_t *i2c_constructor(xf_i2c_num_t i2c_num);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

#define XF_HAL_I2C_CHECK(condition, retval,  format, ...) \
    XF_CHECK(condition, retval, TAG, format, ##__VA_ARGS__)


/* ==================== [Global Functions] ================================== */

xf_err_t xf_hal_i2c_register(const xf_driver_ops_t *driver_ops)
{
    return xf_hal_driver_register(XF_HAL_I2C_TYPE, XF_HAL_FLAG_READ_WRITE, i2c_constructor, driver_ops);
}

xf_err_t xf_hal_i2c_init(xf_i2c_num_t i2c_num, xf_hal_i2c_hosts_t hosts, uint32_t speed)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_driver_create(XF_HAL_I2C_TYPE, i2c_num);
    XF_HAL_I2C_CHECK(!dev, XF_ERR_NOT_FOUND, "i2c init failed!");

    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_i2c->dev.mutex);
#endif

    dev_i2c->config.hosts = hosts;
    dev_i2c->config.speed = speed;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_i2c->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_HOSTS | XF_HAL_I2C_CMD_SPEED, &dev_i2c->config);
    XF_HAL_I2C_CHECK(err, err, "enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_i2c_deinit(xf_i2c_num_t i2c_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_NOT_FOUND, "i2c is not init!");

    err = xf_hal_driver_close(dev);
    XF_HAL_I2C_CHECK(err, err, "deinit failed!");

    return XF_OK;
}

xf_err_t xf_hal_i2c_enable(xf_i2c_num_t i2c_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_i2c->dev.mutex);
#endif

    dev_i2c->config.enable = true;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_i2c->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_ENABLE, &dev_i2c->config);
    XF_HAL_I2C_CHECK(err, err, "i2c enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_i2c_disable(xf_i2c_num_t i2c_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_i2c->dev.mutex);
#endif

    dev_i2c->config.enable = false;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_i2c->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_ENABLE, &dev_i2c->config);
    XF_HAL_I2C_CHECK(err, err, "i2c disable failed!");

    return XF_OK;
}

xf_err_t xf_hal_i2c_set_gpio(xf_i2c_num_t i2c_num, xf_gpio_num_t scl_num, xf_gpio_num_t sda_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_i2c->dev.mutex);
#endif

    dev_i2c->config.scl_num = scl_num;
    dev_i2c->config.sda_num = sda_num;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_i2c->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_SCL_NUM | XF_HAL_I2C_CMD_SDA_NUM , &dev_i2c->config);
    XF_HAL_I2C_CHECK(err, err, "i2c set gpio failed!");

    return XF_OK;

}

xf_err_t xf_hal_i2c_set_address_width(xf_i2c_num_t i2c_num, xf_hal_i2c_address_width_t address_width)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_i2c->dev.mutex);
#endif

    dev_i2c->config.address_width = address_width;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_i2c->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_ADDRESS_WIDTH, &dev_i2c->config);
    XF_HAL_I2C_CHECK(err, err, "set address failed!");

    return XF_OK;
}

xf_err_t xf_hal_i2c_set_address(xf_i2c_num_t i2c_num, uint16_t address)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_i2c->dev.mutex);
#endif

    dev_i2c->config.address = address;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_i2c->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_ADDRESS, &dev_i2c->config);
    XF_HAL_I2C_CHECK(err, err, "set address failed!");

    return XF_OK;
}

xf_err_t xf_hal_i2c_set_mem_addr_width(xf_i2c_num_t i2c_num, xf_hal_i2c_mem_addr_width_t mem_addr_width)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_i2c->dev.mutex);
#endif

    dev_i2c->config.mem_addr_width = mem_addr_width;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_i2c->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_MEM_ADDR_WIDTH, &dev_i2c->config);
    XF_HAL_I2C_CHECK(err, err, "set address failed!");

    return XF_OK;
}

int xf_hal_i2c_write_mem(xf_i2c_num_t i2c_num, uint32_t mem_addr, const uint8_t *buffer, uint32_t size,
                              uint32_t timeout_ms)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

    if (dev_i2c->config.mem_addr != mem_addr || dev_i2c->config.mem_addr_en != true || dev_i2c->config.timeout_ms != timeout_ms) {

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_lock(dev_i2c->dev.mutex);
#endif

        dev_i2c->config.mem_addr = mem_addr;
        dev_i2c->config.mem_addr_en = true;
        dev_i2c->config.timeout_ms = timeout_ms;

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_unlock(dev_i2c->dev.mutex);
#endif

        err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_MEM_ADDR | XF_HAL_I2C_CMD_MEM_ADDR_EN | XF_HAL_I2C_CMD_TIMEOUT, &dev_i2c->config);
        XF_HAL_I2C_CHECK(err, err, "write memory address failed!");
    }

    err = xf_hal_driver_write(dev, buffer, size);
    XF_HAL_I2C_CHECK(err < XF_OK, err, "write memory address failed!:%d!", -err);

    return err;
}

int xf_hal_i2c_read_mem(xf_i2c_num_t i2c_num, uint32_t mem_addr, uint8_t *buffer, uint32_t size, uint32_t timeout_ms)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

    if (dev_i2c->config.mem_addr != mem_addr || dev_i2c->config.mem_addr_en != true || dev_i2c->config.timeout_ms != timeout_ms) {

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_lock(dev_i2c->dev.mutex);
#endif

        dev_i2c->config.mem_addr = mem_addr;
        dev_i2c->config.mem_addr_en = true;
        dev_i2c->config.timeout_ms = timeout_ms;

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_unlock(dev_i2c->dev.mutex);
#endif

        err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_MEM_ADDR | XF_HAL_I2C_CMD_MEM_ADDR_EN | XF_HAL_I2C_CMD_TIMEOUT, &dev_i2c->config);
        XF_HAL_I2C_CHECK(err, err, "read memory address failed!");
    }

    err = xf_hal_driver_read(dev, buffer, size);
    XF_HAL_I2C_CHECK(err < XF_OK, err, "read memory address failed!:%d!", -err);

    return err;
}

int xf_hal_i2c_write(xf_i2c_num_t i2c_num, const uint8_t *buffer, uint32_t size, uint32_t timeout_ms)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

    if (dev_i2c->config.mem_addr_en != false || dev_i2c->config.timeout_ms != timeout_ms) {

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_lock(dev_i2c->dev.mutex);
#endif

        dev_i2c->config.timeout_ms = timeout_ms;
        dev_i2c->config.mem_addr_en = false;

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_unlock(dev_i2c->dev.mutex);
#endif

        err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_MEM_ADDR_EN | XF_HAL_I2C_CMD_TIMEOUT, &dev_i2c->config);
        XF_HAL_I2C_CHECK(err, err, "write address disable failed!");
    }

    err = xf_hal_driver_write(dev, buffer, size);
    XF_HAL_I2C_CHECK(err < XF_OK, err, "write address failed!:%d!", -err);

    return err;
}

int xf_hal_i2c_read(xf_i2c_num_t i2c_num, uint8_t *buffer, uint32_t size, uint32_t timeout_ms)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_I2C_TYPE, i2c_num);
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)dev;
    XF_HAL_I2C_CHECK(!dev_i2c, XF_ERR_UNINIT, "i2c is not init!");

    if (dev_i2c->config.mem_addr_en != false || dev_i2c->config.timeout_ms != timeout_ms) {

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_lock(dev_i2c->dev.mutex);
#endif

        dev_i2c->config.timeout_ms = timeout_ms;
        dev_i2c->config.mem_addr_en = false;

#if XF_HAL_LOCK_IS_ENABLE
        xf_lock_unlock(dev_i2c->dev.mutex);
#endif
        err = xf_hal_driver_ioctl(dev, XF_HAL_I2C_CMD_MEM_ADDR_EN | XF_HAL_I2C_CMD_TIMEOUT, &dev_i2c->config);
        XF_HAL_I2C_CHECK(err, err, "read address disable failed!");
    }

    err = xf_hal_driver_read(dev, buffer, size);
    XF_HAL_I2C_CHECK(err < XF_OK, err, "read address failed!:%d!", -err);

    return err;
}

/* ==================== [Static Functions] ================================== */

static xf_hal_dev_t *i2c_constructor(xf_i2c_num_t i2c_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    xf_hal_dev_t *dev = NULL;
    xf_hal_i2c_t *dev_i2c = (xf_hal_i2c_t *)xf_malloc(sizeof(xf_hal_i2c_t));
    XF_ASSERT(dev_i2c, NULL, TAG, "memory alloc failed!");

    dev = (xf_hal_dev_t *)dev_i2c;

    err = xf_hal_driver_open(dev, XF_HAL_I2C_TYPE, i2c_num);

    if (err) {
        XF_LOGE(TAG, "open failed!");
        xf_free(dev);
        dev = NULL;
    }

    return dev;
}

#endif

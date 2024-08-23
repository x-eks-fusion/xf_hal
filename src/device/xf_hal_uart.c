/**
 * @file xf_hal_uart.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-15
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal_uart.h"

#if XF_HAL_UART_IS_ENABLE

#include "../kernel/xf_hal_dev.h"

/* ==================== [Defines] =========================================== */

#define TAG "hal_uart"
#define XF_HAL_UART_TYPE XF_HAL_UART

/* ==================== [Typedefs] ========================================== */

typedef struct _xf_hal_uart_t {
    xf_hal_dev_t dev; // 一定要放到第一个，以便后续close一起free
    xf_hal_uart_config_t config;
} xf_hal_uart_t;


/* ==================== [Static Prototypes] ================================= */

static xf_hal_dev_t *uart_constructor(xf_uart_num_t uart_num);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

#define XF_HAL_UART_CHECK(condition, retval,  format, ...) \
    XF_CHECK(condition, retval, TAG, format, ##__VA_ARGS__)

/* ==================== [Global Functions] ================================== */

xf_err_t xf_hal_uart_register(const xf_driver_ops_t *driver_ops)
{
    return xf_hal_driver_register(XF_HAL_UART_TYPE, XF_HAL_FLAG_READ_WRITE, uart_constructor, driver_ops);
}

xf_err_t xf_hal_uart_init(xf_uart_num_t uart_num, uint32_t baudrate)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_driver_create(XF_HAL_UART_TYPE, uart_num);
    XF_HAL_UART_CHECK(!dev, -1, "uart init failed!");

    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_uart->dev.mutex);
#endif

    dev_uart->config.baudrate = baudrate;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_uart->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_UART_CMD_BAUDRATE, &dev_uart->config);
    XF_HAL_UART_CHECK(err, err, "init failed!");

    return XF_OK;
}

xf_err_t xf_hal_uart_deinit(xf_uart_num_t uart_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_NOT_FOUND, "uart is not init!");

    err = xf_hal_driver_close(dev);
    XF_HAL_UART_CHECK(err, err, "deinit failed!");

    return XF_OK;
}

xf_err_t xf_hal_uart_enable(xf_uart_num_t uart_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_UNINIT, "uart is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_uart->dev.mutex);
#endif

    dev_uart->config.enable = true;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_uart->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_UART_CMD_ENABLE, &dev_uart->config);
    XF_HAL_UART_CHECK(err, err, "enable failed!");

    return XF_OK;
}

xf_err_t xf_hal_uart_disable(xf_uart_num_t uart_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_UNINIT, "uart is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_uart->dev.mutex);
#endif

    dev_uart->config.enable = false;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_uart->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_UART_CMD_ENABLE, &dev_uart->config);
    XF_HAL_UART_CHECK(err, err, "disable failed!");

    return XF_OK;
}

xf_err_t xf_hal_uart_set_gpio(xf_uart_num_t uart_num, xf_gpio_num_t tx_num,
                              xf_gpio_num_t rx_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_UNINIT, "uart is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_uart->dev.mutex);
#endif

    dev_uart->config.tx_num = tx_num;
    dev_uart->config.rx_num = rx_num;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_uart->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_UART_CMD_TX_NUM | XF_HAL_UART_CMD_RX_NUM, &dev_uart->config);
    XF_HAL_UART_CHECK(err, err, "set gpio failed!");

    return XF_OK;
}

xf_err_t xf_hal_uart_set_config(xf_uart_num_t uart_num, xf_hal_uart_data_bits_t data_bits,
                             xf_hal_uart_stop_bits_t stop_bits, xf_hal_uart_parity_bits_t parity_bits)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_UNINIT, "uart is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_uart->dev.mutex);
#endif

    dev_uart->config.data_bits = data_bits;
    dev_uart->config.stop_bits = stop_bits;
    dev_uart->config.parity_bits = parity_bits;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_uart->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_UART_CMD_DATA_BITS | XF_HAL_UART_CMD_STOP_BITS | XF_HAL_UART_CMD_PARITY_BITS,
                              &dev_uart->config);
    XF_HAL_UART_CHECK(err, err, "setting failed!");

    return XF_OK;
}

xf_err_t xf_hal_uart_set_baudrate(xf_uart_num_t uart_num, uint32_t baudrate)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_UNINIT, "uart is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_uart->dev.mutex);
#endif

    dev_uart->config.baudrate = baudrate;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_uart->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_UART_CMD_BAUDRATE, &dev_uart->config);
    XF_HAL_UART_CHECK(err, err, "uart baudrate failed!");

    return XF_OK;
}

uint32_t xf_hal_uart_get_baudrate(xf_uart_num_t uart_num)
{
    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_UNINIT, "uart is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_uart->dev.mutex);
#endif

    uint32_t baudrate = dev_uart->config.baudrate;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_uart->dev.mutex);
#endif

    return baudrate;
}

xf_err_t xf_hal_uart_set_flow_control(xf_uart_num_t uart_num, xf_hal_uart_flow_control_t flow_control,
                                  xf_gpio_num_t rts_num, xf_gpio_num_t cts_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_UNINIT, "uart is not init!");

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_lock(dev_uart->dev.mutex);
#endif

    dev_uart->config.flow_control = flow_control;
    dev_uart->config.rts_num = rts_num;
    dev_uart->config.cts_num = cts_num;

#if XF_HAL_LOCK_IS_ENABLE
    xf_lock_unlock(dev_uart->dev.mutex);
#endif

    err = xf_hal_driver_ioctl(dev, XF_HAL_UART_CMD_FLOW_CONTROL | XF_HAL_UART_CMD_RTS_NUM | XF_HAL_UART_CMD_CTS_NUM, &dev_uart->config);
    XF_HAL_UART_CHECK(err, err, "uart baudrate failed!");

    return XF_OK;
}

int xf_hal_uart_read(xf_uart_num_t uart_num, uint8_t *data, uint32_t data_len)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_UNINIT, "uart is not init!");

    err = xf_hal_driver_read(dev, data, data_len);
    XF_HAL_UART_CHECK(err < XF_OK, err, "uart read failed!:%d!", -err);

    return err;
}

int xf_hal_uart_write(xf_uart_num_t uart_num, const uint8_t *data, uint32_t data_len)
{
    xf_err_t err = XF_OK;
    UNUSED(err);

    xf_hal_dev_t *dev = xf_hal_device_find(XF_HAL_UART_TYPE, uart_num);
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)dev;
    XF_HAL_UART_CHECK(!dev_uart, XF_ERR_UNINIT, "uart is not init!");

    err = xf_hal_driver_write(dev, data, data_len);
    XF_HAL_UART_CHECK(err < XF_OK, err, "uart write failed!:%d!", -err);

    return err;
}

/* ==================== [Static Functions] ================================== */
static xf_hal_dev_t *uart_constructor(xf_uart_num_t uart_num)
{
    xf_err_t err = XF_OK;
    UNUSED(err);
    xf_hal_dev_t *dev = NULL;
    xf_hal_uart_t *dev_uart = (xf_hal_uart_t *)xf_malloc(sizeof(xf_hal_uart_t));
    XF_ASSERT(dev_uart, NULL, TAG, "memory alloc failed!");

    dev = (xf_hal_dev_t *)dev_uart;

    err = xf_hal_driver_open(dev, XF_HAL_UART_TYPE, uart_num);

    if (err) {
        XF_LOGE(TAG, "open failed!");
        xf_free(dev);
        dev = NULL;
    }

    return dev;
}

#endif

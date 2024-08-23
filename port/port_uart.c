/**
 * @file port_uart.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-15
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

#define XF_HAL_UART_DEFAULT_DATA_BITS       XF_HAL_UART_DATA_BIT_8
#define XF_HAL_UART_DEFAULT_STOP_BITS       XF_HAL_UART_STOP_BIT_1
#define XF_HAL_UART_DEFAULT_PARITY_BITS     XF_HAL_UART_PARITY_BITS_NONE
#define XF_HAL_UART_DEFAULT_FLOW_CONTROL    XF_HAL_UART_FLOW_CONTROL_NONE
#define XF_HAL_UART_DEFAULT_BAUDRATE        115200
#define XF_HAL_UART_DEFAULT_TX_NUM          3
#define XF_HAL_UART_DEFAULT_RX_NUM          4
#define XF_HAL_UART_DEFAULT_RTS_NUM         XF_HAL_GPIO_NUM_NONE
#define XF_HAL_UART_DEFAULT_RTS_NUM         XF_HAL_GPIO_NUM_NONE

/* ==================== [Typedefs] ========================================== */

typedef struct _port_uart_t {
    uint32_t port;
    const char *config1;
    const char *config2;
} port_uart_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static int port_uart_open(xf_hal_dev_t *dev);
static int port_uart_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_uart_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_uart_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static int port_uart_close(xf_hal_dev_t *dev);

// 用户实现id的转换方式
static uint32_t _uart_id_to_port(uint32_t id);

// 底层sdk的操作函数
static void _uart_init(uint32_t uart_port, uint32_t tx_gpip_port,
                       uint32_t rx_gpip_port, uint32_t baudrate);
static void _uart_deinit(uint32_t uart_port);
static void _uart_set_data_bits(uint32_t uart_port, uint8_t data_bits);
static void _uart_set_stop_bits(uint32_t uart_port, uint8_t stop_bits);
static void _uart_set_parity_bits(uint32_t uart_port, uint8_t parity_bits);
static void _uart_set_flow_control(uint32_t uart_port,
                                   uint8_t flow_control);
static void _uart_set_flow_control_gpio(uint32_t uart_port, uint32_t rts_num,
                                        uint32_t cts_num);
static void _uart_read(uint32_t uart_port, uint8_t *buffer, uint32_t count);
static void _uart_write(uint32_t uart_port, const uint8_t *buffer, uint32_t count);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_hal_UART_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_uart_open,
        .ioctl = port_uart_ioctl,
        .write = port_uart_write,
        .read = port_uart_read,
        .close = port_uart_close,
    };
    xf_hal_uart_register(&ops);
}

/* ==================== [Static Functions] ================================== */

static int port_uart_open(xf_hal_dev_t *dev)
{
    port_uart_t *uart = (port_uart_t *)malloc(sizeof(port_uart_t));
    if (uart == NULL) {
        return -1;
    }

    uart->port = _uart_id_to_port(dev->id);
    uart->config1 = "这里用于保存一些上层用不到的参数";
    uart->config2 = "我也不知道写啥，随便写写咯";

    dev->platform_data = uart;

    return 0;
}

static int port_uart_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_uart_config_t *uart_config = (xf_hal_uart_config_t *)config;
    port_uart_t *uart = (port_uart_t *)dev->platform_data;

    if (cmd & XF_HAL_UART_CMD_DATA_BITS) {
        _uart_set_data_bits(uart->port, uart_config->data_bits);
    }

    if (cmd & XF_HAL_UART_CMD_STOP_BITS) {
        _uart_set_stop_bits(uart->port, uart_config->stop_bits);
    }

    if (cmd & XF_HAL_UART_CMD_PARITY_BITS) {
        _uart_set_parity_bits(uart->port, uart_config->parity_bits);
    }

    if (cmd & XF_HAL_UART_CMD_FLOW_CONTROL) {
        _uart_set_flow_control(uart->port, uart_config->flow_control);
    }

    if (cmd & XF_HAL_UART_CMD_BAUDRATE || cmd & XF_HAL_UART_CMD_TX_NUM
            || cmd & XF_HAL_UART_CMD_RX_NUM) {
        _uart_init(uart->port, uart_config->tx_num, uart_config->rx_num,
                   uart_config->baudrate);
    }

    if (cmd & XF_HAL_UART_CMD_RTS_NUM || cmd & XF_HAL_UART_CMD_CTS_NUM) {
        _uart_set_flow_control_gpio(uart->port, uart_config->rts_num,
                                    uart_config->cts_num);
    }

    return 0;
}

static int port_uart_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_uart_t *uart = (port_uart_t *)dev->platform_data;
    _uart_read(uart->port, buf, count);
    return 0;
}

static int port_uart_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_uart_t *uart = (port_uart_t *)dev->platform_data;
    _uart_write(uart->port, buf, count);
    return 0;
}

static int port_uart_close(xf_hal_dev_t *dev)
{
    port_uart_t *uart = (port_uart_t *)dev->platform_data;
    _uart_deinit(uart->port);
    free(uart);
    return 0;
}

static uint32_t _uart_id_to_port(uint32_t id)
{
    return id;
}

static void _uart_init(uint32_t uart_port, uint32_t tx_gpip_port,
                       uint32_t rx_gpip_port, uint32_t baudrate)
{
    printf("\nuart tx_gpip_port:%d!\n", tx_gpip_port);
    printf("\nuart rx_gpip_port:%d!\n", rx_gpip_port);
    printf("\nuart baudrate:%d!\n", baudrate);
    printf("\nuart init!\n");
}

static void _uart_deinit(uint32_t uart_port)
{
    printf("\nuart deinit!\n");
}

static void _uart_set_data_bits(uint32_t uart_port, uint8_t data_bits)
{
    printf("\nuart data_bits:%d!\n", data_bits);
}

static void _uart_set_stop_bits(uint32_t uart_port, uint8_t stop_bits)
{
    printf("\nuart stop_bits:%d!\n", stop_bits);
}

static void _uart_set_parity_bits(uint32_t uart_port, uint8_t parity_bits)
{
    printf("\nuart parity_bits:%d!\n", parity_bits);
}

static void _uart_set_flow_control(uint32_t uart_port,
                                   uint8_t flow_control)
{
    printf("\nuart flow_control:%d!\n", flow_control);
}

static void _uart_set_flow_control_gpio(uint32_t uart_port, uint32_t rts_num,
                                        uint32_t cts_num)
{
    printf("\nuart rts_num:%d!\n", rts_num);
    printf("\nuart cts_num:%d!\n", cts_num);
}

static void _uart_read(uint32_t uart_port, uint8_t *buffer, uint32_t count)
{
    strcpy((char *)buffer, "read buffer");
}

static void _uart_write(uint32_t uart_port, const uint8_t *buffer, uint32_t count)
{
    for (int i = 0; i < count; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

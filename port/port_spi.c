/**
 * @file port_spi.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-17
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

#define XF_HAL_SPI_DEFAULT_HOSTS            1
#define XF_HAL_SPI_DEFAULT_BIT_ORDER        XF_HAL_SPI_BIT_ORDER_MSB_FIRST
#define XF_HAL_SPI_DEFAULT_MODE             XF_HAL_SPI_HOSTS_MASTER
#define XF_HAL_SPI_DEFAULT_DATA_WIDTH       XF_HAL_SPI_DATA_WIDTH_8_BITS
#define XF_HAL_SPI_DEFAULT_TIMEOUT          1000
#define XF_HAL_SPI_DEFAULT_SPEED            1000*500
#define XF_HAL_SPI_DEFAULT_SCLK_NUM         1
#define XF_HAL_SPI_DEFAULT_CS_NUM           2
#define XF_HAL_SPI_DEFAULT_MOSI_NUM         3
#define XF_HAL_SPI_DEFAULT_MISO_NUM         4
#define XF_HAL_SPI_DEFAULT_QUADWP_NUM       XF_HAL_GPIO_NUM_NONE
#define XF_HAL_SPI_DEFAULT_QUADHD_NUM       XF_HAL_GPIO_NUM_NONE

/* ==================== [Typedefs] ========================================== */

typedef struct {
    union {
        int mosi_io_num;
        int data0_io_num;
    };
    union {
        int miso_io_num;
        int data1_io_num;
    };
    int sclk_io_num;
    union {
        int quadwp_io_num;
        int data2_io_num;
    };
    union {
        int quadhd_io_num;
        int data3_io_num;
    };
} spi_bus_config_t;

typedef struct {
    uint8_t mode;
    int clock_speed_hz;
    int spics_io_num;
} spi_device_interface_config_t;

typedef struct _port_spi_t {
    xf_hal_spi_config_t *spi_config;
    uint32_t port;
    void *config;
} port_spi_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static int port_spi_open(xf_hal_dev_t *dev);
static int port_spi_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_spi_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_spi_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static int port_spi_close(xf_hal_dev_t *dev);

// 用户实现id的转换方式
static uint32_t _spi_id_to_port(uint32_t id);

// 底层sdk的操作函数
static int spi_bus_initialize(uint32_t spi_port,
                              const spi_bus_config_t *bus_config);
static int spi_bus_add_device(uint32_t spi_port,
                              const spi_device_interface_config_t *dev_config);
static int spi_device_polling_transmit(uint32_t spi_port, const void *tx_buffer,
                                       void *rx_buffer, size_t length);

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_hal_SPI_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_spi_open,
        .ioctl = port_spi_ioctl,
        .write = port_spi_write,
        .read = port_spi_read,
        .close = port_spi_close,
    };
    xf_hal_spi_register(&ops);
}


/* ==================== [Static Functions] ================================== */

static int port_spi_open(xf_hal_dev_t *dev)
{
    port_spi_t *spi = (port_spi_t *)malloc(sizeof(port_spi_t));
    if (spi == NULL) {
        return -1;
    }

    spi->port = _spi_id_to_port(dev->id);

    dev->platform_data = spi;

    return 0;
}

static int port_spi_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_spi_config_t *spi_config = (xf_hal_spi_config_t *)config;
    port_spi_t *spi = (port_spi_t *)dev->platform_data;

    if (cmd == XF_HAL_SPI_CMD_DEFAULT) {
        spi_config->hosts = XF_HAL_SPI_DEFAULT_HOSTS;
        spi_config->bit_order = XF_HAL_SPI_DEFAULT_BIT_ORDER;
        spi_config->mode =  XF_HAL_SPI_DEFAULT_MODE;
        spi_config->data_width = XF_HAL_SPI_DEFAULT_DATA_WIDTH;
        spi_config->timeout_ms = XF_HAL_SPI_DEFAULT_TIMEOUT;
        spi_config->speed = XF_HAL_SPI_DEFAULT_SPEED;
        spi_config->gpio.sclk_num = XF_HAL_SPI_DEFAULT_SCLK_NUM;
        spi_config->gpio.cs_num = XF_HAL_SPI_DEFAULT_CS_NUM;
        spi_config->gpio.mosi_num = XF_HAL_SPI_DEFAULT_MOSI_NUM;
        spi_config->gpio.miso_num = XF_HAL_SPI_DEFAULT_MISO_NUM;
        spi_config->gpio.quadhd_num = XF_HAL_SPI_DEFAULT_QUADWP_NUM;
        spi_config->gpio.quadwp_num = XF_HAL_SPI_DEFAULT_QUADHD_NUM;
        spi->config = config;
    }

    if (cmd & XF_HAL_SPI_CMD_GPIO) {
        spi_bus_config_t config;
        config.miso_io_num = spi_config->gpio.miso_num;
        config.mosi_io_num = spi_config->gpio.mosi_num;
        config.sclk_io_num = spi_config->gpio.sclk_num;
        config.quadhd_io_num = spi_config->gpio.quadhd_num;
        config.quadwp_io_num = spi_config->gpio.quadwp_num;
        spi_bus_initialize(spi->port, &config);
    }

    if (cmd & XF_HAL_SPI_CMD_GPIO || cmd & XF_HAL_SPI_CMD_SPEED || cmd & XF_HAL_SPI_CMD_MODE) {
        spi_device_interface_config_t config;
        config.clock_speed_hz = spi_config->speed;
        config.mode = spi_config->mode;
        config.spics_io_num = spi_config->gpio.cs_num;
        spi_bus_add_device(spi->port, &config);
    }

    return 0;
}

static int port_spi_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_spi_t *spi = (port_spi_t *)dev->platform_data;

    spi_device_polling_transmit(spi->port, NULL, buf, count);

    return 0;
}

static int port_spi_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_spi_t *spi = (port_spi_t *)dev->platform_data;

    spi_device_polling_transmit(spi->port, buf, NULL, count);

    return 0;
}

static int port_spi_close(xf_hal_dev_t *dev)
{
    port_spi_t *spi = (port_spi_t *)dev->platform_data;
    free(spi);
    return 0;
}

static uint32_t _spi_id_to_port(uint32_t id)
{
    return id;
}

static int spi_bus_initialize(uint32_t spi_port,
                              const spi_bus_config_t *bus_config)
{
    printf("\nmiso_io_num:%d\n", bus_config->miso_io_num);
    printf("\nmosi_io_num:%d\n", bus_config->mosi_io_num);
    printf("\nsclk_io_num:%d\n", bus_config->sclk_io_num);
    printf("\ninit\n");
    return 0;
}

static int spi_bus_add_device(uint32_t spi_port,
                              const spi_device_interface_config_t *dev_config)
{
    printf("\nspics_io_num:%d\n", dev_config->spics_io_num);
    printf("\nmode:%d\n", dev_config->mode);
    printf("\nclock_speed_hz:%d\n", dev_config->clock_speed_hz);

    return 0;
}

static int spi_device_polling_transmit(uint32_t spi_port, const void *tx_buffer,
                                       void *rx_buffer, size_t length)
{
    const char *buffer = "hello!";
    if (rx_buffer != NULL) {
        strncpy((char *)rx_buffer, buffer, length);
    }


    if (tx_buffer != NULL) {
        printf("write:%s\n", (const char *)tx_buffer);
    }

    return 0;
}

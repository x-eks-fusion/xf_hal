/**
 * @file port_i2c.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-05-16
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

#define XF_HAL_I2C_DEFAULT_HOSTS            XF_HAL_I2C_HOSTS_MASTER
#define XF_HAL_I2C_DEFAULT_ADDRESS_WIDTH    XF_HAL_I2C_ADDRESS_WIDTH_7BIT
#define XF_HAL_I2C_DEFAULT_ADDRESS          0x56
#define XF_HAL_I2C_DEFAULT_MEM_ADDR_EN      false
#define XF_HAL_I2C_DEFAULT_MEM_ADDR_WIDTH   XF_HAL_I2C_MEM_ADDR_WIDTH_8BIT
#define XF_HAL_I2C_DEFAULT_SPEED            1000000
#define XF_HAL_I2C_DEFAULT_SCL_NUM              1
#define XF_HAL_I2C_DEFAULT_SDA_NUM              2

/* ==================== [Typedefs] ========================================== */

typedef struct {
    bool hosts;
    int sda_io_num;
    int scl_io_num;
    union {
        struct {
            uint32_t clk_speed;      /*!< I2C clock frequency for master hosts, (no higher than 1MHz for now) */
        } master;                    /*!< I2C master config */
        struct {
            uint8_t addr_10bit_en;   /*!< I2C 10bit address hosts enable for slave hosts */
            uint16_t slave_addr;     /*!< I2C address for slave hosts */
            uint32_t maximum_speed;  /*!< I2C expected clock speed from SCL. */
        } slave;                     /*!< I2C slave config */
    };
} i2c_config_t;

typedef struct _port_i2c_t {
    xf_hal_i2c_config_t *i2c_config;
    uint32_t port;
    void *config;
} port_i2c_t;

/* ==================== [Static Prototypes] ================================= */

// 用户实现对接的部分
static int port_i2c_open(xf_hal_dev_t *dev);
static int port_i2c_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config);
static int port_i2c_read(xf_hal_dev_t *dev, void *buf, size_t count);
static int port_i2c_write(xf_hal_dev_t *dev, const void *buf, size_t count);
static int port_i2c_close(xf_hal_dev_t *dev);

// 用户实现id的转换方式
static uint32_t _i2c_id_to_port(uint32_t id);

// 底层sdk的操作函数
static void _i2c_init(uint32_t i2c_port, i2c_config_t config);
static void _i2c_deinit(uint32_t i2c_port);
static void _i2c_master_write_to_dev(uint32_t i2c_num,
                                     uint8_t device_address,
                                     const uint8_t *write_buffer,
                                     size_t write_size,
                                     uint32_t timeout_ms);
static void _i2c_master_read_from_dev(uint32_t i2c_num,
                                      uint8_t device_address,
                                      uint8_t *read_buffer,
                                      size_t read_size,
                                      uint32_t timeout_ms);

int _i2c_slave_write_buffer(uint32_t i2c_num, const uint8_t *data, int size,
                            uint32_t timeout_ms);

int _i2c_slave_read_buffer(uint32_t i2c_num, uint8_t *data, size_t max_size,
                           uint32_t timeout_ms);
/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_hal_I2C_reg(void)
{
    xf_driver_ops_t ops = {
        .open = port_i2c_open,
        .ioctl = port_i2c_ioctl,
        .write = port_i2c_write,
        .read = port_i2c_read,
        .close = port_i2c_close,
    };
    xf_hal_i2c_register(&ops);
}

/* ==================== [Static Functions] ================================== */

static int port_i2c_open(xf_hal_dev_t *dev)
{
    port_i2c_t *i2c = (port_i2c_t *)malloc(sizeof(port_i2c_t));
    if (i2c == NULL) {
        return -1;
    }

    i2c->port = _i2c_id_to_port(dev->id);

    dev->platform_data = i2c;

    return 0;
}

static int port_i2c_ioctl(xf_hal_dev_t *dev, uint32_t cmd, void *config)
{
    xf_hal_i2c_config_t *i2c_config = (xf_hal_i2c_config_t *)config;
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;

    if (cmd == XF_HAL_I2C_CMD_DEFAULT) {
        i2c_config->hosts = XF_HAL_I2C_DEFAULT_HOSTS;
        i2c_config->address_width = XF_HAL_I2C_DEFAULT_ADDRESS_WIDTH;
        i2c_config->address =  XF_HAL_I2C_DEFAULT_ADDRESS;
        i2c_config->mem_addr_en = XF_HAL_I2C_DEFAULT_MEM_ADDR_EN;
        i2c_config->mem_addr_width = XF_HAL_I2C_DEFAULT_MEM_ADDR_WIDTH;
        i2c_config->speed = XF_HAL_I2C_DEFAULT_SPEED;
        i2c_config->scl_num = XF_HAL_I2C_DEFAULT_SCL_NUM;
        i2c_config->sda_num = XF_HAL_I2C_DEFAULT_SDA_NUM;
        i2c->config = config;
    }

    if (cmd & XF_HAL_I2C_CMD_HOSTS
            || cmd & XF_HAL_I2C_CMD_SCL_NUM
            || cmd & XF_HAL_I2C_CMD_SDA_NUM
            || cmd & XF_HAL_I2C_CMD_ADDRESS_WIDTH
            || cmd & XF_HAL_I2C_CMD_SPEED
            || cmd & XF_HAL_I2C_CMD_ADDRESS) {
        i2c_config_t config = {0};

        config.hosts = i2c_config->hosts;
        config.scl_io_num = i2c_config->scl_num;
        config.sda_io_num = i2c_config->sda_num;
        if (i2c_config->hosts == XF_HAL_I2C_HOSTS_MASTER) {
            config.master.clk_speed = i2c_config->speed;
        } else {
            if (i2c_config->address_width == XF_HAL_I2C_ADDRESS_WIDTH_10BIT) {
                config.slave.addr_10bit_en = true;
            } else {
                config.slave.addr_10bit_en = false;
            }
            config.slave.maximum_speed = i2c_config->speed;
            config.slave.slave_addr = i2c_config->address;
        }
        _i2c_init(i2c->port, config);
    }

    return 0;
}

static int port_i2c_read(xf_hal_dev_t *dev, void *buf, size_t count)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;
    xf_hal_i2c_config_t *i2c_config = (xf_hal_i2c_config_t *)i2c->config;

    if (i2c_config->mem_addr_en != XF_HAL_I2C_MEM_ADDR_DISABLE) {
        return -1;
    }


    if (i2c_config->hosts == XF_HAL_I2C_HOSTS_MASTER) {
        _i2c_master_read_from_dev(i2c->port, i2c_config->address, buf, count,
                                  i2c_config->timeout_ms);
    } else {
        _i2c_slave_read_buffer(i2c->port, buf, count, i2c_config->timeout_ms);
    }

    return 0;

}

static int port_i2c_write(xf_hal_dev_t *dev, const void *buf, size_t count)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;
    xf_hal_i2c_config_t *i2c_config = (xf_hal_i2c_config_t *)i2c->config;


    if (i2c_config->mem_addr_en !=  XF_HAL_I2C_MEM_ADDR_DISABLE) {
        return -1;
    }


    if (i2c_config->hosts == XF_HAL_I2C_HOSTS_MASTER) {
        _i2c_master_write_to_dev(i2c->port, i2c_config->address, buf, count,
                                 i2c_config->timeout_ms);
    } else {
        _i2c_slave_write_buffer(i2c->port, buf, count, i2c_config->timeout_ms);
    }

    return 0;
}

static int port_i2c_close(xf_hal_dev_t *dev)
{
    port_i2c_t *i2c = (port_i2c_t *)dev->platform_data;
    _i2c_deinit(i2c->port);
    free(i2c);
    return 0;
}

static uint32_t _i2c_id_to_port(uint32_t id)
{
    return id;
}

static void _i2c_init(uint32_t i2c_port, i2c_config_t config)
{
    printf("\ni2c init!\n");
}

static void _i2c_deinit(uint32_t i2c_port)
{
    printf("\ni2c deinit!\n");
}

static void _i2c_master_write_to_dev(uint32_t i2c_num,
                                     uint8_t device_address,
                                     const uint8_t *write_buffer,
                                     size_t write_size,
                                     uint32_t timeout_ms)
{
    printf("master write:");
    for (int i = 0; i < write_size; i++) {
        printf("%c", write_buffer[i]);
    }
    printf("\n");
}

static void _i2c_master_read_from_dev(uint32_t i2c_num,
                                      uint8_t device_address,
                                      uint8_t *read_buffer,
                                      size_t read_size,
                                      uint32_t timeout_ms)
{
    printf("master read\n");
    const char *buffer = "hello!";
    strncpy((char *)read_buffer, buffer, read_size);
}

int _i2c_slave_write_buffer(uint32_t i2c_num, const uint8_t *data, int size,
                            uint32_t timeout_ms)
{
    printf("slave write:");
    for (int i = 0; i < size; i++) {
        printf("%c", data[i]);
    }
    printf("\n");

    return 0;
}

int _i2c_slave_read_buffer(uint32_t i2c_num, uint8_t *data, size_t max_size,
                           uint32_t timeout_ms)
{
    printf("slave read\n");
    const char *buffer = "hello!";
    strncpy((char *)data, buffer, max_size);

    return 0;
}

/**
 * @file xf_hal_i2c.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 的 i2c(同步半双工串行通信总线) 设备抽象。
 * @version 0.1
 * @date 2024-05-16
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_I2C_H__
#define __XF_HAL_I2C_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_device_config.h"

/**
 * @ingroup group_xf_hal_user
 * @defgroup group_xf_hal_user_i2c i2c
 * @brief 同步半双工串行通信总线。
 * @{
 */

#if XF_HAL_I2C_IS_ENABLE

#include "xf_hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief i2c 序号。
 */
typedef uint32_t xf_i2c_num_t;

/**
 * @brief i2c 从机地址宽度。
 */
typedef enum _xf_hal_i2c_address_width_t {
    _XF_HAL_I2C_ADDRESS_WIDTH_BASE = 0,

    XF_HAL_I2C_ADDRESS_WIDTH_7BIT = _XF_HAL_I2C_ADDRESS_WIDTH_BASE, /*!< 7 位地址宽度 */
    XF_HAL_I2C_ADDRESS_WIDTH_10BIT, /*!< 10 位地址宽度模式 */

    _XF_HAL_I2C_ADDRESS_WIDTH_MAX
} xf_hal_i2c_address_width_t;

/**
 * @brief i2c 主从模式。
 */
typedef enum _xf_hal_i2c_hosts_t {
    _XF_HAL_I2C_HOSTS_BASE = 0,

    XF_HAL_I2C_HOSTS_MASTER = _XF_HAL_I2C_HOSTS_BASE, /*!< i2c主机模式 */
    XF_HAL_I2C_HOSTS_SLAVE, /*!< i2c从机模式 */

    _XF_HAL_I2C_HOSTS_MAX
} xf_hal_i2c_hosts_t;

/**
 * @brief i2c 从机内存地址是否启用。
 */
typedef enum _xf_hal_i2c_mem_addr_en_t {
    _XF_HAL_I2C_MEM_ADDR_BASE = 0,

    XF_HAL_I2C_MEM_ADDR_DISABLE = _XF_HAL_I2C_MEM_ADDR_BASE, /*!< i2c 禁用从机内存地址 */
    XF_HAL_I2C_MEM_ADDR_ENABLE, /*!< i2c 启用从机内存地址 */

    _XF_HAL_I2C_MEM_ADDR_MAX
} xf_hal_i2c_mem_addr_en_t;

/**
 * @brief i2c 从机内存地址宽度。
 */
typedef enum _xf_hal_i2c_mem_addr_width_t {
    _XF_HAL_I2C_MEM_ADDR_WIDTH_BASE = 0,

    XF_HAL_I2C_MEM_ADDR_WIDTH_8BIT = _XF_HAL_I2C_MEM_ADDR_WIDTH_BASE, /*!< i2c 从机内存地址 8 位宽度 */
    XF_HAL_I2C_MEM_ADDR_WIDTH_16BIT,    /*!< i2c 从机内存地址 16 位宽度 */
    XF_HAL_I2C_MEM_ADDR_WIDTH_24BIT,    /*!< i2c 从机内存地址 24 位宽度 */
    XF_HAL_I2C_MEM_ADDR_WIDTH_32BIT,    /*!< i2c 从机内存地址 32 位宽度 */

    _XF_HAL_I2C_MEM_ADDR_WIDTH_MAX
} xf_hal_i2c_mem_addr_width_t;

/**
 * @brief 对移植者，用于对接 i2c 指令的命令。
 */
typedef enum _xf_hal_i2c_cmd_t {
    XF_HAL_I2C_CMD_DEFAULT          = 0x0,          /*!< 默认参数命令，在创建设备时优先执行 */

    XF_HAL_I2C_CMD_HOSTS            = 0x1 << 0,     /*!< 主从命令，见 @ref xf_hal_i2c_config_t.hosts */
    XF_HAL_I2C_CMD_ENABLE           = 0x1 << 1,     /*!< 启用命令，见 @ref xf_hal_i2c_config_t.enable */
    XF_HAL_I2C_CMD_ADDRESS_WIDTH    = 0x1 << 2,     /*!< 地址宽度命令，见 @ref xf_hal_i2c_config_t.address_width */
    XF_HAL_I2C_CMD_ADDRESS          = 0x1 << 3,     /*!< 地址命令，见 @ref xf_hal_i2c_config_t.address */
    XF_HAL_I2C_CMD_MEM_ADDR_EN      = 0x1 << 4,     /*!< 内存地址启用命令，见 @ref xf_hal_i2c_config_t.mem_addr_en */
    XF_HAL_I2C_CMD_MEM_ADDR_WIDTH   = 0x1 << 5,     /*!< 内存地址宽度命令，见 @ref xf_hal_i2c_config_t.mem_addr_width */
    XF_HAL_I2C_CMD_MEM_ADDR         = 0x1 << 6,     /*!< 内存地址命令，见 @ref xf_hal_i2c_config_t.mem_addr */
    XF_HAL_I2C_CMD_SPEED            = 0x1 << 7,     /*!< 速度命令，见 @ref xf_hal_i2c_config_t.speed */
    XF_HAL_I2C_CMD_TIMEOUT          = 0x1 << 8,     /*!< 超时命令，见 @ref xf_hal_i2c_config_t.timeout_ms */
    XF_HAL_I2C_CMD_SCL_NUM          = 0x1 << 9,     /*!< scl io 命令，见 @ref xf_hal_i2c_config_t.scl_num */
    XF_HAL_I2C_CMD_SDA_NUM          = 0x1 << 10,    /*!< sda io 命令，见 @ref xf_hal_i2c_config_t.sda_num */

    XF_HAL_I2C_CMD_ALL              = 0x7FFFFFFF,  /*!< 默认设置命令，在创建设备时其次执行 */
} xf_hal_i2c_cmd_t;

/**
 * @brief 对移植者，用于对接 i2c 设置的参数。
 */
typedef struct _xf_hal_i2c_config_t {
    uint32_t hosts          : 1;    /*!< 主从参数，0 为主机，1 为从机 */
    uint32_t enable         : 1;    /*!< 启用参数，0 为禁用，1 为启用 */
    uint32_t address_width  : 1;    /*!< 地址宽度参数，0 为7bit，1 为10bit */
    uint32_t address        : 16;   /*!< 地址参数，i2c 从机地址 */
    uint32_t mem_addr_en    : 1;    /*!< 内存地址启用参数，0为禁用，1为启用 */
    uint32_t mem_addr_width : 2;    /*!< 内存地址宽度参数，可能为 8bit、16bit、24bit、32bit */
    uint32_t reserve        : 10;
    uint32_t mem_addr;              /*!< 内存地址参数，i2c 从机内存地址 */
    uint32_t speed;                 /*!< 速度参数，单位为 hz */
    uint32_t timeout_ms;            /*!< 超时参数，单位为 ms */
    xf_gpio_num_t scl_num;          /*!< scl io 参数，设置 scl io 序号 */
    xf_gpio_num_t sda_num;          /*!< sda io 参数，设置 sda io 序号 */
} xf_hal_i2c_config_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief i2c 初始化函数。使用 i2c 之前必须要进行初始化。
 *
 * @param i2c_num i2c 的序号。
 * @param hosts 设置主机或从机。见 @ref xf_hal_i2c_hosts_t.
 * @param speed i2c 速度。单位 hz
 * @return xf_err_t
 *      - XF_OK 成功初始化
 *      - other 初始化失败
 */
xf_err_t xf_hal_i2c_init(xf_i2c_num_t i2c_num, xf_hal_i2c_hosts_t hosts, uint32_t speed);

/**
 * @brief i2c 反初始化函数，恢复到未初始化状态且回收相关资源。
 *
 * @param i2c_num i2c 的序号。
 * @return xf_err_t
 *      - XF_OK 成功反初始化
 *      - XF_ERR_UNINIT 该 i2c 未初始化
 *      - other 反初始化失败
 */
xf_err_t xf_hal_i2c_deinit(xf_i2c_num_t i2c_num);

/**
 * @brief i2c 启用函数。i2c 配置好功能后，可以调用 i2c 启用函数进行激活。
 *
 * @note 激活后，方可使用 xf_hal_i2c_write 等函数。激活之前的配置是静态配置。
 *
 * @param i2c_num i2c 的序号。
 * @return xf_err_t
 *      - XF_OK 成功启用
 *      - XF_ERR_UNINIT 该 i2c 未初始化
 *      - other 启用失败
 */
xf_err_t xf_hal_i2c_enable(xf_i2c_num_t i2c_num);

/**
 * @brief i2c 禁用函数。
 *
 * 如果 i2c 正在启用状态，且不支持动态配置。
 * 则需要该函数先将 i2c 禁用，然后进行配置。再次进行启用。
 *
 * @param i2c_num i2c 的序号。
 * @return xf_err_t
 *      - XF_OK 成功禁用
 *      - XF_ERR_UNINIT 该 i2c 未初始化
 *      - other 禁用失败
 */
xf_err_t xf_hal_i2c_disable(xf_i2c_num_t i2c_num);

/**
 * @brief i2c 设置 gpio。
 *
 * @note 不是所有芯片都支持这个功能。
 *
 * @param i2c_num i2c 的序号
 * @param scl_num scl 的 io 序号
 * @param sda_num sda 的 io 序号
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 i2c 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_i2c_set_gpio(xf_i2c_num_t i2c_num, xf_gpio_num_t scl_num, xf_gpio_num_t sda_num);

/**
 * @brief i2c 设置地址宽度。
 *
 * @param i2c_num i2c 的序号。
 * @param address_width 地址宽度。可以设置 7 bit 或者是 10 bit。
 *                      见 @ref xf_hal_i2c_address_width_t.
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 i2c 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_i2c_set_address_width(xf_i2c_num_t i2c_num, xf_hal_i2c_address_width_t address_width);

/**
 * @brief i2c 设置 i2c 从机地址。
 *
 * @param i2c_num i2c 的序号。
 * @param address i2c 地址。需要根据之前设置的宽度填写地址。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 i2c 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_i2c_set_address(xf_i2c_num_t i2c_num, uint16_t address);

/**
 * @brief i2c 设置 i2c 内存地址宽度。
 *
 * @param i2c_num i2c 的序号。
 * @param mem_addr_widths 内存地址宽度。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 i2c 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_i2c_set_mem_addr_width(xf_i2c_num_t i2c_num, xf_hal_i2c_mem_addr_width_t mem_addr_widths);

/**
 * @brief i2c 指定从机内存写入。
 *
 * @param i2c_num i2c 的序号。
 * @param mem_addr 写入内存地址指针。
 * @param buffer 写入数据的指针。
 * @param size 写入数据的大小。
 * @param timeout_ms 超时时间，单位为 ms（针对有RTOS的底层）。
 * @return xf_err_t
 *      - XF_OK 成功写入
 *      - XF_ERR_UNINIT 该 i2c 未初始化
 *      - other 写入失败
 */
int xf_hal_i2c_write_mem(xf_i2c_num_t i2c_num, uint32_t mem_addr, const uint8_t *buffer, uint32_t size,
                         uint32_t timeout_ms);

/**
 * @brief i2c 指定从机内存读取。
 *
 * @param i2c_num i2c 的序号。
 * @param mem_addr 读取内存地址指针。
 * @param buffer 读取的数据指针。
 * @param size 读取数据的大小。
 * @param timeout_ms 超时时间，单位为 ms（针对有RTOS的底层）。
 * @return int 返回实际读取大小。
 */
int xf_hal_i2c_read_mem(xf_i2c_num_t i2c_num, uint32_t mem_addr, uint8_t *buffer, uint32_t size, uint32_t timeout_ms);

/**
 * @brief i2c 写入数据。
 *
 * @param i2c_num i2c 的序号。
 * @param buffer 写入的数据指针。
 * @param size 写入数据的大小。
 * @param timeout_ms 超时时间，单位为ms（针对有RTOS的底层）。
 * @return int 返回实际写入大小。
 */
int xf_hal_i2c_write(xf_i2c_num_t i2c_num, const uint8_t *buffer, uint32_t size, uint32_t timeout_ms);

/**
 * @brief i2c 读取数据。
 *
 * @param i2c_num i2c 的序号。
 * @param buffer 读取的数据指针。
 * @param size 读取数据的大小。
 * @param timeout_ms 超时时间，单位为ms（针对有RTOS的底层）。
 * @return int 返回实际读取大小。
 */
int xf_hal_i2c_read(xf_i2c_num_t i2c_num, uint8_t *buffer, uint32_t size, uint32_t timeout_ms);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // XF_HAL_I2C_IS_ENABLE

/**
 * End of group_xf_hal_user_i2c
 * @}
 */

#endif // __XF_HAL_I2C_H__

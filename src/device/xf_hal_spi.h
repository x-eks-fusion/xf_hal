/**
 * @file xf_hal_spi.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 的 spi(串行外设接口) 设备抽象。
 * @version 0.1
 * @date 2024-05-17
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_SPI_H__
#define __XF_HAL_SPI_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_device_config.h"

/**
 * @ingroup group_xf_hal_user
 * @defgroup group_xf_hal_user_spi spi
 * @brief 串行外设接口。
 * @{
 */

#if XF_HAL_SPI_IS_ENABLE

#include "xf_hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief spi 序号。
 */
typedef uint32_t xf_spi_num_t;

/**
 * @brief spi 回调函数原型。
 *
 * @param spi_num 请求回调的 spi 序号。
 * @param user_data 用户数据，见 @ref xf_hal_spi_set_prev_cb
 *                  或 @ref xf_hal_spi_set_post_cb
 *                  的 `user_data` 参数。
 */
typedef void (*xf_hal_spi_cb_t)(xf_spi_num_t spi_num, void *user_data);

/**
 * @brief spi 的回调函数结构体。
 */
typedef struct _xf_hal_spi_callback_t {
    xf_hal_spi_cb_t callback;
    void *user_data;
} xf_hal_spi_callback_t;

/**
 * @brief spi 的主从模式。
 */
typedef enum _xf_hal_spi_hosts_t {
    _XF_HAL_SPI_HOSTS_BASE = 0,

    XF_HAL_SPI_HOSTS_MASTER = _XF_HAL_SPI_HOSTS_BASE, /*!< 主机模式 */
    XF_HAL_SPI_HOSTS_SLAVE, /*!< 从机模式 */

    _XF_HAL_SPI_HOSTS_MAX
} xf_hal_spi_hosts_t;

/**
 * @brief spi 传输位顺序。
 */
typedef enum _xf_hal_spi_bit_order_t {
    _XF_HAL_SPI_BIT_ORDER_BASE = 0,

    XF_HAL_SPI_BIT_ORDER_LSB_FIRST = _XF_HAL_SPI_BIT_ORDER_BASE, /*!< 低位在前的传输方式 */
    XF_HAL_SPI_BIT_ORDER_MSB_FIRST, /*!< 高位在前的传输方式 */

    _XF_HAL_SPI_BIT_ORDER_MAX
} xf_hal_spi_bit_order_t;

/**
 * @brief spi 模式。
 *
 * 每种模式由两个参数决定：时钟极性（CPOL）和时钟相位（CPHA）。
 */
typedef enum _xf_hal_spi_mode_t {
    _XF_HAL_SPI_MODE_BASE = 0,

    XF_HAL_SPI_MODE_0 = _XF_HAL_SPI_MODE_BASE, /*!< CPOL = 0, CPHA = 0 */
    XF_HAL_SPI_MODE_1,  /*!< CPOL = 0, CPHA = 1 */
    XF_HAL_SPI_MODE_2,  /*!< CPOL = 1, CPHA = 0 */
    XF_HAL_SPI_MODE_3,  /*!< CPOL = 1, CPHA = 1 */

    _XF_HAL_SPI_MODE_MAX
} xf_hal_spi_mode_t;

/**
 * @brief spi 传输数据位宽。
 */
typedef enum _xf_hal_spi_data_width_t {
    _XF_HAL_SPI_DATA_WIDTH_BASE = 0,

    XF_HAL_SPI_DATA_WIDTH_8_BITS = _XF_HAL_SPI_DATA_WIDTH_BASE, /*!< spi 传输位宽为 8bit 数据 */
    XF_HAL_SPI_DATA_WIDTH_16_BITS,  /*!< spi 传输位宽为 16bit 数据 */
    XF_HAL_SPI_DATA_WIDTH_32_BITS,  /*!< spi 传输位宽为 32bit 数据 */

    _XF_HAL_SPI_DATA_WIDTH_MAX
} xf_hal_spi_data_width_t;

/**
 * @brief 用于对接 spi 设置的命令。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef enum _xf_hal_spi_cmd_t {
    XF_HAL_SPI_CMD_DEFAULT          = 0x0,          /*!< 默认参数命令，在创建设备时优先执行 */

    XF_HAL_SPI_CMD_HOSTS            = 0x1 << 0,     /*!< 主从命令，见 @ref xf_hal_spi_config_t.hosts */
    XF_HAL_SPI_CMD_ENABLE           = 0x1 << 1,     /*!< 启用命令，见 @ref xf_hal_spi_config_t.enable */
    XF_HAL_SPI_CMD_BIT_ORDER        = 0x1 << 2,     /*!< 字节序命令，见 @ref xf_hal_spi_config_t.bit_order */
    XF_HAL_SPI_CMD_MODE             = 0x1 << 3,     /*!< 模式命令，见 @ref xf_hal_spi_config_t.mode */
    XF_HAL_SPI_CMD_DATA_WIDTH       = 0x1 << 4,     /*!< 传输位宽命令，见 @ref xf_hal_spi_config_t.data_width */
    XF_HAL_SPI_CMD_TIMEOUT          = 0x1 << 5,     /*!< 超时命令，见 @ref xf_hal_spi_config_t.timeout_ms */
    XF_HAL_SPI_CMD_SPEED            = 0x1 << 6,     /*!< 速度命令，见 @ref xf_hal_spi_config_t.speed */
    XF_HAL_SPI_CMD_GPIO             = 0x1 << 7,     /*!< 传输io命令，见 @ref xf_hal_spi_config_t.gpio */
    XF_HAL_SPI_CMD_PREV_CB          = 0x1 << 8,     /*!< 传输前回调命令，见 @ref xf_hal_spi_config_t.prev_cb */
    XF_HAL_SPI_CMD_POST_CB          = 0x1 << 9,     /*!< 传输后回调命令，见 @ref xf_hal_spi_config_t.post_cb */

    XF_HAL_SPI_CMD_ALL             = 0x7FFFFFFF,  /*!< 默认设置命令，在创建设备时其次执行 */
} xf_hal_spi_cmd_t;

/**
 * @brief spi 的引脚设置。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef struct _xf_hal_spi_gpio_t {
    xf_gpio_num_t sclk_num;
    xf_gpio_num_t cs_num;
    union {
        xf_gpio_num_t data0_num;
        xf_gpio_num_t mosi_num;
    };
    union {
        xf_gpio_num_t data1_num;
        xf_gpio_num_t miso_num;
    };
    union {
        xf_gpio_num_t data2_num;
        xf_gpio_num_t quadwp_num;
    };
    union {
        xf_gpio_num_t data3_num;
        xf_gpio_num_t quadhd_num;
    };
} xf_hal_spi_gpio_t;

/**
 * @brief 用于对接 SPI 设置的参数。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef struct _xf_hal_spi_config_t {
    uint32_t hosts      : 1;        /*!< 主从参数，0 为主机，1 为从机，见 @ref xf_hal_spi_hosts_t */
    uint32_t enable     : 1;        /*!< 启用参数，0 为禁用，1 为启用 */
    uint32_t bit_order  : 1;        /*!< 字节序参数，0 为低位在前，1 为高位在前，
                                     *   见 @ref xf_hal_spi_bit_order_t */
    uint32_t mode       : 2;        /*!< 模式参数，设置时钟极性和时钟相位，见 @ref xf_hal_spi_mode_t */
    uint32_t data_width : 2;        /*!< 传输数据位宽参数，0 为 8bit，1 为 16 bit，2 为 32 bit，
                                     *   见 @ref xf_hal_spi_data_width_t */
    uint32_t reserve    : 25;
    uint32_t timeout_ms;            /*!< 传输超时参数，单位为 ms */
    uint32_t speed;                 /*!< 传输速度参数，单位为 hz */
    xf_hal_spi_gpio_t gpio;         /*!< 传输 IO 参数 */
    xf_hal_spi_callback_t prev_cb;  /*!< 传输前回调参数 */
    xf_hal_spi_callback_t post_cb;  /*!< 传输后回调参数 */
} xf_hal_spi_config_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief spi 初始化函数。
 *
 * 使用 spi 之前必须要进行初始化。
 *
 * @param spi_num spi 的序号。
 * @param hosts 主从机模式。见 @ref xf_hal_spi_hosts_t.
 * @param speed 传输速度。 单位为 hz
 * @return xf_err_t
 *      - XF_OK 成功初始化
 *      - other 初始化失败
 */
xf_err_t xf_hal_spi_init(xf_spi_num_t spi_num, xf_hal_spi_hosts_t hosts, uint32_t speed);

/**
 * @brief spi 反初始化函数。
 *
 * 恢复到未初始化状态且回收相关资源。
 *
 * @param spi_num spi 的序号。
 * @return xf_err_t
 *      - XF_OK 成功反初始化
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 反初始化失败
 */
xf_err_t xf_hal_spi_deinit(xf_spi_num_t spi_num);

/**
 * @brief spi 启用函数。
 *
 * spi 配置好功能后，可以调用 spi 启用函数进行激活。
 *
 * @note 激活后，方可使用 xf_hal_spi_write 等函数。激活之前的配置是静态配置。
 * @note 激活之后的配置属于动态配置，不一定所有的芯片都支持动态配置。
 *
 * @param spi_num spi 的序号。
 * @return xf_err_t
 *      - XF_OK 成功启用
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 启用失败
 */
xf_err_t xf_hal_spi_enable(xf_spi_num_t spi_num);

/**
 * @brief spi 禁用函数。
 *
 * @note 如果 spi 正在启用状态，且不支持动态配置。则需要该函数，
 *       先将 spi 禁用，然后进行配置。再次进行启用。
 *
 * @param spi_num spi 的序号。
 * @return xf_err_t
 *      - XF_OK 成功禁用
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 禁用失败
 */
xf_err_t xf_hal_spi_disable(xf_spi_num_t spi_num);

/**
 * @brief 设置 spi 传输前回调。
 *
 * callback 会在传输开始前进行调用。
 *
 * @param spi_num spi 的序号。
 * @param callback 传输前的回调函数。
 * @param user_data 用户自定义参数。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_spi_set_prev_cb(xf_spi_num_t spi_num, xf_hal_spi_cb_t callback, void *user_data);

/**
 * @brief 设置 spi 传输后回调。
 *
 * callback 会在传输结束后进行调用。
 *
 * @param spi_num spi 的序号。
 * @param callback 传输后的回调函数。
 * @param user_data 用户自定义参数。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_spi_set_post_cb(xf_spi_num_t spi_num, xf_hal_spi_cb_t callback, void *user_data);

/**
 * @brief 设置 spi 的输出 io。
 *
 * @param spi_num spi 的序号。
 * @param spi_gpio spi 的 io 口，不需要的设置为 XF_HAL_GPIO_NUM_NONE。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_spi_set_gpio(xf_spi_num_t spi_num, const xf_hal_spi_gpio_t *spi_gpio);

/**
 * @brief 设置 spi 输出的字节序。
 *
 * @param spi_num spi 的序号。
 * @param bit_order spi 的字节序。见 @ref xf_hal_spi_bit_order_t.
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_spi_set_bit_order(xf_spi_num_t spi_num, xf_hal_spi_bit_order_t bit_order);

/**
 * @brief 设置 spi 模式。
 * 
 * 与时钟极性（CPOL）和时钟相位（CPHA）有关
 *
 * @param spi_num spi 的序号。
 * @param mode spi 的模式。总共有四种。见 @ref xf_hal_spi_mode_t.
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_spi_set_mode(xf_spi_num_t spi_num, xf_hal_spi_mode_t mode);

/**
 * @brief 设置 spi 的传输数据宽度。
 *
 * @param spi_num spi 的序号。
 * @param data_width spi 传输数据的宽度。见 @ref xf_hal_spi_data_width_t.
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_spi_set_data_width(xf_spi_num_t spi_num, xf_hal_spi_data_width_t data_width);

/**
 * @brief 设置 spi 的传输速度。
 *
 * @param spi_num spi 的序号。
 * @param speed spi 的传输速度，单位为 hz。
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - XF_ERR_UNINIT 该 spi 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_spi_set_speed(xf_spi_num_t spi_num, uint32_t speed);

/**
 * @brief spi 写入数据函数。
 *
 * @param spi_num spi 的序号。
 * @param buffer 写入数据的指针。
 * @param size 写入数据的大小。
 * @param timeout_ms 超时时间，单位为ms（针对有RTOS的底层）。
 * @return int 实际写入大小
 */
int xf_hal_spi_write(xf_spi_num_t spi_num, const uint8_t *buffer, uint32_t size, uint32_t timeout_ms);

/**
 * @brief spi 读取数据函数。
 *
 * @param spi_num spi 的序号。
 * @param buffer 读取数据函数。
 * @param size 读取数据的大小。
 * @param timeout_ms 超时时间，单位为ms（针对有RTOS的底层）。
 * @return int 实际读取大小
 */
int xf_hal_spi_read(xf_spi_num_t spi_num, uint8_t *buffer, uint32_t size, uint32_t timeout_ms);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_HAL_SPI_H__

/**
 * End of group_xf_hal_user_spi
 * @}
 */

#endif

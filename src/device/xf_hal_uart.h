/**
 * @file xf_hal_uart.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 的 uart(通用异步收发器) 设备抽象。
 * @version 0.1
 * @date 2024-05-15
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_UART_H__
#define __XF_HAL_UART_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_device_config.h"

/**
 * @ingroup group_xf_hal_user
 * @defgroup group_xf_hal_user_uart uart
 * @brief 通用异步收发器。
 * @{
 */

#if XF_HAL_UART_IS_ENABLE

#include "xf_hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief uart 序号。
 */
typedef uint32_t xf_uart_num_t;

/**
 * @brief uart 的数据位。
 */
typedef enum _xf_hal_uart_data_bits_t {
    _XF_HAL_UART_DATA_BIT_BASE = 0,

    XF_HAL_UART_DATA_BIT_5 = _XF_HAL_UART_DATA_BIT_BASE,  /*!< 5 bit 数据位 */
    XF_HAL_UART_DATA_BIT_6, /*!< 6 bit 数据位 */
    XF_HAL_UART_DATA_BIT_7, /*!< 7 bit 数据位 */
    XF_HAL_UART_DATA_BIT_8, /*!< 8 bit 数据位 */
    XF_HAL_UART_DATA_BIT_9, /*!< 9 bit 数据位 */

    _XF_HAL_UART_DATA_BIT_MAX,
} xf_hal_uart_data_bits_t;

/**
 * @brief uart 的停止位。
 */
typedef enum _xf_hal_uart_stop_bits_t {
    _XF_HAL_UART_STOP_BIT_BASE = 0,

    XF_HAL_UART_STOP_BIT_1 = _XF_HAL_UART_STOP_BIT_BASE, /*!< 1 bit 停止位 */
    XF_HAL_UART_STOP_BIT_1_5,   /*!< 1.5 bit 停止位 */
    XF_HAL_UART_STOP_BIT_2,     /*!< 2 bit 停止位 */

    _XF_HAL_UART_STOP_BIT_MAX,
} xf_hal_uart_stop_bits_t;

/**
 * @brief uart 校验位。
 */
typedef enum _xf_hal_uart_parity_bits_t {
    _XF_HAL_UART_PARITY_BITS_BASE = 0,

    XF_HAL_UART_PARITY_BITS_NONE = _XF_HAL_UART_PARITY_BITS_BASE, /*!< 无校验位 */
    XF_HAL_UART_PARITY_BITS_EVEN,   /*!< 偶校验位 */
    XF_HAL_UART_PARITY_BITS_ODD,    /*!< 奇校验位 */
    XF_HAL_UART_PARITY_BITS_SPACE,  /*!< 空校验位 */
    XF_HAL_UART_PARITY_BITS_MARK,   /*!< 标记校验位 */

    _XF_HAL_UART_PARITY_BITS_MAX,
} xf_hal_uart_parity_bits_t;

/**
 * @brief uart 流控模式。
 */
typedef enum _xf_hal_uart_flow_control_t {
    _XF_HAL_UART_FLOW_CONTROL_BASE = 0,

    XF_HAL_UART_FLOW_CONTROL_NONE = _XF_HAL_UART_FLOW_CONTROL_BASE, /*!< 无流控 */
    XF_HAL_UART_FLOW_CONTROL_RTS,       /*!< RTS 流控 */
    XF_HAL_UART_FLOW_CONTROL_CTS,       /*!< CTS 流控 */
    XF_HAL_UART_FLOW_CONTROL_RTS_CTS,   /*!< RTS 和 CTS 流控 */

    _XF_HAL_UART_FLOW_CONTROL_MAX,
} xf_hal_uart_flow_control_t;

/**
 * @brief 用于对接 uart 设置的命令。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef enum _xf_hal_uart_cmd_t {
    XF_HAL_UART_CMD_DEFAULT         = 0x0,          /*!< 默认参数命令，在创建设备时优先执行 */

    XF_HAL_UART_CMD_ENABLE          = 0x1 << 0,     /*!< 启用命令，见 @ref xf_hal_uart_config_t.enable */
    XF_HAL_UART_CMD_DATA_BITS       = 0x1 << 1,     /*!< 数据位命令，见 @ref xf_hal_uart_config_t.data_bits */
    XF_HAL_UART_CMD_STOP_BITS       = 0x1 << 2,     /*!< 停止位命令，见 @ref xf_hal_uart_config_t.stop_bits */
    XF_HAL_UART_CMD_PARITY_BITS     = 0x1 << 3,     /*!< 校验位命令，见 @ref xf_hal_uart_config_t.parity_bits */
    XF_HAL_UART_CMD_FLOW_CONTROL    = 0x1 << 4,     /*!< 流控命令，见 @ref xf_hal_uart_config_t.flow_control */
    XF_HAL_UART_CMD_BAUDRATE        = 0x1 << 5,     /*!< 波特率命令，见 @ref xf_hal_uart_config_t.baudrate */
    XF_HAL_UART_CMD_TX_NUM          = 0x1 << 6,     /*!< tx io 命令，见 @ref xf_hal_uart_config_t.tx_num */
    XF_HAL_UART_CMD_RX_NUM          = 0x1 << 7,     /*!< rx io 命令，见 @ref xf_hal_uart_config_t.rx_num */
    XF_HAL_UART_CMD_RTS_NUM         = 0x1 << 8,     /*!< rtx io 命令，见 @ref xf_hal_uart_config_t.rts_num */
    XF_HAL_UART_CMD_CTS_NUM         = 0x1 << 9,     /*!< ctx io 命令，见 @ref xf_hal_uart_config_t.cts_num */

    XF_HAL_UART_CMD_ALL             = 0x7FFFFFFF, /*!< 默认设置命令，在创建设备时其次执行 */
} xf_hal_uart_cmd_t;

/**
 * @brief 用于对接 uart 设置的参数。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef struct _xf_hal_uart_config_t {
    uint32_t enable         : 1;    /*!< 启用参数，0为禁用，1为启用 */
    uint32_t data_bits      : 3;    /*!< 数据位参数，有 5、6、7、8、9bit 数据位 */
    uint32_t stop_bits      : 2;    /*!< 停止位参数，有 1、1.5、2bit 停止位 */
    uint32_t parity_bits    : 3;    /*!< 校验位参数，有奇、偶、空、标记校验 */
    uint32_t flow_control   : 3;    /*!< 流控参数，有 RTS、CTS、CTS 和 RTS */
    uint32_t reserve        : 20;
    uint32_t baudrate;              /*!< 波特率参数 */
    xf_gpio_num_t tx_num;           /*!< tx io口参数 */
    xf_gpio_num_t rx_num;           /*!< rx io口参数 */
    xf_gpio_num_t rts_num;          /*!< rtx io口参数 */
    xf_gpio_num_t cts_num;          /*!< ctx io口参数 */
} xf_hal_uart_config_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief uart 初始化。
 *
 * 使用 uart 之前必须要进行初始化。
 *
 * @param uart_num uart 的序号。
 * @param baudrate uart 的波特率。
 * @return xf_err_t
 *      - XF_OK 成功初始化
 *      - other 初始化失败
 */
xf_err_t xf_hal_uart_init(xf_uart_num_t uart_num, uint32_t baudrate);

/**
 * @brief uart 反初始化。
 *
 * @param uart_num uart 的序号。
 * @return xf_err_t
 *      - XF_OK 成功反初始化
 *      - XF_ERR_UNINIT 该 uart 未初始化
 *      - other 反初始化失败
 */
xf_err_t xf_hal_uart_deinit(xf_uart_num_t uart_num);

/**
 * @brief uart 启用函数。
 *
 * uart 配置好功能后，可以调用 uart 启用函数进行激活。
 *
 * @note 激活后，方可使用 xf_hal_uart_read 等函数。激活之前的配置是静态配置。
 * @note 激活之后的配置属于动态配置，不一定所有的芯片都支持动态配置。
 *
 * @param uart_num uart 的序号。
 * @return xf_err_t
 *      - XF_OK 成功启用
 *      - XF_ERR_UNINIT 该 uart 未初始化
 *      - other 启用失败
 */
xf_err_t xf_hal_uart_enable(xf_uart_num_t uart_num);

/**
 * @brief uart 禁用函数。
 *
 * 如果 uart 正在启用状态，且不支持动态配置。则需要该函数禁用 uart，配置后再次启用。
 *
 * @param uart_num uart 的序号。
 * @return xf_err_t
 *      - XF_OK 成功禁用
 *      - XF_ERR_UNINIT 该 uart 未初始化
 *      - other 禁用失败
 */
xf_err_t xf_hal_uart_disable(xf_uart_num_t uart_num);

/**
 * @brief uart 设置 uart 的通讯 io。
 *
 * @param uart_num uart 的序号。
 * @param tx_num tx io 的序号。
 * @param rx_num rx io 的序号。
 * @return xf_err_t 成功设置
 *      - XF_ERR_UNINIT 该 uart 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_uart_set_gpio(xf_uart_num_t uart_num, xf_gpio_num_t tx_num, xf_gpio_num_t rx_num);

/**
 * @brief 设置 uart 相关配置。
 *
 * @param uart_num uart 的序号。
 * @param data_bits 设置数据位。见 @ref xf_hal_uart_data_bits_t.
 * @param stop_bits 设置停止位。见 @ref xf_hal_uart_stop_bits_t.
 * @param parity_bits 设置校验位。见 @ref xf_hal_uart_parity_bits_t.
 * @return xf_err_t 成功设置
 *      - XF_ERR_UNINIT 该 uart 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_uart_set_config(xf_uart_num_t uart_num, xf_hal_uart_data_bits_t data_bits,
                                xf_hal_uart_stop_bits_t stop_bits, xf_hal_uart_parity_bits_t parity_bits);

/**
 * @brief 设置 uart 的波特率。
 *
 * @param uart_num uart 的序号。
 * @param baudrate 波特率数值。
 * @return xf_err_t 成功设置
 *      - XF_ERR_UNINIT 该 uart 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_uart_set_baudrate(xf_uart_num_t uart_num, uint32_t baudrate);

/**
 * @brief 获取当前 uart 的波特率。
 *
 * @param uart_num uart 的序号。
 * @return uint32_t 获取到的波特率值。为 0 则获取失败
 */
uint32_t xf_hal_uart_get_baudrate(xf_uart_num_t uart_num);

/**
 * @brief uart 流控 io 设置。
 *
 * @param uart_num uart 的序号。
 * @param flow_control 流控类型。见 @ref xf_hal_uart_flow_control_t.
 * @param rts_num rts io 的序号。如果是 XF_HAL_UART_FLOW_CONTROL_CTS ，则该参数无效。
 * @param cts_num cts io 的序号。如果是 XF_HAL_UART_FLOW_CONTROL_RTS ，则该参数无效。
 * @return xf_err_t 成功设置
 *      - XF_ERR_UNINIT 该 uart 未初始化
 *      - other 设置失败
 */
xf_err_t xf_hal_uart_set_flow_control(xf_uart_num_t uart_num, xf_hal_uart_flow_control_t flow_control,
                                      xf_gpio_num_t rts_num, xf_gpio_num_t cts_num);

/**
 * @brief uart 读取函数。
 *
 * @param uart_num uart 的序号。
 * @param data 读取的数据指针。
 * @param data_len 读取数据长度。
 * @return int 实际读取的大小
 */
int xf_hal_uart_read(xf_uart_num_t uart_num, uint8_t *data, uint32_t data_len);

/**
 * @brief uart 写入函数。
 *
 * @param uart_num uart 的序号。
 * @param data 写入的数据指针。
 * @param data_len 写入数据长度。
 * @return int 实际写入的大小
 */
int xf_hal_uart_write(xf_uart_num_t uart_num, const uint8_t *data, uint32_t data_len);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // XF_HAL_UART_IS_ENABLE

/**
 * End of group_xf_hal_user_uart
 * @}
 */

#endif // __XF_HAL_UART_H__

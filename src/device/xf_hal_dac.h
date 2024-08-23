/**
 * @file xf_hal_dac.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 的 dac(数模转换) 设备。
 * @version 0.1
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_DAC_H__
#define __XF_HAL_DAC_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_device_config.h"

/**
 * @ingroup group_xf_hal_user
 * @defgroup group_xf_hal_user_dac dac
 * @brief 数字模拟转换器。
 * @{
 */

#if XF_HAL_DAC_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief dac 序号。
 */
typedef uint32_t xf_dac_num_t;

/**
 * @brief 用于对接 dac 设置的命令。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef enum _xf_hal_dac_cmd_t {
    XF_HAL_DAC_CMD_DEFAULT      = 0x0,          /*!< 默认参数命令，在创建设备时优先执行 */

    XF_HAL_DAC_CMD_ENABLE       = 0x1 << 0,     /*!< 启用命令，见 @ref xf_hal_dac_config_t.enable */
    XF_HAL_DAC_CMD_RESOLUTION   = 0x1 << 1,     /*!< 分辨率命令，见 @ref xf_hal_dac_config_t.resolution */
    XF_HAL_DAC_CMD_SPEED        = 0x1 << 2,     /*!< 速度命令，见 @ref xf_hal_dac_config_t.speed */
    XF_HAL_DAC_CMD_VALUE_MAX    = 0x1 << 3,     /*!< 数值最大值命令，见 @ref xf_hal_dac_config_t.value_max */
    XF_HAL_DAC_CMD_VERF         = 0x1 << 4,     /*!< 参考电压命令，见 @ref xf_hal_dac_config_t.verf_mv */

    XF_HAL_DAC_CMD_ALL         = 0x7FFFFFFF,  /*!< 默认设置命令，在创建设备时其次执行 */
} xf_hal_dac_cmd_t;

/**
 * @brief 用于对接 dac 设置的参数。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef struct _xf_hal_dac_config_t {
    bool enable         : 1;    /*!< 启用参数，0为禁用，1为启用 */
    uint32_t resolution : 5;    /*!< 分辨率参数，对应的数值最大值为 `2^resolution - 1` */
    uint32_t speed      : 26;   /*!< 默认参数命令，在创建设备时优先执行 */
    uint32_t value_max;         /*!< 默认参数命令，在创建设备时优先执行 */
    uint32_t verf_mv;           /*!< 默认参数命令，在创建设备时优先执行 */
} xf_hal_dac_config_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief dac 初始化。
 *
 * 使用 dac 之前必须要进行初始化
 *
 * @param dac_num dac 的序号
 * @return xf_err_t
 *      - XF_OK 成功初始化
 *      - other 初始化失败
 */
xf_err_t xf_hal_dac_init(xf_dac_num_t dac_num);

/**
 * @brief dac 反初始化。
 *
 * 恢复到未初始化状态且回收相关资源
 *
 * @param dac_num dac 的序号
 * @return xf_err_t
 *      - XF_OK 成功反初始化
 *      - other 反初始化失败
 */
xf_err_t xf_hal_dac_deinit(xf_dac_num_t dac_num);

/**
 * @brief dac 启用函数。
 *
 * dac 配置好功能后，可以调用 dac 进行激活。
 *
 * @note 激活后，方可使用 xf_hal_dac_write 等函数。激活之前的配置
 *       是静态配置。激活之后的配置属于动态配置，
 * @attention 不一定所有的芯片都支持动态配置
 *
 * @param dac_num dac 的序号
 * @return xf_err_t
 *      - XF_OK 成功启用 dac
 *      - other dac 启用失败
 */
xf_err_t xf_hal_dac_enable(xf_dac_num_t dac_num);

/**
 * @brief dac 禁用函数。
 *
 * @note 如果 dac 正在启用状态，且不支持动态配置。则需要该函数，
 *       先将 dac 禁用，然后进行配置。再次进行启用。
 *
 * @param dac_num dac 的序号
 * @return xf_err_t
 *      - XF_OK 成功禁用 dac
 *      - other 禁用 DAC失败
 */
xf_err_t xf_hal_dac_disable(xf_dac_num_t dac_num);

/**
 * @brief dac 设置分辨率。
 *
 * @note 并不是所有芯片都支持分辨率设置。
 *
 * @param dac_num dac 的序号
 * @param resolution 如果为 10，则输出的DAC值的范围为 0~2^10 之间
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - other 设置失败
 */
xf_err_t xf_hal_dac_set_resolution(xf_dac_num_t dac_num, uint8_t resolution);

/**
 * @brief 设置 dac 输出速度。
 *
 * @note 并不是所有芯片都支持输出速度设置。
 *
 * @param dac_num dac 的序号
 * @param speed 设置 dac 的速度，单位为 hz
 * @return xf_err_t
 *      - XF_OK 成功设置
 *      - other 设置失败
 */
xf_err_t xf_hal_dac_set_speed(xf_dac_num_t dac_num, uint32_t speed);

/**
 * @brief dac 输出电压
 *
 * @param dac_num dac 的序号
 * @param value 设置 dac 原始的值，最大值由 resolution 决定
 * @return xf_err_t
 *      - XF_OK 成功输出电压
 *      - other 输出电压失败
 */
xf_err_t xf_hal_dac_write(xf_dac_num_t dac_num, uint32_t value);

/**
 * @brief dac 输出电压 mv
 *
 * @param dac_num dac 的序号
 * @param mv 设置 dac 输出的电压大小，最大不会超过 verf_mv
 * @return xf_err_t
 *      - XF_OK 成功输出电压
 *      - other 输出电压失败
 */
xf_err_t xf_hal_dac_write_mv(xf_dac_num_t dac_num, uint32_t mv);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // XF_HAL_DAC_IS_ENABLE

/**
 * End of group_xf_hal_user_dac
 * @}
 */

#endif // __XF_HAL_DAC_H__

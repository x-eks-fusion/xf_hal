/**
 * @file xf_hal_adc.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 的 adc(模数转换) 设备抽象。
 * @version 0.1
 * @date 2024-05-13
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_HAL_ADC_H__
#define __XF_HAL_ADC_H__

/* ==================== [Includes] ========================================== */

#include "xf_hal_device_config.h"

/**
 * @ingroup group_xf_hal_user
 * @defgroup group_xf_hal_user_adc adc
 * @brief 模拟数字转换器。
 * @{
 */

#if XF_HAL_ADC_IS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/**
 * @brief adc 序号。
 */
typedef uint32_t xf_adc_num_t;

/**
 * @brief adc 命令枚举。
 *
 * 用于对接 adc 设置的命令。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef enum _xf_hal_adc_cmd_t {
    XF_HAL_ADC_CMD_DEFAULT      = 0x0,          /*!< 默认参数命令，在创建设备时优先执行 */

    XF_HAL_ADC_CMD_ENABLE       = 0x1 << 0,     /*!< 启用命令，见 @ref xf_hal_adc_config_t.enable */
    XF_HAL_ADC_CMD_RESOLUTION   = 0x1 << 1,     /*!< 分辨率命令，见 @ref xf_hal_adc_config_t.resolution */
    XF_HAL_ADC_CMD_SAMPLE_RATE  = 0x1 << 2,     /*!< 采样率命令，见 @ref xf_hal_adc_config_t.sample_rate */

    XF_HAL_ADC_CMD_ALL          = 0x7FFFFFFF, /*!< 默认设置命令，在创建设备时其次执行 */
} xf_hal_adc_cmd_t;

/**
 * @brief adc 配置结构体。
 *
 * 用于对接 adc 设置的参数。
 *
 * @note 普通用户可忽略，移植者需注意。
 */
typedef struct _xf_hal_adc_config_t {
    uint32_t enable         : 1;    /*!< 启用参数，0 为禁用，1 为启用 */
    uint32_t resolution     : 5;    /*!< 分辨率参数，对应的数值最大值为 `2^resolution - 1` */
    uint32_t sample_rate    : 26;   /*!< 采样率参数，1s 中采样的频率，单位为 hz */
} xf_hal_adc_config_t;

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief adc 初始化函数。
 *
 * 使用 adc 之前必须要进行初始化。
 *
 * @param adc_num adc 的序号
 * @return xf_err_t
 *      - XF_OK     成功初始化
 *      - other     初始化失败
 */
xf_err_t xf_hal_adc_init(xf_adc_num_t adc_num);

/**
 * @brief adc 反初始化函数。
 *
 * 恢复到未初始化状态，且回收内部申请的相关资源。
 *
 * @param adc_num adc 的序号
 * @return xf_err_t
 *      - XF_OK             成功反初始化
 *      - XF_ERR_UNINIT    该 adc 未初始化
 *      - other             反初始化失败
 */
xf_err_t xf_hal_adc_deinit(xf_adc_num_t adc_num);

/**
 * @brief adc 启用函数。
 *
 * ADC 配置好功能后，可以调用 adc 启用函数进行激活。
 *
 * @note 激活后，方可使用 `xf_hal_adc_read_raw()` 函数。
 *       激活之前的配置是静态配置。
 *       激活之后的配置属于动态配置，不一定所有的芯片都支持动态配置。
 *
 * @param adc_num adc 的序号
 * @return xf_err_t
 *      - XF_OK             成功启用
 *      - XF_ERR_UNINIT    该 adc 未初始化
 *      - other             启用失败
 */
xf_err_t xf_hal_adc_enable(xf_adc_num_t adc_num);

/**
 * @brief adc 禁用函数。
 *
 * @note 如果 adc 正在启用状态，且不支持动态配置，
 *       则需要使用该函数禁用 adc 后在此配置并用 `xf_hal_adc_enable()` 启用。
 *
 * @param adc_num adc 的序号
 * @return xf_err_t
 *      - XF_OK             成功禁用
 *      - XF_ERR_UNINIT    该 adc 未初始化
 *      - other             禁用失败
 */
xf_err_t xf_hal_adc_disable(xf_adc_num_t adc_num);

/**
 * @brief adc 设置分辨率。
 *
 * @note 不是所有芯片都支持分辨率设置。
 *
 * @param adc_num adc 的序号
 * @param resolution 如果为 10，则获取到的值的范围为 0 ~ 2^10 之间
 * @return xf_err_t
 *      - XF_OK                     设置成功
 *      - XF_ERR_UNINIT            该 adc 未初始化
 *      - XF_ERR_NOT_SUPPORTED      不支持该功能
 *      - other                     设置失败
 */
xf_err_t xf_hal_adc_set_resolution(xf_adc_num_t adc_num, uint8_t resolution);

/**
 * @brief adc 设置采样率。
 *
 * @note 并不是所有芯片都支持采样率设置。
 *
 * @param adc_num adc 的序号
 * @param sample_rate 采样率，1s 中所采集 adc 的次数，单位为 hz
 * @return xf_err_t
 *      - XF_OK                     设置成功
 *      - XF_ERR_UNINIT            该 adc 未初始化
 *      - XF_ERR_NOT_SUPPORTED      不支持该功能
 *      - other                     设置失败
 */
xf_err_t xf_hal_adc_set_sample_rate(xf_adc_num_t adc_num, uint32_t sample_rate);

/**
 * @brief adc读取的原始值,最大值为 2^resolution
 *
 * @param adc_num adc 的序号
 * @return uint32_t 获取的 adc 数值
 */
uint32_t xf_hal_adc_read_raw(xf_adc_num_t adc_num);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // XF_HAL_ADC_IS_ENABLE

/**
 * End of group_xf_hal_user_adc
 * @}
 */

#endif // __XF_HAL_ADC_H__

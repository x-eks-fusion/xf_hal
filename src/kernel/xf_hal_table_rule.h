/**
 * @file xf_hal_table_rule.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief xf_hal 内部设备生成规则。
 * @version 0.1
 * @date 2024-05-09
 * @note 此文件不需要防止重复包含。
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 * @details 用法：
 * 在包含本文件前定义 `XF_HAL_TABLE_TYPE` 或 `XF_HAL_TABLE_STR` 可以
 * 生成枚举值或生成字符串。
 */

/* ==================== [Includes] ========================================== */

/* ==================== [Defines] =========================================== */

#ifdef XF_HAL_REG
#   undef XF_HAL_REG
#endif

// 用于生成设备类型枚举
#ifdef XF_HAL_TABLE_TYPE
#   define XF_HAL_REG(dev_name) XF_HAL_##dev_name,
#endif

// 用于生成设备字符串
#ifdef XF_HAL_TABLE_STR
#   define XF_TO_STR(x) #x
#   define XF_HAL_REG(dev_name) XF_TO_STR(dev_name),
#endif

#undef XF_HAL_TABLE_TYPE
#undef XF_HAL_TABLE_STR

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

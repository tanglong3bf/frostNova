#pragma once

/**
 *  @defgroup 错误码定义
 *  @{
 */
/**
 *  @brief 参数错误
 *  这里的参数指的是接口参数
 *  @{
 */
constexpr int PARAMETER_ERROR = 2000;     ///< 参数错误
constexpr int PARAM_MISSING = 2001;       ///< 缺少必备参数
constexpr int PARAM_FORMAT_ERROR = 2002;  ///< 参数格式错误 json/form-data
constexpr int INVALID_PARAM_VALUE = 2003;  ///< 参数值无效
constexpr int PARAM_TYPE_MISMATCH = 2004;  ///< 参数类型不匹配 string/integer
constexpr int PARAM_LENGTH_ERROR = 2005;  ///< 参数长度不在指定范围内
/** @} */
/**
 *  @brief 权限错误
 *  @{
 */
constexpr int AUTHORITY_ERROR = 3000;  ///< 权限错误
constexpr int INVALID_TOKEN = 3001;    ///< 无效的token
constexpr int TOKEN_EXPIRED = 3002;    ///< token过期
/** @} */
/**
 *  @brief 数据库错误
 *  @{
 */
constexpr int DATABASE_ERROR = 4000;  ///< 数据库错误
/** @} */
/**
 *  @brief 业务错误
 *  @{
 */
constexpr int BUSINESS_ERROR = 5000;    ///< 业务错误
constexpr int USER_NOT_EXITS = 5001;    ///< 用户不存在
constexpr int USER_IS_EXISTS = 5002;    ///< 用户已存在
constexpr int PASSWORD_ERROR = 5003;    ///< 密码错误
constexpr int USER_IS_DISABLED = 5004;  ///< 用户被禁用
/** @} */
/** @} */

/*!
 * @file  string_utility.h
 * @brief some string utility functions.
 * @date  2014-02-06
 */
#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H

#include <string>

/*!
 * @namespace openrtm_network_camera
 * @brief OpenRTM NetworkCamera用コンポーネント
 *
 */
namespace openrtm_network_camera {

/*!
 * @namespace utility
 * @brief 共通処理
 *
 */
namespace utility {

/*!
 * @brief 文字列を小文字に変換する。
 *
 * @param target  処理対象文字列
 * @return 小文字に変換した文字列
 */
std::string convertToLower(const std::string& target);

/*!
 * @brief 文字列の前後より空白を削除する。
 *
 * @param target  処理対象文字列
 * @param trimCharacters 削除対象の空白文字
 * @return 空白削除後の文字列
 */
std::string trim(const std::string& target, const char* trimCharacters = " \t\v\r\n");

/*!
 * @brief base64エンコード後の文字列を取得する。
 *
 * @param target  処理対象文字列
 * @return base64エンコード後の文字列
 */
std::string base64encode(const std::string& target);

} // utility
} // openrtm_network_camera

#endif // STRING_UTILITY_H

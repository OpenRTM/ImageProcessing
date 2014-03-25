/*!
 * @file  string_utility.cpp
 * @brief some string utility functions.
 * @date  2014-02-06
 */
#include "string_utility.h"

#include <algorithm>
#include <string>
#include <sstream>

#include <boost/serialization/pfto.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace openrtm_network_camera {
namespace utility {

std::string convertToLower(const std::string& target) {
  std::string t = target;
  std::transform(t.begin(), t.end(), t.begin(), ::tolower);
  return t;
}

std::string trim(const std::string& target, const char* trimCharacters) {
  std::string result("");

  // 左側の空白以外の文字位置
  std::string::size_type left = target.find_first_not_of(trimCharacters);

  if (std::string::npos != left) {
    // 右側の空白以外の文字位置
    std::string::size_type right = target.find_last_not_of(trimCharacters);

    result = target.substr(left, right - left + 1);
  }
  return result;
}

/*!
 * @brief base64エンコーディングに用いる定数定義
 */
namespace {
  const std::string base64_padding[] = {"", "==", "="};
}

/*!
 * @brief base64エンコード後の文字列を取得する。
 *
 * Base64エンコーディングについて
 * @see http://ja.wikipedia.org/wiki/Base64
 *
 * BoostでのBase64エンコーディング実装方法について
 * @see http://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
 * @see http://d.hatena.ne.jp/amachang/20090325/1237960531
 * @see http://www.boost.org/doc/libs/1_55_0/libs/serialization/doc/dataflow.html
 *
 * @param target  処理対象文字列
 * @return base64エンコード後の文字列
 */
std::string base64encode(const std::string& target) {
  using boost::archive::iterators::base64_from_binary;
  using boost::archive::iterators::transform_width;

  typedef base64_from_binary<transform_width<const char *, 6, 8> > base64_encorder;

  std::stringstream ss;
  std::copy(
    base64_encorder(BOOST_MAKE_PFTO_WRAPPER(target.c_str())),
    base64_encorder(BOOST_MAKE_PFTO_WRAPPER(target.c_str() + target.size())),
    std::ostream_iterator<char>(ss)
    );

  // Base64コーディングにあわせて４文字単位になるようにパディングを行う
  // パディング文字数の決め方は下記のとおり考えればよい
  //
  // 対象文字列の文字数をnとすると
  //   8n/6 = 4n/3    : 6bit単位に分割した際の文字数（要切り上げ）
  //   (4n/3)/4 = n/3 : 4文字単位にまとめたグループの数（要切り上げ）
  //   4*1/3 = 1.33.. : グループ数の余りが１の場合の文字数（要切り上げ、即ち２文字）
  //   4*2/3 = 2.66.. : グループ数の余りが２の場合の文字数（要切り上げ、即ち１文字）
  ss << base64_padding[target.size() % 3];

  return ss.str();
}

} // utility
} // openrtm_network_camera

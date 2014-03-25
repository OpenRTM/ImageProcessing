/*!
 * @file  IntBounds.h
 * @brief bounds for int.
 * @date  2014-02-09
 */
#ifndef INT_BOUNDS_H
#define INT_BOUNDS_H

/*!
 * @namespace openrtm_network_camera
 * @brief OpenRTM NetworkCamera用コンポーネント
 *
 */
namespace openrtm_network_camera {

/*!
 * @namespace sony
 * @brief Sonyカメラ
 *
 */
namespace sony {

/*!
 * @brief int型の境界クラス
 *
 * int型整数に対する境界を表すクラス
 * テンプレートパラメータで示した範囲を超える数値は、デフォルト値に
 * 置き換える。
 */
template<int lower, int upper, int defaultValue>
class IntBounds {
public:
  IntBounds() {
    if ((lower > upper) || (defaultValue < lower) || (upper < defaultValue)) {
      throw std::exception();
    }
  }
  ~IntBounds() {}
  int operator()(const int val) {
    if (val < lower) return defaultValue;
    if (val > upper) return defaultValue;
    return val;
  }
};

} // sony
} // openrtm_network_camera

#endif

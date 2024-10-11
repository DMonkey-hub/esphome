#pragma once

#include "esphome/core/helpers.h"

namespace esphome {
namespace axs15231 {

static const uint8_t AXS_TOUCH_ONE_POINT_LEN = 0x06;
static const uint8_t AXS_TOUCH_BUF_HEAD_LEN  = 0x02;

static const uint8_t AXS_TOUCH_GESTURE_POS = 0x00;
static const uint8_t AXS_TOUCH_POINT_NUM   = 0x01;
static const uint8_t AXS_TOUCH_EVENT_POS   = 0x02;
static const uint8_t AXS_TOUCH_XPOSH       = 0x02;
static const uint8_t AXS_TOUCH_XPOSL       = 0x03;
static const uint8_t AXS_TOUCH_YPOSH       = 0x04;
static const uint8_t AXS_TOUCH_YPOSL       = 0x05;
static const uint8_t AXS_TOUCH_WEIGHT_POS  = 0x06;
static const uint8_t AXS_TOUCH_AREA_POS    = 0x07;
static const uint8_t AXS_TOUCH_DATA_SIZE   = 0x08;

#define AXS_GET_POINT_NUM(buf) buf[AXS_TOUCH_POINT_NUM]
#define AXS_GET_GESTURE_TYPE(buf)  buf[AXS_TOUCH_GESTURE_POS]
#define AXS_GET_POINT_X(buf,point_index) (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_XPOSH] & 0x0F) <<8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_XPOSL])
#define AXS_GET_POINT_Y(buf,point_index) (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_YPOSH] & 0x0F) <<8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_YPOSL])
#define AXS_GET_POINT_EVENT(buf,point_index) (buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_EVENT_POS] >> 6)

#define I2C_ERROR_CHECK(err) \
  if ((err) != i2c::ERROR_OK) { \
    ESP_LOGE(TAG, "Failed to communicate!"); \
    this->status_set_warning(); \
    return; \
  }

}  // namespace axs15231
}  // namespace esphome

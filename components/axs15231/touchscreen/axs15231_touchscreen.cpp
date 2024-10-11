#include "axs15231_touchscreen.h"
#include "axs15231_defines.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace axs15231 {

namespace {
  constexpr static const char *const TAG = "axs15231.touchscreen";
  constexpr static const uint8_t AXS_READ_TOUCHPAD[11] = {0xb5, 0xab, 0xa5, 0x5a, 0x0, 0x0, 0x0, 0x8};
} // anonymous namespace

void AXS15231Touchscreen::setup() {
  ESP_LOGCONFIG(TAG, "Setting up AXS15231 Touchscreen...");

  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();
    this->reset_pin_->digital_write(true);
    delay(2);
    this->reset_pin_->digital_write(false);
    delay(10);
    this->reset_pin_->digital_write(true);
    delay(2);
  }

  this->x_raw_max_ = this->display_->get_width();
  this->y_raw_max_ = this->display_->get_height();
  ESP_LOGCONFIG(TAG, "AXS15231 Touchscreen setup complete");
}

void AXS15231Touchscreen::update_touches() {
  i2c::ErrorCode err;
  bool touched = false;
  uint8_t buff[AXS_TOUCH_DATA_SIZE];
  u_int16_t x, y, tempx;

  err = this->write(AXS_READ_TOUCHPAD, sizeof(AXS_READ_TOUCHPAD), false);
  I2C_ERROR_CHECK(err);

  err = this->read(buff, AXS_TOUCH_DATA_SIZE);
  I2C_ERROR_CHECK(err);

  x = AXS_GET_POINT_X(buff, 0);
  y = AXS_GET_POINT_Y(buff, 0);

  if ((x == 0 && y == 0) || AXS_GET_GESTURE_TYPE(buff) != 0) {
    return;
  }

  // DMonkey - moded to support rotation 90/180 - (0/270 both ok)
  if (this->swap_x_y_) {
	  tempx = x;
	  x = y;
	  y = tempx;
  }
  // end of mod

  this->add_raw_touch_position_(0, x, y);
}

void AXS15231Touchscreen::dump_config() {
  ESP_LOGCONFIG(TAG, "AXS15231 Touchscreen:");
  LOG_I2C_DEVICE(this);
  LOG_PIN(" Reset Pin: ", this->reset_pin_);
  ESP_LOGCONFIG(TAG, "  X min: %d", this->x_raw_min_);
  ESP_LOGCONFIG(TAG, "  X max: %d", this->x_raw_max_);
  ESP_LOGCONFIG(TAG, "  Y min: %d", this->y_raw_min_);
  ESP_LOGCONFIG(TAG, "  Y max: %d", this->y_raw_max_);

  ESP_LOGCONFIG(TAG, "  Swap X/Y: %s", YESNO(this->swap_x_y_));
  ESP_LOGCONFIG(TAG, "  Invert X: %s", YESNO(this->invert_x_));
  ESP_LOGCONFIG(TAG, "  Invert Y: %s", YESNO(this->invert_y_));
}

}  // namespace axs15231
}  // namespace esphome

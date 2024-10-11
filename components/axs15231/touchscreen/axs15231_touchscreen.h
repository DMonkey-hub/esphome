#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/touchscreen/touchscreen.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace axs15231 {

class AXS15231Touchscreen : public touchscreen::Touchscreen, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;

  void set_reset_pin(GPIOPin *pin) {
    this->reset_pin_ = pin;
  }

 protected:
  void update_touches() override;

  GPIOPin *reset_pin_{};
};

}  // namespace axs15231
}  // namespace esphome

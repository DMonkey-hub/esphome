#pragma once

#ifdef USE_ESP_IDF

#include "esphome/core/component.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/display/display.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome {
namespace axs15231 {

class AXS15231Display : public display::DisplayBuffer,
                        public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW,
                                              spi::CLOCK_PHASE_LEADING, spi::DATA_RATE_20MHZ> {
 public:
  void update() override;

  float get_setup_priority() const override;

  void setup() override;

  bool can_proceed() override;

  void dump_config() override;

  // Display overrides

  /// Fill the entire screen with the given color.
  virtual void fill(Color color);

  // Get the type of display that the buffer corresponds to.
  display::DisplayType get_display_type() override;

  // DisplayBuffer overrides
  int get_width_internal() override;
  int get_height_internal() override;
  uint32_t get_buffer_length_();

  void set_reset_pin(GPIOPin *reset_pin);

  void set_backlight_pin(GPIOPin *backlight_pin);

  void set_width(uint16_t width);

  void set_dimensions(uint16_t width, uint16_t height);

  void set_mirror_x(bool mirror_x);

  void set_mirror_y(bool mirror_y);

  void set_swap_xy(bool swap_xy);

  void set_brightness(uint8_t brightness);

  void set_offsets(int16_t offset_x, int16_t offset_y);

 protected:
  void setup_pins_();

  void set_madctl_();

  void init_lcd_();

  void reset_();

  void display_();

  void invalidate_();

  void write_command_(uint8_t cmd, const uint8_t *bytes, size_t len);

  void write_command_(uint8_t cmd, uint8_t data);

  void write_command_(uint8_t cmd);

  void set_addr_window_(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

  void draw_absolute_pixel_internal(int x, int y, Color color) override;

 private:
  GPIOPin *reset_pin_{nullptr};
  GPIOPin *backlight_pin_{nullptr};
  uint16_t x_low_{0};
  uint16_t y_low_{0};
  uint16_t x_high_{0};
  uint16_t y_high_{0};
  bool setup_complete_{};

  bool prossing_update_ = false;
  bool need_update_ = false;

  size_t width_{};
  size_t height_{};
  int16_t offset_x_{0};
  int16_t offset_y_{0};
  bool swap_xy_{};
  bool mirror_x_{};
  bool mirror_y_{};
  uint8_t brightness_{0xD0};
};

}  // namespace axs15231
}  // namespace esphome

#endif

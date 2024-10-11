#include "axs15231_display.h"
#include "axs15231_defines.h"

#include "esphome/core/log.h"
#include "esphome/components/display/display_color_utils.h"

#ifdef USE_ESP_IDF

namespace esphome {
namespace axs15231 {

namespace {
  constexpr static const char *const TAG = "axs15231.display";

  typedef struct
  {
      uint8_t cmd;
      uint8_t data[36];
      uint8_t len;
  } lcd_cmd_t;

  const static lcd_cmd_t AXS_QSPI_INIT_NEW[] = {
    {AXS_LCD_DISPOFF, {0x00}, 0x40},
    {AXS_LCD_SLPIN,   {0x00}, 0x80},
    {AXS_LCD_SLPOUT,  {0x00}, 0x80},
    {AXS_LCD_DISPON,  {0x00}, 0x00},
  };

  // store a 16 bit value in a buffer, big endian.
  static inline void put16_be(uint8_t *buf, uint16_t value) {
    buf[0] = value >> 8;
    buf[1] = value;
  }
}  // anonymous namespace

void AXS15231Display::update() {
  if (this->prossing_update_) {
    this->need_update_ = true;
    return;
  }

  this->prossing_update_ = true;
  do {
    this->need_update_ = false;
    this->do_update_();
  } while (this->need_update_);
  this->prossing_update_ = false;
  this->display_();
}

float AXS15231Display::get_setup_priority() const {
  return setup_priority::HARDWARE;
}

void AXS15231Display::setup() {
  ESP_LOGCONFIG(TAG, "setting up axs15231");

  ESP_LOGI(TAG, "setup pins");
  this->setup_pins_();
  ESP_LOGI(TAG, "setup lcd");
  this->init_lcd_();

  ESP_LOGI(TAG, "set madctl");
  this->set_madctl_();
  this->invalidate_();

  ESP_LOGI(TAG, "init internal buffer");
  this->init_internal_(this->get_buffer_length_());
  if (this->buffer_ == nullptr) {
    this->mark_failed();
  }

  ESP_LOGI(TAG, "set brightness");
  this->write_command_(AXS_LCD_WRDISBV, &this->brightness_, 1);

  this->setup_complete_ = true;
  ESP_LOGCONFIG(TAG, "axs15231 setup complete");
}

bool AXS15231Display::can_proceed() {
  return this->setup_complete_;
}

void AXS15231Display::dump_config() {
  ESP_LOGCONFIG("", "AXS15231 Display");
  ESP_LOGCONFIG(TAG, "  Height: %u", this->height_);
  ESP_LOGCONFIG(TAG, "  Width: %u", this->width_);
  LOG_PIN("  CS Pin: ", this->cs_);
  LOG_PIN("  Reset Pin: ", this->reset_pin_);
  ESP_LOGCONFIG(TAG, "  SPI Data rate: %dMHz", (unsigned) (this->data_rate_ / 1000000));
#ifdef USE_POWER_SUPPLY
  ESP_LOGCONFIG(TAG, "  Power Supply Configured: yes");
#endif
}

void AXS15231Display::fill(Color color) {
  uint16_t new_color = 0;
  this->x_low_ = 0;
  this->y_low_ = 0;
  this->x_high_ = this->get_width_internal() - 1;
  this->y_high_ = this->get_height_internal() - 1;

  new_color = display::ColorUtil::color_to_565(color);
  if (((uint8_t) (new_color >> 8)) == ((uint8_t) new_color)) {
    // Upper and lower is equal can use quicker memset operation
    memset(this->buffer_, (uint8_t) new_color, this->get_buffer_length_());
  } else {
    // Slower set of both buffers
    for (uint32_t i = 0; i < this->get_buffer_length_(); i = i + 2) {
      this->buffer_[i] = (uint8_t) (new_color >> 8);
      this->buffer_[i + 1] = (uint8_t) new_color;
    }
  }
}

display::DisplayType AXS15231Display::get_display_type() {
  return display::DisplayType::DISPLAY_TYPE_COLOR;
}

uint32_t AXS15231Display::get_buffer_length_() {
  return this->get_width_internal() * this->get_height_internal() * 2;
}

int AXS15231Display::get_width_internal() {
  return this->width_;
}

int AXS15231Display::get_height_internal() {
  return this->height_;
}

void AXS15231Display::set_reset_pin(GPIOPin *reset_pin) {
  this->reset_pin_ = reset_pin;
}

void AXS15231Display::set_backlight_pin(GPIOPin *backlight_pin) {
  this->backlight_pin_ = backlight_pin;
}

void AXS15231Display::set_width(uint16_t width) {
  this->width_ = width;
}

void AXS15231Display::set_dimensions(uint16_t width, uint16_t height) {
  this->width_ = width;
  this->height_ = height;
}

void AXS15231Display::set_mirror_x(bool mirror_x) {
  this->mirror_x_ = mirror_x;
}

void AXS15231Display::set_mirror_y(bool mirror_y) {
  this->mirror_y_ = mirror_y;
}

void AXS15231Display::set_swap_xy(bool swap_xy) {
  this->swap_xy_ = swap_xy;
}

void AXS15231Display::set_brightness(uint8_t brightness) {
  this->brightness_ = brightness;

  if (this->setup_complete_) {
    this->write_command_(AXS_LCD_WRDISBV, &this->brightness_, 1);
  }
}

void AXS15231Display::set_offsets(int16_t offset_x, int16_t offset_y) {
  this->offset_x_ = offset_x;
  this->offset_y_ = offset_y;
}

void AXS15231Display::setup_pins_() {
  if (this->backlight_pin_ != nullptr) {
    this->backlight_pin_->setup();
    this->backlight_pin_->digital_write(true);
  }

  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();
    this->reset_pin_->digital_write(true);
  }

  this->spi_setup();

  this->reset_();
}

void AXS15231Display::set_madctl_() {
// custom x/y transform and color order
  uint8_t mad = MADCTL_RGB;
  // TODO(buglloc): MADCTL_MV is broken
  // if (this->swap_xy_)
  //   mad |= MADCTL_MV;
  if (this->mirror_x_)
    mad |= MADCTL_MX;
  if (this->mirror_y_)
    mad |= MADCTL_MY;

  this->write_command_(AXS_LCD_MADCTL, &mad, 1);
  ESP_LOGD(TAG, "wrote MADCTL 0x%02X", mad);
}

void AXS15231Display::init_lcd_() {
  const lcd_cmd_t *lcd_init = AXS_QSPI_INIT_NEW;
  for (int i = 0; i < sizeof(AXS_QSPI_INIT_NEW) / sizeof(lcd_cmd_t); ++i) {
    this->write_command_(lcd_init[i].cmd, (uint8_t *)lcd_init[i].data, lcd_init[i].len & 0x3f);
    if (lcd_init[i].len & 0x80)
        delay(150);
    if (lcd_init[i].len & 0x40)
        delay(20);
  }
}

void AXS15231Display::reset_() {
  if (this->reset_pin_  == nullptr) {
    return;
  }

  this->reset_pin_->digital_write(true);
  delay(20);
  this->reset_pin_->digital_write(false);
  delay(20);
  this->reset_pin_->digital_write(true);
  delay(20);
}

// definition from esphome -
// void 	write_cmd_addr_data (size_t cmd_bits, uint32_t cmd, size_t addr_bits, uint32_t address, const uint8_t *data, size_t length, uint8_t bus_width=1)

void AXS15231Display::write_command_(uint8_t cmd, const uint8_t *bytes, size_t len) {
  this->enable();
  this->write_cmd_addr_data(8, 0x02, 24, cmd << 8, bytes, len);
  this->disable();
}

void AXS15231Display::write_command_(uint8_t cmd, uint8_t data) { this->write_command_(cmd, &data, 1); }

void AXS15231Display::write_command_(uint8_t cmd) { this->write_command_(cmd, &cmd, 0); }

void AXS15231Display::set_addr_window_(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  uint8_t buf[4];
  x1 += this->offset_x_;
  x2 += this->offset_x_;
  y1 += this->offset_y_;
  y2 += this->offset_y_;
  put16_be(buf, x1);
  put16_be(buf + 2, x2);
  this->write_command_(AXS_LCD_CASET, buf, sizeof(buf));
  put16_be(buf, y1);
  put16_be(buf + 2, y2);
  this->write_command_(AXS_LCD_RASET, buf, sizeof(buf));
}

void AXS15231Display::display_() {
  if ((this->x_high_ < this->x_low_) || (this->y_high_ < this->y_low_)) {
    return;
  }

  // we will only update the changed rows to the display
  // ############ DMonkey - force the next 2 lines to full width and height
  size_t const w = this->width_; //this->x_high_ - this->x_low_ + 1;
  size_t const h = this->height_; //this->y_high_ - this->y_low_ + 1;
  size_t const x_pad = this->get_width_internal() - w - this->x_low_;
  // ############ DMonkey - force the window to full screen
  this->set_addr_window_(1, 1, this->width_, this->height_);
  // this->set_addr_window_(this->x_low_, this->y_low_, this->x_high_, this->y_high_);

  this->enable();

  // ############ DMonkey - force the next line to always draw the full screen
  if (true) { //(this->x_low_ == 0 && this->y_low_ == 0 && x_pad == 0) {
      this->write_cmd_addr_data(8, 0x32, 24, 0x2C00, this->buffer_, w * h * 2, 4);
  } else {
    this->write_cmd_addr_data(8, 0x32, 24, 0x2C00, nullptr, 0, 4);
    size_t stride = this->x_low_ + w + x_pad;
    for (int y = 0; y != h; y++) {
      size_t offset = ((y + this->y_low_) * stride + this->x_low_);
      this->write_cmd_addr_data(0, 0, 0, 0, this->buffer_ + offset * 2, w * 2, 4);
    }
  }

  this->disable();

  this->invalidate_();
}

void AXS15231Display::invalidate_() {
  // invalidate watermarks
  this->x_low_ = this->width_;
  this->y_low_ = this->height_;
  this->x_high_ = 0;
  this->y_high_ = 0;
}

void AXS15231Display::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (x < 0 || x >= this->get_width_internal() || y < 0 || y >= this->get_height_internal()) {
    ESP_LOGW(TAG, "tring to draw invalid pixel: x(0 <= %d < %d) && y(0 <= %d < %d)", x, this->get_width_internal(), y,
               this->get_height_internal());
    return;
  }

  uint32_t pos = (y * width_) + x;
  uint16_t new_color;
  bool updated = false;

  pos = pos * 2;
  new_color = display::ColorUtil::color_to_565(color, display::ColorOrder::COLOR_ORDER_RGB);
  if (this->buffer_[pos] != (uint8_t) (new_color >> 8)) {
    this->buffer_[pos] = (uint8_t) (new_color >> 8);
    updated = true;
  }
  pos = pos + 1;
  new_color = new_color & 0xFF;

  if (this->buffer_[pos] != new_color) {
    this->buffer_[pos] = new_color;
    updated = true;
  }

  if (updated) {
    // low and high watermark may speed up drawing from buffer
    if (x < this->x_low_)
      this->x_low_ = x;
    if (y < this->y_low_)
      this->y_low_ = y;
    if (x > this->x_high_)
      this->x_high_ = x;
    if (y > this->y_high_)
      this->y_high_ = y;
  }
}

#endif
}  // namespace axs15231
}  // namespace esphome

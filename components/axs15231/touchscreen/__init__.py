import esphome.codegen as cg
import esphome.config_validation as cv

from esphome import pins
from esphome.components import i2c, touchscreen
from esphome.const import (
  CONF_ID,
  CONF_RESET_PIN
)
from .. import axs15231_ns


DEPENDENCIES = ["i2c"]

AXS15231Touchscreen = axs15231_ns.class_(
    "AXS15231Touchscreen",
    touchscreen.Touchscreen,
    i2c.I2CDevice,
)

CONFIG_SCHEMA = touchscreen.TOUCHSCREEN_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(AXS15231Touchscreen),
        cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema,
    }
).extend(i2c.i2c_device_schema(0x3B))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await touchscreen.register_touchscreen(var, config)
    await i2c.register_i2c_device(var, config)

    if reset_pin := config.get(CONF_RESET_PIN):
        cg.add(var.set_reset_pin(await cg.gpio_pin_expression(reset_pin)))

import esphome.codegen as cg
import esphome.config_validation as cv

from esphome import pins
from esphome.components import (
    spi,
    display,
)
from esphome.const import (
    CONF_RESET_PIN,
    CONF_OUTPUT,
    CONF_ID,
    CONF_DIMENSIONS,
    CONF_WIDTH,
    CONF_HEIGHT,
    CONF_LAMBDA,
    CONF_BRIGHTNESS,
    CONF_BACKLIGHT_PIN,
    CONF_OFFSET_HEIGHT,
    CONF_OFFSET_WIDTH,
    CONF_MIRROR_X,
    CONF_MIRROR_Y,
    CONF_SWAP_XY,
    CONF_TRANSFORM,
)
from .. import axs15231_ns


DEPENDENCIES = ["spi"]

AXS15231Component = axs15231_ns.class_(
    "AXS15231Display", display.Display, display.DisplayBuffer, cg.Component, spi.SPIDevice
)

DATA_PIN_SCHEMA = pins.gpio_pin_schema(
    {
        CONF_OUTPUT: True,
    },
    internal=True,
)

CONFIG_SCHEMA = cv.All(
    display.FULL_DISPLAY_SCHEMA.extend(
        cv.Schema(
            {
                cv.GenerateID(): cv.declare_id(AXS15231Component),
                cv.Required(CONF_DIMENSIONS): cv.Any(
                    cv.dimensions,
                    cv.Schema(
                        {
                            cv.Required(CONF_WIDTH): cv.int_,
                            cv.Required(CONF_HEIGHT): cv.int_,
                            cv.Optional(CONF_OFFSET_HEIGHT, default=0): cv.int_,
                            cv.Optional(CONF_OFFSET_WIDTH, default=0): cv.int_,
                        }
                    ),
                ),
                cv.Optional(CONF_TRANSFORM): cv.Schema(
                    {
                        cv.Optional(CONF_MIRROR_X, default=False): cv.boolean,
                        cv.Optional(CONF_MIRROR_Y, default=False): cv.boolean,
                        cv.Optional(CONF_SWAP_XY, default=False): cv.boolean,
                    }
                ),
                cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema,
                cv.Optional(CONF_BACKLIGHT_PIN): pins.gpio_output_pin_schema,
                cv.Optional(CONF_BRIGHTNESS, default=0xD0): cv.int_range(
                    0, 0xFF, min_included=True, max_included=True
                ),
            }
        ).extend(
            spi.spi_device_schema(
                cs_pin_required=False,
                default_mode="MODE0",
                default_data_rate=20e6,
                quad=True,
            )
        )
    ),
    cv.only_with_esp_idf,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)
    await spi.register_spi_device(var, config)

    cg.add(var.set_brightness(config[CONF_BRIGHTNESS]))
    if backlight_pin := config.get(CONF_BACKLIGHT_PIN):
        backlight = await cg.gpio_pin_expression(backlight_pin)
        cg.add(var.set_backlight_pin(backlight))

    if reset_pin := config.get(CONF_RESET_PIN):
        reset = await cg.gpio_pin_expression(reset_pin)
        cg.add(var.set_reset_pin(reset))

    if transform := config.get(CONF_TRANSFORM):
        cg.add(var.set_mirror_x(transform[CONF_MIRROR_X]))
        cg.add(var.set_mirror_y(transform[CONF_MIRROR_Y]))
        cg.add(var.set_swap_xy(transform[CONF_SWAP_XY]))

    if CONF_DIMENSIONS in config:
        dimensions = config[CONF_DIMENSIONS]
        if isinstance(dimensions, dict):
            cg.add(var.set_dimensions(dimensions[CONF_WIDTH], dimensions[CONF_HEIGHT]))
            cg.add(
                var.set_offsets(
                    dimensions[CONF_OFFSET_WIDTH], dimensions[CONF_OFFSET_HEIGHT]
                )
            )
        else:
            (width, height) = dimensions
            cg.add(var.set_dimensions(width, height))

    if lamb := config.get(CONF_LAMBDA):
        lambda_ = await cg.process_lambda(
            lamb, [(display.DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))

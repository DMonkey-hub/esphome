#pragma once

#include "esphome/core/helpers.h"

namespace esphome {
namespace axs15231 {

static const uint8_t MADCTL_MY  = 0x80;   ///< Bit 7 Bottom to top
static const uint8_t MADCTL_MX  = 0x40;   ///< Bit 6 Right to left
static const uint8_t MADCTL_MV  = 0x20;   ///< Bit 5 Reverse Mode
static const uint8_t MADCTL_RGB = 0x00;  ///< Bit 3 Red-Green-Blue pixel order

static const uint8_t AXS_LCD_NOP      = 0x00; // No operation (C)
static const uint8_t AXS_LCD_SWRESET  = 0x01; // Software reset (C)
static const uint8_t AXS_LCD_RDDID    = 0x04; // Read display (R)
static const uint8_t AXS_LCD_RDNUMED  = 0x05; // Read Number of the Errors on DSI (R)
static const uint8_t AXS_LCD_RDDST    = 0x09; // Read display status (R)
static const uint8_t AXS_LCD_RDDPM    = 0x0A; // Read display power (R)
static const uint8_t AXS_LCD_RDDMADC  = 0x0B; // Read memory data access control (R)
static const uint8_t AXS_LCD_RDDIPF   = 0x0C; // Read Interface Pixel Format (R)
static const uint8_t AXS_LCD_RDDIM    = 0x0D; // Read display image (R)
static const uint8_t AXS_LCD_RDDSM    = 0x0E; // Read display signal (R)
static const uint8_t AXS_LCD_RDDSDR   = 0x0F; // Read display self-diagnostic result (R)
static const uint8_t AXS_LCD_SLPIN    = 0x10; // Sleep in (C)
static const uint8_t AXS_LCD_SLPOUT   = 0x11; // Sleep out (C)
static const uint8_t AXS_LCD_PTLON    = 0x12; // Partial mode on (C)
static const uint8_t AXS_LCD_NORON    = 0x13; // Partial mode off(Normal) (C)
static const uint8_t AXS_LCD_INVOFF   = 0x20; // Display inversion off (C)
static const uint8_t AXS_LCD_INVON    = 0x21; // Display inversion on (C)
static const uint8_t AXS_LCD_ALLPOFF  = 0x22; // All pixel off (C)
static const uint8_t AXS_LCD_ALLPON   = 0x23; // All pixel on (C)
static const uint8_t AXS_LCD_ALLPFILL = 0x24; // All pixel fill given color (W)
static const uint8_t AXS_LCD_GAMSET   = 0x26; // Gamma curve set (W)
static const uint8_t AXS_LCD_DISPOFF  = 0x28; // Display off (C)
static const uint8_t AXS_LCD_DISPON   = 0x29; // Display on (C)
static const uint8_t AXS_LCD_CASET    = 0x2A; // Column address set (W)
static const uint8_t AXS_LCD_RASET    = 0x2B; // Row address set (W)
static const uint8_t AXS_LCD_RAMWR    = 0x2C; // Memory write any length MIPI/SPI/QSPI/DBI (W)
static const uint8_t AXS_LCD_RAMRD    = 0x2E; // Memory read any length SPI/QSPI/DBI (R)
static const uint8_t AXS_LCD_RAWFILL  = 0x2F; // Memory fill given color at window (W)
static const uint8_t AXS_LCD_PTLAR    = 0x30; // Partial start/end address set (W)
static const uint8_t AXS_LCD_PTLARC   = 0x31; // set_partial_columns (W)
static const uint8_t AXS_LCD_VSCRDEF  = 0x33; // Vertical scrolling definition (W)
static const uint8_t AXS_LCD_TEOFF    = 0x34; // Tearing effect line off (C)
static const uint8_t AXS_LCD_TEON     = 0x35; // Tearing effect line on (W)
static const uint8_t AXS_LCD_MADCTL   = 0x36; // Memory data access control (W)
static const uint8_t AXS_LCD_VSCRSADD = 0x37; // Vertical scrolling start address (W)
static const uint8_t AXS_LCD_IDMOFF   = 0x38; // Idle mode off (C)
static const uint8_t AXS_LCD_IDMON    = 0x39; // Idle mode on (C)
static const uint8_t AXS_LCD_IPF      = 0x3A; // Interface pixel format (W)
static const uint8_t AXS_LCD_RAMWRC   = 0x3C; // Memory write continue any length MIPI/SPI/QSPI/DBI (W)
static const uint8_t AXS_LCD_RAMRDC   = 0x3E; // Memory read continue any length SPI/QSPI/DBI (R)
static const uint8_t AXS_LCD_TESCAN   = 0x44; // Set tear scanline (W)
static const uint8_t AXS_LCD_RDTESCAN = 0x45; // Get tear scanline (R)
static const uint8_t AXS_LCD_WRDISBV  = 0x51; // Write display brightness value (W)
static const uint8_t AXS_LCD_RDDISBV  = 0x52; // Read display brightness value (R)
static const uint8_t AXS_LCD_WRCTRLD  = 0x53; // Write CTRL display (W)
static const uint8_t AXS_LCD_RDCTRLD  = 0x54; // Read CTRL dsiplay (R)
static const uint8_t AXS_LCD_RDFCHKSU = 0xAA; // Read First Checksum (R)
static const uint8_t AXS_LCD_RDCCHKSU = 0xAA; // Read Continue Checksum (R)
static const uint8_t AXS_LCD_RDID1    = 0xDA; // Read ID1 (R)
static const uint8_t AXS_LCD_RDID2    = 0xDB; // Read ID2 (R)
static const uint8_t AXS_LCD_RDID3    = 0xDC; // Read ID3 (R)
static const uint8_t AXS_LCD_DSTB     = 0x90; // Enter Deep-Standby (W)

}  // namespace axs15231
}  // namespace esphome

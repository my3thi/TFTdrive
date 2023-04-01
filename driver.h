/* Update:2021-9-13 */

#ifndef __LANDTANK_DRIVER_H
#define __LANDTANK_DRIVER_H

#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <STM32RTC.h>

/* ================================================================================================= */
extern STM32RTC &rtc;
extern Adafruit_ST7735 tft;

extern struct S_Flag flagX;

/* ================================================================================================= */

const struct
{
    uint8_t SW0 = PA0, SW1 = PB9, SW2 = PA15, SW3 = PB10;
} pinKey;
const struct
{
    uint8_t uU24 = PB8, uLight = PB5, uPump = PB4, uUSB = PB3;
} pinMOS;
const struct
{
    uint8_t uU24 = PA5, uLight = PA6, uPump = PA7, uUSB = PA4;
} pinADC;
const struct
{
    const uint8_t TFT_CS = PA10, TFT_DC = PA9, TFT_RST = PA8, LED = PC13;
} pinDev;
const struct
{
    const uint32_t TimeUpdateLast = 19;
} BKPregisterIndex;

const struct
{
    const uint16_t BLACK = ST7735_BLACK,
                   WHITE = ST7735_WHITE,
                   BLUE = 0x001F,
                   GREEN = 0x07E0,
                   RED = 0xF800,
                   YELLOW = ST7735_YELLOW,
                   ORANGE = 0xFD00,
                   CYAN = ST7735_CYAN,
                   VIOLET = 0xF81F,
                   MAGENTA = ST7735_MAGENTA,
                   VividGreen = 0x07EF;
} TFTcolor;

struct S_TFTarea
{
    uint8_t aX, aY, aW, aH; // point X,Y; width,height
};
const struct
{
    const struct S_TFTarea Clock = {0, 0, 24, 32},
                           USB = {0, 40, 24, 16},
                           Pump = {0, 60, 24, 16},
                           Light = {0, 80, 24, 16},
                           U24 = {0, 100, 24, 16};
    const uint8_t ColomnText = 38, ColomnTitle = 0;
} TFTarea;

/* ================================================================================================= */
enum E_Mode
{
    eON,
    eOff,
    eAuto
};

/* data */
struct S_Device
{
    enum E_Mode eMode;
    bool bStatus;
};

struct S_Flag
{
    struct S_Device sU24 = {eAuto, false},
                    sLight = {eAuto, false},
                    sPump = {eON, true},
                    sUSB = {eON, true};
    bool bKeyPressSW0 = false, bKeyPressSW1 = false, bKeyPressSW2 = false, bKeyPressSW3 = false;
    bool bShowInfo = true;
    uint32_t uTR;
};

/* ================================================================================================= */
void DeviceInit(void);
void IOpinInit(void);

void TFTprintDeviceInfo(struct S_Device &sDevice, uint8_t uPinADC);
void TFTsetCursor(uint16_t x, uint16_t y, uint16_t textColor, uint8_t textSize = 2);

void SetNextModeWithPressKey(struct S_Device &sDevice);
void SetNextModeWithPressKeyInAuto(struct S_Device &sDevice);

void SerialInput(void);

#endif


#include "Src\Driver.h"

struct
{
    uint8_t uHour, uMinite, uSecond;
} sTime;

void setup()
{
    IOpinInit();
    DeviceInit();
}

void loop()
{
    if (flagX.uTR != RTC->TR)
    {
        flagX.uTR = RTC->TR;
        rtc.getTime(&(sTime.uHour), &(sTime.uMinite), &(sTime.uSecond), nullptr, nullptr);

        /* 自动模式 */
        if (flagX.sLight.eMode == eAuto)
        {
            if (((sTime.uHour >= 6) && (sTime.uHour <= 8)) ||
                ((sTime.uHour >= 12) && (sTime.uHour <= 21))
            )
            {
                flagX.sLight.bStatus = true;
            }
            else
            {
                flagX.sLight.bStatus = false;
            }
        }
        // if (flagX.sPump.eMode == eAuto)
        // {
        //     if (((sTime.uHour == 6)) ||
        //         ((sTime.uHour >= 18) && (sTime.uHour <= 19)))
        //     {
        //         flagX.sPump.bStatus = true;
        //     }
        //     else
        //     {
        //         flagX.sPump.bStatus = false;
        //     }
        // }
        // if (flagX.sU24.eMode == eAuto)
        // {
        //     if (((sTime.uHour == 6)) ||
        //         ((sTime.uHour >= 18) && (sTime.uHour <= 19)))
        //     {
        //         flagX.sU24.bStatus = true;
        //         flagX.sPump.bStatus = true;
        //     }
        //     else
        //     {
        //         flagX.sU24.bStatus = false;
        //     }
        // }

        flagX.bShowInfo = true;
        // digitalWrite(pinDev.LED, flagX.uTR % 2);
    }

    /* 处理状态 */
    if (flagX.sU24.bStatus != digitalRead(pinMOS.uU24))
    {
        digitalWrite(pinMOS.uU24, flagX.sU24.bStatus);
    }
    if (flagX.sLight.bStatus != digitalRead(pinMOS.uLight))
    {
        digitalWrite(pinMOS.uLight, flagX.sLight.bStatus);
    }
    if (flagX.sPump.bStatus != digitalRead(pinMOS.uPump))
    {
        digitalWrite(pinMOS.uPump, flagX.sPump.bStatus);
    }
    // 反逻辑
    if (flagX.sUSB.bStatus == digitalRead(pinMOS.uPump))
    {
        digitalWrite(pinMOS.uUSB, !(flagX.sUSB.bStatus));
    }

    /* 处理按钮 */
    if (flagX.bKeyPressSW0)
    {
        flagX.bKeyPressSW0 = false;
        flagX.bShowInfo = true;
        SetNextModeWithPressKey(flagX.sUSB);
    }    
    if (flagX.bKeyPressSW1)
    {
        flagX.bKeyPressSW1 = false;
        flagX.bShowInfo = true;
        SetNextModeWithPressKey(flagX.sPump);
    }
    if (flagX.bKeyPressSW2)
    {
        flagX.bKeyPressSW2 = false;
        flagX.bShowInfo = true;
        SetNextModeWithPressKeyInAuto(flagX.sU24);
    }
    if (flagX.bKeyPressSW3)
    {
        flagX.bKeyPressSW3 = false;
        flagX.bShowInfo = true;
        SetNextModeWithPressKeyInAuto(flagX.sLight);
    }

    if (flagX.bShowInfo)
    {
        TFTsetCursor(TFTarea.Clock.aX, TFTarea.Clock.aY, TFTcolor.WHITE);
        tft.printf("%06x %04x", flagX.uTR,(RTC->DR & 0x1F3F));

        TFTsetCursor(TFTarea.ColomnText, TFTarea.U24.aY, TFTcolor.VividGreen);
        TFTprintDeviceInfo(flagX.sU24, pinADC.uU24);

        TFTsetCursor(TFTarea.ColomnText, TFTarea.Light.aY, TFTcolor.VIOLET);
        TFTprintDeviceInfo(flagX.sLight, pinADC.uLight);

        TFTsetCursor(TFTarea.ColomnText, TFTarea.Pump.aY, TFTcolor.CYAN);
        TFTprintDeviceInfo(flagX.sPump, pinADC.uPump);

        TFTsetCursor(TFTarea.ColomnText, TFTarea.USB.aY, TFTcolor.YELLOW);
        TFTprintDeviceInfo(flagX.sUSB, pinADC.uUSB);

        flagX.bShowInfo = false;
    }

    /*处理串口*/
    if (Serial.available() > 1)
    {
        SerialInput();
    }
}

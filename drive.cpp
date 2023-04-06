#include "Driver.h"
#include "Font.h"

#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <STM32RTC.h>

/* ================================================================================================= */
Adafruit_ST7735 tft(pinDev.TFT_CS, pinDev.TFT_DC, pinDev.TFT_RST);
STM32RTC &rtc = STM32RTC::getInstance();

/* ================================================================================================= */

struct S_Flag flagX;

/* ================================================================================================= */
void ISR_KeyPress_SW0(void)
{
    static uint32_t lastTime;
    if ((millis() - lastTime) < 500)
        return; // 防抖动*
    lastTime = millis();
    flagX.bKeyPressSW0 = true;
};
void ISR_KeyPress_SW1(void)
{
    static uint32_t lastTime;
    if ((millis() - lastTime) < 500)
        return; // 防抖动
    lastTime = millis();
    flagX.bKeyPressSW1 = true;
};
void ISR_KeyPress_SW2(void)
{
    static uint32_t lastTime;
    if ((millis() - lastTime) < 500)
        return; // 防抖动
    lastTime = millis();
    flagX.bKeyPressSW2 = true;
};
void ISR_KeyPress_SW3(void)
{
    static uint32_t lastTime;
    if ((millis() - lastTime) < 500)
        return; // 防抖动
    lastTime = millis();
    flagX.bKeyPressSW3 = true;
};

/* ================================================================================================= */
void DeviceInit(void)
{
    Serial.begin(115200);

    analogReadResolution(12);

    rtc.setClockSource(STM32RTC::Source_Clock::LSE_CLOCK);
    rtc.begin(false, STM32RTC::Hour_Format::HOUR_24);

    SPI.setSCLK(PB13);
    SPI.setMISO(PB14);
    SPI.setMOSI(PB15);
    SPI.begin();

    tft.initR(INITR_144GREENTAB);
    tft.setRotation(3);
    tft.fillScreen(TFTcolor.BLACK);
    tft.setTextWrap(false);
    tft.setTextColor(TFTcolor.WHITE, TFTcolor.BLACK);
    tft.setTextSize(1);

    tft.fillScreen(TFTcolor.BLACK);

    tft.drawBitmap(TFTarea.USB.aX, TFTarea.USB.aY, sFont40x16.uSeparator, sFont40x16.uWidth, sFont40x16.uHeight, TFTcolor.YELLOW, TFTcolor.BLACK);
    tft.drawBitmap(TFTarea.Pump.aX, TFTarea.Pump.aY, sFont40x16.uWavePumps, sFont40x16.uWidth, sFont40x16.uHeight, TFTcolor.CYAN, TFTcolor.BLACK);
    tft.drawBitmap(TFTarea.Light.aX, TFTarea.Light.aY, sFont40x16.uLight, sFont40x16.uWidth, sFont40x16.uHeight, TFTcolor.VIOLET, TFTcolor.BLACK);
    tft.drawBitmap(TFTarea.U24.aX, TFTarea.U24.aY, sFont40x16.uAtomizer, sFont40x16.uWidth, sFont40x16.uHeight, TFTcolor.VividGreen, TFTcolor.BLACK);    
};

/* ================================================================================================= */
void IOpinInit(void)
{
    pinMode(pinADC.uU24, INPUT_ANALOG);
    pinMode(pinADC.uLight, INPUT_ANALOG);
    pinMode(pinADC.uPump, INPUT_ANALOG);
    pinMode(pinADC.uUSB, INPUT_ANALOG);

    pinMode(pinMOS.uU24, OUTPUT);
    pinMode(pinMOS.uLight, OUTPUT);
    pinMode(pinMOS.uPump, OUTPUT);
    pinMode(pinMOS.uUSB, OUTPUT_OPEN_DRAIN);

    pinMode(pinDev.LED, OUTPUT);

    pinMode(pinKey.SW0, INPUT_PULLUP);
    pinMode(pinKey.SW1, INPUT_PULLUP);
    pinMode(pinKey.SW2, INPUT_PULLUP);
    pinMode(pinKey.SW3, INPUT_PULLUP);

    digitalWrite(pinMOS.uUSB, !(flagX.sUSB.bStatus));
    digitalWrite(pinMOS.uU24, flagX.sU24.bStatus);
    digitalWrite(pinMOS.uLight, flagX.sLight.bStatus);
    digitalWrite(pinMOS.uPump, flagX.sPump.bStatus);
    digitalWrite(pinDev.LED, HIGH);

    attachInterrupt(digitalPinToInterrupt(pinKey.SW0), ISR_KeyPress_SW0, FALLING);
    attachInterrupt(digitalPinToInterrupt(pinKey.SW1), ISR_KeyPress_SW1, FALLING);
    attachInterrupt(digitalPinToInterrupt(pinKey.SW2), ISR_KeyPress_SW2, FALLING);
    attachInterrupt(digitalPinToInterrupt(pinKey.SW3), ISR_KeyPress_SW3, FALLING);
};

/* ================================================================================================= */
void TFTprintDeviceInfo(struct S_Device &sDevice, uint8_t uPinADC)
{
    const uint32_t uCoefficient = 1115; // 0.8968	1.115
    uint32_t uVoltage;

    uVoltage = analogRead(uPinADC);
    uVoltage *= 1000;
    uVoltage /= uCoefficient;

    tft.printf("%2d.%02d ", (uVoltage / 100), (uVoltage % 100));

    switch (sDevice.eMode)
    {
    case eON:
    {
        tft.setTextColor(TFTcolor.GREEN, TFTcolor.BLACK);
        tft.print(F("on"));
        break;
    }
    case eOff:
    {
        tft.setTextColor(TFTcolor.RED, TFTcolor.BLACK);
        tft.print(F("x "));
        break;
    }
    case eAuto:
    {
        if (sDevice.bStatus)
        {
            tft.setTextColor(TFTcolor.GREEN, TFTcolor.BLACK);
        }
        else
        {
            tft.setTextColor(TFTcolor.WHITE, TFTcolor.BLACK);
        }
        tft.print(F("Au"));
        break;
    }
    }
}

void TFTsetCursor(uint16_t x, uint16_t y, uint16_t textColor, uint8_t textSize)
{
    tft.setTextColor(textColor, TFTcolor.BLACK);
    tft.setTextSize(textSize);
    tft.setCursor(x, y);
};

/* ================================================================================================= */
void RtcSetRegCALR(uint32_t uTimeStampCurrent, uint32_t uTimeStampLocal, uint32_t uTimeLastUpdate)
{
    uint8_t iHour = 0, iMinute = 0, iSecond = 0;
    rtc.getTime(&iHour, &iMinute, &iSecond, nullptr, nullptr);
    Serial.printf("Time Set %2d:%02d:%02d ", iHour, iMinute, iSecond);
    Serial.println();

    // RTC 精准校准
    Serial.print(F("Local     :"));
    Serial.println(uTimeStampLocal);
    Serial.print(F("Current   :"));
    Serial.println(uTimeStampCurrent);
    Serial.print(F("LastUpdate:"));
    Serial.println(uTimeLastUpdate);
    Serial.println();

    // 时间比现在更早则退出
    if (uTimeLastUpdate >= uTimeStampCurrent)
    {
        Serial.println(F("RTC Error."));
        return;
    }
    // 时间一致
    if (uTimeStampLocal == uTimeStampCurrent)
    {
        Serial.println(F("Time Same."));
        return;
    }

    int32_t uSetCalrValue = RTC->CALR & RTC_CALR_CALM_Msk;
    uint32_t uDiffTime;
    uint32_t uSampling = uTimeStampCurrent - uTimeLastUpdate;
    Serial.print(F("Sampling (s):"));
    Serial.println(uSampling);
    Serial.print(F("DiffTime "));
    // 时间快了
    if (uTimeStampLocal > uTimeStampCurrent)
    {
        Serial.print(F("<fast> :"));
        uDiffTime = uTimeStampLocal - uTimeStampCurrent;
    }
    else
    {
        // 时间慢了
        Serial.print(F("<slow> :"));
        uDiffTime = uTimeStampCurrent - uTimeStampLocal;
    }
    Serial.println(uDiffTime);

    // RTC->CALR   32(s)*32768(Hz)=1048576
    uint32_t uCalrDetla = uDiffTime * 1048576ul / uSampling;
    Serial.print(F("CalrDetla:"));
    Serial.println(uCalrDetla);

    if (uTimeStampLocal < uTimeStampCurrent)
    {
        uSetCalrValue -= uCalrDetla;
        if (uSetCalrValue < 0)
            uSetCalrValue = 0;
    }
    else
    {
        uSetCalrValue += uCalrDetla;
        if (uSetCalrValue > 511ul)
            uSetCalrValue = 511ul;
    }

    enableBackupDomain();

    RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_1;
    RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_2;
    RTC->CALR = uSetCalrValue & RTC_CALR_CALM_Msk;
    RTC->WPR = RTC_WRITE_PROTECTION_DISABLE;

    setBackupRegister(BKPregisterIndex.TimeUpdateLast, uTimeStampCurrent);
    disableBackupDomain();

    Serial.print(F("RTC->CALR:"));
    Serial.println(uSetCalrValue);
    Serial.println();
}

/* ================================================================================================= */
void RtcSetTimeByEpoch(uint32_t uTimeStamp)
{
    // 时区 CCT + 8 hour Beijing
    uint32_t uTimeStampCurrent = uTimeStamp + 28800ul;
    uint32_t uTimeLastUpdate = getBackupRegister(BKPregisterIndex.TimeUpdateLast);
    uint32_t uTimeStampLocal = rtc.getEpoch();

    rtc.setClockSource(STM32RTC::Source_Clock::LSE_CLOCK);
    rtc.begin(false, STM32RTC::Hour_Format::HOUR_24);
    rtc.setEpoch(uTimeStampCurrent);

    RtcSetRegCALR(uTimeStampCurrent, uTimeStampLocal, uTimeLastUpdate);
};

/* ================================================================================================= */
void RtcSetTime(uint32_t uHHMMSS)
{
    uint32_t uTimeStampCurrent;
    uint32_t uTimeLastUpdate = getBackupRegister(BKPregisterIndex.TimeUpdateLast);
    uint32_t uTimeStampLocal = rtc.getEpoch();

    rtc.setClockSource(STM32RTC::Source_Clock::LSE_CLOCK);
    rtc.begin(false, STM32RTC::Hour_Format::HOUR_24);

    uint32_t iSecond = uHHMMSS % 100;
    uHHMMSS /= 100;
    uint32_t iMinute = uHHMMSS % 100;
    uint32_t iHour = uHHMMSS / 100;

    if (iHour > 23)
        iHour = 23;
    if (iMinute > 59)
        iMinute = 59;
    if (iSecond > 59)
        iSecond = 59;

    rtc.setTime((uint8_t)iHour, (uint8_t)iMinute, (uint8_t)iSecond);
    uTimeStampCurrent = rtc.getEpoch();

    RtcSetRegCALR(uTimeStampCurrent, uTimeStampLocal, uTimeLastUpdate);
};

/* ================================================================================================= */
void SerialInput(void)
{
    switch (Serial.read())
    {
    case 't':
    {
        uint32_t uTimeStamp = Serial.parseInt();
        RtcSetTimeByEpoch(uTimeStamp);
        break;
    }
    case 's':
    {
        uint32_t uHHMMSS = Serial.parseInt();
        RtcSetTime(uHHMMSS);
        break;
    }
    case '?':
    {
        Serial.println(F("Command List:\n"));
        Serial.printf(">>t[%lu]\n", getBackupRegister(BKPregisterIndex.TimeUpdateLast));
        Serial.print(F(">>s[HHMMSS]\n"));
        Serial.printf("RTC->CALR:%lu\n", (RTC->CALR));
        Serial.println();
        break;
    }
    }
}

/* ================================================================================================= */
void SetNextModeWithPressKey(struct S_Device &sDevice)
{
    if (sDevice.eMode == eON)
    {
        sDevice.eMode = eOff;
        sDevice.bStatus = false;
    }
    else
    {
        sDevice.eMode = eON;
        sDevice.bStatus = true;
    }
}

void SetNextModeWithPressKeyInAuto(struct S_Device &sDevice)
{
    if (sDevice.eMode == eON)
    {
        sDevice.eMode = eOff;
        sDevice.bStatus = false;
    }
    else if (sDevice.eMode == eOff)
    {
        sDevice.eMode = eAuto;
    }
    else
    {
        sDevice.eMode = eON;
        sDevice.bStatus = true;
    }
}

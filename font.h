#ifndef __LANDTANK_FONT_H
#define __LANDTANK_FONT_H

static const struct
{
    const uint8_t uWidth = 48, uHeight = 16;
    //灯光:
    const uint8_t uLight[96] = {16, 0, 1, 0, 0, 0, 16, 0, 33, 8, 0, 0, 19, 254, 17, 8, 0, 0, 16, 32, 9, 16, 0, 0, 84, 32, 9, 32, 0, 0, 88, 32, 1, 0, 0, 0, 80, 32, 255, 254, 24, 0, 144, 32, 4, 64, 24, 0, 16, 32, 4, 64, 0, 0, 16, 32, 4, 64, 0, 0, 16, 32, 4, 64, 0, 0, 40, 32, 8, 66, 0, 0, 36, 32, 8, 66, 24, 0, 68, 32, 16, 66, 24, 0, 64, 160, 32, 62, 0, 0, 128, 64, 192, 0, 0, 0};
    //水泵:
    const uint8_t uPump[96] = {1, 0, 0, 0, 0, 0, 1, 0, 255, 254, 0, 0, 1, 0, 4, 0, 0, 0, 1, 8, 8, 0, 0, 0, 1, 8, 31, 240, 0, 0, 125, 144, 40, 16, 0, 0, 5, 160, 200, 16, 24, 0, 9, 64, 15, 240, 24, 0, 9, 64, 1, 0, 0, 0, 17, 32, 1, 4, 0, 0, 17, 16, 125, 136, 0, 0, 33, 8, 9, 80, 0, 0, 65, 6, 17, 32, 24, 0, 129, 0, 33, 24, 24, 0, 5, 0, 197, 6, 0, 0, 2, 0, 2, 0, 0, 0};
    //雾化:
    const uint8_t uAtomizer[96] = {63, 248, 8, 128, 0, 0, 1, 0, 8, 128, 0, 0, 127, 254, 8, 132, 0, 0, 65, 2, 16, 136, 0, 0, 157, 116, 16, 144, 0, 0, 1, 0, 48, 160, 0, 0, 29, 112, 48, 192, 24, 0, 4, 0, 80, 128, 24, 0, 15, 224, 145, 128, 0, 0, 20, 64, 18, 128, 0, 0, 3, 128, 20, 128, 0, 0, 28, 112, 16, 130, 0, 0, 226, 14, 16, 130, 24, 0, 15, 224, 16, 130, 24, 0, 4, 32, 16, 126, 0, 0, 24, 96, 16, 0, 0, 0};
    //造浪:
    const uint8_t uWavePumps[96] = {0, 64, 0, 128, 0, 0, 34, 64, 32, 64, 0, 0, 19, 248, 19, 248, 0, 0, 20, 64, 18, 8, 0, 0, 0, 64, 130, 8, 0, 0, 7, 252, 67, 248, 0, 0, 240, 0, 74, 8, 24, 0, 16, 0, 10, 8, 24, 0, 19, 248, 19, 248, 0, 0, 18, 8, 18, 64, 0, 0, 18, 8, 226, 68, 0, 0, 19, 248, 34, 40, 0, 0, 18, 8, 34, 16, 24, 0, 40, 0, 34, 136, 24, 0, 71, 254, 35, 6, 0, 0, 0, 0, 2, 0, 0, 0};
    //蛋分：
    const uint8_t uSeparator[96] = {127, 252, 0, 64, 0, 0, 1, 4, 4, 64, 0, 0, 17, 0, 4, 32, 0, 0, 17, 248, 8, 32, 0, 0, 17, 0, 16, 16, 0, 0, 41, 0, 32, 8, 0, 0, 71, 254, 64, 4, 24, 0, 129, 0, 159, 226, 24, 0, 31, 240, 4, 32, 0, 0, 17, 16, 4, 32, 0, 0, 17, 16, 4, 32, 0, 0, 31, 240, 8, 32, 0, 0, 17, 0, 8, 32, 24, 0, 1, 8, 16, 32, 24, 0, 127, 252, 33, 64, 0, 0, 32, 4, 64, 128, 0, 0};
} sFont40x16;

#endif

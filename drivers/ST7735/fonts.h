/* vim: set ai et ts=4 sw=4: */
#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;

extern FontDef Font_7x10;

#ifdef USE_FONT_11X18
extern FontDef Font_11x18;
#endif

#ifdef USE_FONT_16X26
extern FontDef Font_16x26;
#endif

#endif // __FONTS_H__

#ifndef __ST7789V_H_
#define __ST7789V_H_

#include "spi.h"
#include "gpio.h"
#include "gpdma.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458
#define LIGHTGREEN 0X841F
#define LGRAY 0XC618
#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12

    typedef struct
    {
        volatile uint8_t *Image;
        volatile uint16_t Width;
        volatile uint16_t Height;
        volatile uint16_t WidthMemory;
        volatile uint16_t HeightMemory;
        volatile uint16_t Color;
        volatile uint16_t Rotate;
        volatile uint16_t Mirror;
        volatile uint16_t WidthByte;
        volatile uint16_t HeightByte;
    } Pain;

    void LCD_clear(uint16_t Color);
    void LCD_init(uint16_t Scan_dir);
    void LCD_drawImage(const unsigned char *image, int x_start, int y_start, int wide, int hight);
    void LCD_SetWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
    void spi_setDataLength(int bit);

#ifdef __cplusplus
}
#endif

#endif
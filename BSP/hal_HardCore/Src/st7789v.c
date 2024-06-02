#include "st7789v.h"

volatile Pain draw;

void (*Display)(uint16_t, uint16_t, uint16_t);

void spi_setDataLength(int bit)
{
    if (bit == SPI_DATASIZE_16BIT)
    {
        hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
    }
    else if (SPI_DATASIZE_8BIT)
    {
        hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    }
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
}

void st7789v_WriteDate_Byte(uint16_t Data)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&Data, 1, 500);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void st7789v_WriteDate_Word(uint16_t Data)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&Data, 1, 10);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void st7789v_WriteCmd(uint16_t Cmd)
{
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&Cmd, 1, 500);
}

void LCD_SetWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    Xstart = Xstart;
    Xend = Xend;
    Ystart = Ystart + 20;
    Yend = Yend + 20;
    spi_setDataLength(SPI_DATASIZE_8BIT);
    st7789v_WriteCmd(0x2a);
    st7789v_WriteDate_Byte(Xstart >> 8);
    st7789v_WriteDate_Byte(Xstart);
    st7789v_WriteDate_Byte(Xend >> 8);
    st7789v_WriteDate_Byte(Xend);

    st7789v_WriteCmd(0x2b);
    st7789v_WriteDate_Byte(Ystart >> 8);
    st7789v_WriteDate_Byte(Ystart);
    st7789v_WriteDate_Byte(Yend >> 8);
    st7789v_WriteDate_Byte(Yend);

    st7789v_WriteCmd(0x2C);
}

void LCD_clear(uint16_t Color)
{
    spi_setDataLength(SPI_DATASIZE_8BIT);
    LCD_SetWindow(0, -7, 240 - 1, 280 - 1);
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    spi_setDataLength(SPI_DATASIZE_16BIT);
    for (int i = 0; i < 240; i++)
    {
        for (int j = 0; j < 280; j++)
        {
            HAL_SPI_Transmit(&hspi1, (uint8_t *)&Color, 1, 500);
        }
    }
}

void LCD_Reset(void)
{
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
}

//* 0x00为竖直 0x70为水平
void LCD_SetAttributes(uint16_t MemoryAccessReg)
{
    st7789v_WriteCmd(0x36);
	
    st7789v_WriteDate_Byte(MemoryAccessReg);
		
}

void LCD_initReg(void)
{
		
    spi_setDataLength(SPI_DATASIZE_8BIT);
    st7789v_WriteCmd(0x36);
    st7789v_WriteDate_Byte(0x00);
		
    st7789v_WriteCmd(0x3A);
    st7789v_WriteDate_Byte(0x05);
		
    st7789v_WriteCmd(0xB2);
    st7789v_WriteDate_Byte(0x0B);
    st7789v_WriteDate_Byte(0x0B);
    st7789v_WriteDate_Byte(0x00);
    st7789v_WriteDate_Byte(0x33);
    st7789v_WriteDate_Byte(0x35);
		
    st7789v_WriteCmd(0xB7);
    st7789v_WriteDate_Byte(0x11);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
    st7789v_WriteCmd(0xBB);
    st7789v_WriteDate_Byte(0x35);
		
    st7789v_WriteCmd(0xC0);
    st7789v_WriteDate_Byte(0x2C);
		
    st7789v_WriteCmd(0xC2);
    st7789v_WriteDate_Byte(0x01);
		
    st7789v_WriteCmd(0xC3);
    st7789v_WriteDate_Byte(0x0D);
		
    st7789v_WriteCmd(0xC4);
    st7789v_WriteDate_Byte(0x20);

    st7789v_WriteCmd(0xC6);
    st7789v_WriteDate_Byte(0x13);

    st7789v_WriteCmd(0xD0);
    st7789v_WriteDate_Byte(0xA4);
    st7789v_WriteDate_Byte(0xA1);

    st7789v_WriteCmd(0xD6);
    st7789v_WriteDate_Byte(0xA1);

    st7789v_WriteCmd(0xE0);
    st7789v_WriteDate_Byte(0xF0);
    st7789v_WriteDate_Byte(0x06);
    st7789v_WriteDate_Byte(0x0B);
    st7789v_WriteDate_Byte(0x0A);
    st7789v_WriteDate_Byte(0x09);
    st7789v_WriteDate_Byte(0x26);
    st7789v_WriteDate_Byte(0x29);
    st7789v_WriteDate_Byte(0x33);
    st7789v_WriteDate_Byte(0x41);
    st7789v_WriteDate_Byte(0x18);
    st7789v_WriteDate_Byte(0x16);
    st7789v_WriteDate_Byte(0x15);
    st7789v_WriteDate_Byte(0x29);
    st7789v_WriteDate_Byte(0x2D);

    st7789v_WriteCmd(0xE1);
    st7789v_WriteDate_Byte(0xF0);
    st7789v_WriteDate_Byte(0x04);
    st7789v_WriteDate_Byte(0x08);
    st7789v_WriteDate_Byte(0x08);
    st7789v_WriteDate_Byte(0x07);
    st7789v_WriteDate_Byte(0x03);
    st7789v_WriteDate_Byte(0x28);
    st7789v_WriteDate_Byte(0x32);
    st7789v_WriteDate_Byte(0x40);
    st7789v_WriteDate_Byte(0x3B);
    st7789v_WriteDate_Byte(0x19);
    st7789v_WriteDate_Byte(0x18);
    st7789v_WriteDate_Byte(0x2A);
    st7789v_WriteDate_Byte(0x2E);

    st7789v_WriteCmd(0xE4);
    st7789v_WriteDate_Byte(0x25);
    st7789v_WriteDate_Byte(0x00);
    st7789v_WriteDate_Byte(0x00);

    st7789v_WriteCmd(0x21);

    st7789v_WriteCmd(0x11);
    HAL_Delay(120);
    st7789v_WriteCmd(0x29);
		
}

void LCD_init(uint16_t Scan_dir)
{
    LCD_Reset();
    LCD_SetAttributes(Scan_dir);
    LCD_initReg();
}

void LCD_NewImage(uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color)
{
    draw.WidthMemory = Width;
    draw.HeightMemory = Height;
    draw.Color = Color;
    draw.WidthByte = Width;
    draw.HeightByte = Height;

    draw.Rotate = Rotate;
    draw.Mirror = 0; //^ 无镜像
    if (Rotate == 0 || Rotate == 180)
    {
        draw.Width = Width;
        draw.Height = Height;
    }
    else
    {
        draw.Width = Height;
        draw.Height = Width;
    }
}

void LCD_SetPixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color)
{
    uint16_t X, Y;

    switch (0) // draw.Rotate)
    {
    case 0:
        X = Xpoint;
        Y = Ypoint;
        break;
    case 90:
        X = 240 /*draw.WidthMemory*/ - Ypoint - 1;
        Y = Xpoint;
        break;
    case 180:
        X = draw.WidthMemory - Xpoint - 1;
        Y = draw.HeightMemory - Ypoint - 1;
        break;
    case 270:
        X = Ypoint;
        Y = draw.HeightMemory - Xpoint - 1;
        break;
    default:
        return;
    }

    switch (0 /*draw.Mirror*/)
    {
    case 0:
        break;
    case 1:
        X = draw.WidthMemory - X - 1;
        break;
    case 2:
        Y = draw.HeightMemory - Y - 1;
        break;
    case 3:
        X = draw.WidthMemory - X - 1;
        Y = draw.HeightMemory - Y - 1;
        break;
    default:
        return;
    }
    // Display(X, Y, Color);
    LCD_SetWindow(X, Y - 7, X, Y - 7);
    // HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    // uint16_t temp = (Color >> 8) & 0xff;
    // HAL_SPI_Transmit(&hspi1, (uint8_t *)&temp, 1, 500);
    // HAL_SPI_Transmit(&hspi1, (uint8_t *)&Color, 1, 500);
    // return;
    st7789v_WriteDate_Word(Color);
}

void LCD_DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char Acsii_Char,
                  sFont *Font, uint16_t Color_Background, uint16_t Color_Foreground)
{
    uint16_t Page, Column;
    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];
    LCD_SetWindow(Xpoint, Ypoint, Xpoint + Font->Width - 1, Ypoint + Font->Height - 7);
    spi_setDataLength(SPI_DATASIZE_16BIT);
    for (Page = 0; Page < Font->Height; Page++)
    {
        for (Column = 0; Column < Font->Width; Column++)
        {
            if (0xFFFF == Color_Background)
            {
                if (*ptr & (0x80 >> (Column % 8)))
                    st7789v_WriteDate_Word(Color_Foreground);
            }
            else
            {
                if (*ptr & (0x80 >> (Column % 8)))
                {
                    st7789v_WriteDate_Word(Color_Foreground);
                }
                else
                {
                    st7789v_WriteDate_Word(Color_Background);
                }
            }
            if (Column % 8 == 7)
                ptr++;
        }
        if (Font->Width % 8 != 0)
            ptr++;
    }
}

void LCD_DrawString(uint16_t Xstart, uint16_t Ystart, const char *pString,
                    sFont *Font, uint16_t Color_Background, uint16_t Color_Foreground)
{
    uint16_t Xpoint = Xstart;
    uint16_t Ypoint = Ystart;
    while (*pString != '\0')
    {
        if ((Xpoint + Font->Width) > 240 /*draw.Width*/)
        {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }
        if ((Ypoint + Font->Height) > 280 /*draw.Height*/)
        {
            Xpoint = Xstart;
            Ypoint = Ystart;
        }
        LCD_DrawChar(Xpoint, Ypoint, *pString, Font, Color_Background, Color_Foreground);
        pString++;
        Xpoint += Font->Width;
    }
}

#define PLUS 31199
int data_length = 0;
const unsigned char *p = NULL;
extern DMA_HandleTypeDef handle_GPDMA1_Channel12;
void LCD_drawImage(const unsigned char *image, int x_start, int y_start, int wide, int hight)
{
    while (HAL_DMA_GetState(&handle_GPDMA1_Channel12) == HAL_DMA_STATE_BUSY)
        HAL_Delay(1);
    LCD_SetWindow(x_start, y_start, x_start + wide - 1, y_start + hight - 1);
    spi_setDataLength(SPI_DATASIZE_16BIT);
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

    data_length = hight * wide;
    p = image;
    if (data_length > PLUS)
        HAL_SPI_Transmit_DMA(&hspi1, image, PLUS);
    else
        HAL_SPI_Transmit_DMA(&hspi1, image, data_length);

    // HAL_SPI_Transmit(&hspi1, image, hight * wide, 1000);
    // HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    static unsigned int accumulation = 1;
    if (hspi == &hspi1)
    {
        data_length -= PLUS;
        if (data_length > PLUS)
        {
            HAL_SPI_Transmit_DMA(&hspi1, p + accumulation * PLUS * 2, PLUS);
            accumulation++;
        }
        else if (data_length > 0)
            HAL_SPI_Transmit_DMA(&hspi1, p + accumulation * PLUS * 2, data_length);
        else
        {
            HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
            accumulation = 1;
        }
    }
}

uint16_t RGB888toRGB565(uint16_t B, uint16_t G, uint16_t R)
{
    B /= 8;
    G /= 4;
    R /= 8;
    return (B << 11) + (G << 5) + R;
}
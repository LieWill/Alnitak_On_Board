#include "CST816T.h"
// IIC驱动实现

#define LCD_W 240
#define LCD_H 280
#define delay_num 4 // 宏定义延时时间

GPIO_InitTypeDef GPIO_InitStruct = {0};
void CST816T_ReceiveByte(uint8_t Addr, uint8_t *Data);

void CST816T_Init(void)
{
    HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(50);
}

void CST816T_SendByte(uint8_t Addr, uint8_t Data)
{
    uint8_t Buffer[2] = {Addr, Data};
    HAL_I2C_Master_Transmit(&hi2c3, 0x2A, Buffer, 2, HAL_MAX_DELAY);
}

void CST816T_ReceiveByte(uint8_t Addr, uint8_t *Data)
{
    HAL_I2C_Mem_Read(&hi2c3, 0x2a, Addr, I2C_MEMADD_SIZE_8BIT, Data, 1, HAL_MAX_DELAY);
}

uint8_t CST816_GetAction(uint16_t *X, uint16_t *Y)
{
    uint8_t data[6];
brak:
    HAL_I2C_Mem_Read(&hi2c3, 0x2a, 0x01, I2C_MEMADD_SIZE_8BIT, data, 6, 10);
    *X = (uint16_t)((data[2] & 0x0F) << 8) | data[3];
    *Y = (uint16_t)((data[4] & 0x0F) << 8) | data[5];
    *X = *X / 11.0 * 12 - 5;
    *Y = *Y / 25.0 * 28;
    if (*X > 250 || *Y > 300)
    {
        HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, GPIO_PIN_RESET);
        HAL_Delay(10);
        HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, GPIO_PIN_SET);
        HAL_Delay(50);
        return 0;
        goto brak;
    }
    return data[1];
}

// uint8_t data[6] = {0};

// uint8_t CST816_GetAction(TouchType *Touch)
// {
//     //brak:
//     //HAL_I2C_Mem_Read(&hi2c3, 0x2a, 0x01, I2C_MEMADD_SIZE_8BIT, data, 6, 10);
//     HAL_I2C_Mem_Read_DMA(&hi2c3, 0x2a, 0x01, I2C_MEMADD_SIZE_8BIT, (uint8_t *)data, 6);
//     osDelay(10);
//     Touch->Gesture = data[0];
//     Touch->X       = (uint16_t)((data[2] & 0x0F) << 8) | data[3];
//     Touch->Y       = (uint16_t)((data[4] & 0x0F) << 8) | data[5];
//     // if (Touch->X > 250 || Touch->Y > 250) {
//     //     HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, GPIO_PIN_RESET);
//     //     osDelay(10);
//     //     HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, GPIO_PIN_SET);
//     //     osDelay(50);
//     //     //return 0;
//     //     goto brak;
//     // }
//     return data[1];
//     // Touch->Finger;
// }
/**
*********************************************************************
*********
* @project_name :templateDevelopment
* @file : MyKey.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/5/4 
*********************************************************************
*********
*/
//

#include "MyKey.h"

MyKey::KEYS MyKey::ScanPressedKey(uint32_t timeout) {
    KEYS keys = KEY_NONE;
    uint32_t tickStart = HAL_GetTick();
    const uint32_t btnDelay = 20;
    GPIO_PinState keyState;
    while (1) {
        keyState = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);
        if (keyState == GPIO_PIN_RESET) {
            HAL_Delay(btnDelay);
            keyState = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);
            if (keyState == GPIO_PIN_RESET) {
                return KEY_DOWN;
            }
        }
        keyState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
        if (keyState == GPIO_PIN_SET) {
            HAL_Delay(btnDelay);
            keyState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
            if (keyState == GPIO_PIN_SET) {
                return KEY_UP;
            }
        }
        if (timeout!=KEY_WAIT_ALWAYS){
            if ((HAL_GetTick()-tickStart)>timeout)
                break;

        }
    }
    return keys;
}
void MyKey::KeyScan(uint32_t timeOut) {
    MyUsart myUsart(&huart1);

    KEYS curKey = ScanPressedKey(timeOut);
    switch (curKey) {
        case KEY_LEFT:

            break;
        case KEY_RIGHT:

            break;
        case KEY_UP:
           flage++;
            if (flage>=2)
                flage=0;
            myUsart.SendOneInt(flage);
           myUsart.SendString("UP",4);
            break;
        case KEY_DOWN:
            flage--;
            if (flage<0)
                flage=0;
            myUsart.SendString("DOWN",4);
            break;
    }
    HAL_Delay(150);
}
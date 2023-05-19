/**
*********************************************************************
*********
* @project_name :templateDevelopment
* @file : Mymain.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/4/27 
*********************************************************************
*********
*/
//
#include "Myapplication.h"
#include "IncludeALL.h"

#define SIZE 1024
#define DIVISOR 66
MyLCD myLcd;
MyFFT myFFT;

//定义变量或者数组只能声明局部变量 不能声明全局否则会找不到地址
void Mymain()
{

    float adc;
    double dac=0;
    char temp[24];
    char *dacv;
    int flag=0;
    MyUsart myUsart(&huart1);
    MyADC myAdc(&hadc2);
    MyDAC myDac(&hdac,DAC1_CHANNEL_1);
    MyKey myKey;
    myLcd.MyLCDTouchInit();
    myDac.MyDACStart();
    myDac.MyDACSetValue((uint32_t) dac);
    lcd_show_string((480-200)/2,30,200,24,24,"DC power supply",RED);
    lcd_show_string(0,300,200,24,24,"DACSetVaule:",RED);
    lcd_show_string(280,300,200,24,24,"ElectricCurrent:",RED);
    sprintf(temp,"%lf",(dac* 3.3 / 4095));
    lcd_fill(0,330,200,354,WHITE);
    lcd_show_string(0,330,200,24,24,temp,RED);
    adc=myAdc.myADCGetADCValue();
    myUsart.SendOneFloat(adc);
    myLcd.flag=1;
    myLcd.MyLCDLoadKeyBoard();

    for (;;) {
        myKey.KeyScan(100);
        dacv=myLcd.MyLCDMonitorScreen(100);
        //myUsart.SendString(dacv,20);
        if (dacv!=NULL&&dacv[1]!='-'&&dacv[2]!='+') {
            dac = atof(dacv);
            if (dac<=3.3) {
                myUsart.SendString("tihs:", 6);
                dac = dac / 3.3 * 4095;
                myUsart.SendOneFloat(dac);
//                myDac.MyDACStart();
                myDac.MyDACSetValue((uint32_t) dac);

                lcd_show_string(0,300,200,24,24,"DACSetVaule:",RED);
                sprintf(temp,"%lf",(dac* 3.3 / 4095));
                lcd_fill(0,330,200,354,WHITE);
                lcd_show_string(0,330,200,24,24,temp,RED);
                //lcd_show_float(0,330,dac,24,0,RED);
                adc = myAdc.myADCGetADCValue();
                myUsart.SendOneFloat(adc);
                lcd_show_string(280,300,200,24,24,"ElectricCurrent:",RED);
                sprintf(temp,"%lf",(adc/DIVISOR));
                lcd_fill(280,330,480,354,WHITE);
                lcd_show_string(280,330,200,24,24,temp,RED);
            }
            else
            {

                lcd_show_string(105, 380, 240, 24, 24, "Error", RED);
                HAL_Delay(500);
                lcd_fill(105,365,300,400,WHITE);
                lcd_show_string(105, 380, 240, 24, 24, "Please re-enter", RED);
                HAL_Delay(1000);
                lcd_fill(105,365,300,400,WHITE);
            }
        }
        if (myKey.flage==0) {
            if (flag==0) {
                flag++;
                lcd_fill(0, 200, 200, 230, WHITE);
                lcd_show_string(0, 200, 200, 24, 24, "Manual", RED);
            }
            if (dacv[1] == '-') {
                myUsart.SendOneFloat(dac);
                if (dac >= 0 && dac < 4095) {
                    dac -= 41;
                    myDac.MyDACSetValue(dac);
                    sprintf(temp,"%lf",(dac* 3.3 / 4095));
                    lcd_fill(0,330,200,354,WHITE);
                    lcd_show_string(0,330,200,24,24,temp,RED);
                    adc = myAdc.myADCGetADCValue();
                    sprintf(temp,"%lf",(adc/DIVISOR));
                    lcd_fill(280,330,480,354,WHITE);
                    lcd_show_string(280,330,200,24,24,temp,RED);
                    myUsart.SendOneFloat(adc);

                }
                if (dac > 4095 || dac < 10) {
                    dac = 1240;
                    myDac.MyDACSetValue(dac);
                    sprintf(temp,"%lf",(dac* 3.3 / 4095));
                    lcd_fill(0,330,200,354,WHITE);
                    lcd_show_string(0,330,200,24,24,temp,RED);
                }

            } else if (dacv[2] == '+') {
                myUsart.SendOneFloat(dac);
                if (dac >= 0 && dac < 4095) {
                    dac += 41;
                    sprintf(temp,"%lf",(dac* 3.3 / 4095));
                    lcd_fill(0,330,200,354,WHITE);
                    lcd_show_string(0,330,200,24,24,temp,RED);
                    myDac.MyDACSetValue(dac);
                    adc = myAdc.myADCGetADCValue();
                    sprintf(temp,"%lf",(adc/DIVISOR));
                    lcd_fill(280,330,480,354,WHITE);
                    lcd_show_string(280,330,200,24,24,temp,RED);
                    myUsart.SendOneFloat(adc);

                }
                if (dac > 4095 || dac < 10) {
                    dac = 1240;
                    myDac.MyDACSetValue(dac);
                    sprintf(temp,"%lf",(dac* 3.3 / 4095));
                    lcd_fill(0,330,200,354,WHITE);
                    lcd_show_string(0,330,200,24,24,temp,RED);
                }
            }
        }
        else if (myKey.flage==1)
        {

            if (flag==1) {
                flag--;
                lcd_fill(0, 200, 200, 230, WHITE);
                lcd_show_string(0, 200, 200, 24, 24, "Automatic", RED);
            }
            dac += 41;
            if (dac>=4095)
                dac=4095;
            sprintf(temp,"%lf",(dac* 3.3 / 4095));
            lcd_fill(0,330,200,354,WHITE);
            lcd_show_string(0,330,200,24,24,temp,RED);
            myDac.MyDACSetValue(dac);
            adc = myAdc.myADCGetADCValue();
            sprintf(temp,"%lf",(adc/DIVISOR));
            lcd_fill(280,330,480,354,WHITE);
            lcd_show_string(280,330,200,24,24,temp,RED);
            myUsart.SendOneFloat(adc);

        }
        else if (myKey.flage==2)
        {
            dac -= 41;
            if (dac<=0)
                dac=0;
            sprintf(temp,"%lf",(dac* 3.3 / 4095));
            lcd_fill(0,330,200,354,WHITE);
            lcd_show_string(0,330,200,24,24,temp,RED);
            myDac.MyDACSetValue(dac);
            adc = myAdc.myADCGetADCValue();
            sprintf(temp,"%lf",(adc/DIVISOR));
            lcd_fill(280,330,480,354,WHITE);
            lcd_show_string(280,330,200,24,24,temp,RED);
            myUsart.SendOneFloat(adc);
        }
       // myUsart.SendString("hello",6);
    }

    HAL_Delay(500);
}
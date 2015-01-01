/**
  ******************************************************************************
  * @file    audio.h
  * @author  Ivan Pavic
  * @version V1.0
  * @date    01-January-2015
  * @brief   This file provides defines of some important pins, and declarations 
  *          of functions.
	*
  ******************************************************************************
  */

#ifndef __AUDIO_H_
#define __AUDIO_H_

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_i2c.h>


/** GPIOA **/
#define I2S_WS GPIO_Pin_4

/** GPIOB **/
#define I2C_SCL GPIO_Pin_6
#define I2C_SDA GPIO_Pin_9

/** GPIOC **/
#define I2S_MCLK GPIO_Pin_7
#define I2S_SCLK GPIO_Pin_10
#define I2S_SD GPIO_Pin_12

/** GPIOD **/
#define AUDIO_RST GPIO_Pin_4

#define AUDIO_I2C_ADDRESS 0x94
#define HOME_ADDRESS 0x33


void init_I2S(void);
void init_I2C(void);
void sendBytesAudioI2C(char *bytes, int num);
void configAudio(void);


#endif

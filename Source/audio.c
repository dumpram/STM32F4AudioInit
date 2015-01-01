/**
  ******************************************************************************
  * @file    audio.c
  * @author  Ivan Pavic
  * @version V1.0
  * @date    01-January-2015
  * @brief   This file provides initialization functions for audio configuration.
  *          
  ******************************************************************************
  */

#include "audio.h"

void init_I2S() {
	I2S_InitTypeDef I2S_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	  /* Initialize the I2S_Mode member */
  I2S_InitStruct.I2S_Mode = I2S_Mode_MasterTx;
  /* Initialize the I2S_Standard member */
  I2S_InitStruct.I2S_Standard = I2S_Standard_Phillips;
  /* Initialize the I2S_DataFormat member */
  I2S_InitStruct.I2S_DataFormat = I2S_DataFormat_16b;
  /* Initialize the I2S_MCLKOutput member */
  I2S_InitStruct.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
  /* Initialize the I2S_AudioFreq member */
  I2S_InitStruct.I2S_AudioFreq = I2S_AudioFreq_48k;
  /* Initialize the I2S_CPOL member */
  I2S_InitStruct.I2S_CPOL = I2S_CPOL_Low;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	RCC_PLLI2SCmd(ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = I2S_SCLK | I2S_SD | I2S_MCLK;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = I2S_WS;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);	
	
	I2S_Init(SPI3, &I2S_InitStruct);
	I2S_Cmd(SPI3, ENABLE);
}

void init_I2C() {
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/** Enable clocks **/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	/** Configure GPIO for I2C **/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD; 
	GPIO_InitStruct.GPIO_Pin = I2C_SCL | I2C_SDA;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	
	/* initialize the I2C_ClockSpeed member */
  I2C_InitStruct.I2C_ClockSpeed = 100000;
  /* Initialize the I2C_Mode member */
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  /* Initialize the I2C_DutyCycle member */
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  /* Initialize the I2C_OwnAddress1 member */
  I2C_InitStruct.I2C_OwnAddress1 = 33;
  /* Initialize the I2C_Ack member */
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  /* Initialize the I2C_AcknowledgedAddress member */
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE);
}

void init_AudioReset() {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = AUDIO_RST;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void configAudio() {
	char bytes[5]; 
	init_I2S();
	init_I2C();
	init_AudioReset();	
	GPIO_SetBits(GPIOD, AUDIO_RST);
	
	/** Power sequence **/
	bytes[0] = 0x02;
	bytes[1] = 0x01;
	sendBytesAudioI2C(bytes, 2);
	
	/** Initialization sequence **/
	bytes[0] = 0x00;
	bytes[1] = 0x99;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x47;
	bytes[1] = 0x80;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x32;
	bytes[1] = 0x80;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x32;
	bytes[1] = 0x0;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x00;
	bytes[1] = 0x00;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x04;
	bytes[1] = 0xAF;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x0D;
	bytes[1] = 0x70;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x05;
	bytes[1] = 0x81;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x06;
	bytes[1] = 0x07;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x0A;
	bytes[1] = 0x00;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x27;
	bytes[1] = 0x00;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x10;
	bytes[1] = 0x0A;
	bytes[2] = 0x0A;
	sendBytesAudioI2C(bytes, 3);
	
	bytes[0] = 0x1F;
	bytes[1] = 0x0F;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x22;
	bytes[1] = 0xC0;
	sendBytesAudioI2C(bytes, 2);
	
	bytes[0] = 0x02;
	bytes[1] = 0x9E;
	sendBytesAudioI2C(bytes, 2);
	
}

void sendBytesAudioI2C(char *bytes, int num) {
	
	int i;
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);
	
	while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB));
	
	I2C_Send7bitAddress(I2C1, AUDIO_I2C_ADDRESS, I2C_Direction_Transmitter);
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	for (i = 0; i < num; i++) {
		I2C_SendData(I2C1, bytes[i]);
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}



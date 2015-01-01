/**
  ******************************************************************************
  * @file    main.c
  * @author  Ivan Pavic
  * @version V1.0
  * @date    01-January-2015
  * @brief   This file provides main function which should reproduce noise.
  *          
  ******************************************************************************
  */

#include "audio.h"
#include <stdlib.h>

int main() {
	configAudio();
	while(1) {
		if (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)) {
			SPI_I2S_SendData(SPI3, rand());
		}
	}
}

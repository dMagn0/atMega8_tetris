/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Advanced
Automatic Program Generator
� Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 07/08/2025
Author  : www.Eca.ir *** www.Webkade.ir
Company : 
Comments: 


Chip type               : ATmega8
Program type            : Application
AVR Core Clock frequency: 14,745600 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega8.h>
#include <delay.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "avr_utils.h"
#include "avr_utils.c"

// SPI functions
#include <spi.h>
#include "max7219.h"
#include "max7219.c"

#include "engine_jogo.h"
#include "engine_jogo.c"

void main(void)
{
  InputDoControle input_do_controle = {0,0,0};
  uint8_t data_to_spi[8] = {
      0b01100110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b01111110,
      0b00111100,
      0b00011000,
      0b00000000
  };

  srandom(1324);
  inicia_registradores();
  SPI_init();
  MAX7219_init();   
  MAX7219_display(data_to_spi);
  delay_ms(150);  

  while (1)
  {
    get_input(&input_do_controle);
    
    jogo(&input_do_controle, data_to_spi);

    MAX7219_display(data_to_spi);  
    delay_ms(18);
  }
}

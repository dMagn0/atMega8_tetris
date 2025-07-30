/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Advanced
Automatic Program Generator
� Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 11/04/2023
Author  : www.Eca.ir *** www.Webkade.ir
Company : 
Comments: 


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 14,745600 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega16.h>
#include <avr/io.h>
#include <delay.h>
#include <stdio.h>
#include "max7219.h"

#define MUX_ADC0 0b01000000
#define MUX_ADC1 0b01000001

uint8_t data_to_spi[8] = {
      0b01100110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b01111110,
      0b00111100,
      0b00011000,
      0b00000000
}
unsigned int read_adc(unsigned char adc_input)
{
      ADMUX = adc_input;
      delay_us(1);
      
      ADCSRA|=0x40;
      
      while ((ADCSRA & 0x10)==0);
      ADCSRA|=0x10;
      return ADCW;
}

void main(void)
{

      PORTA=0x00;
      DDRA=0x00;

      // SPI init 
      // MOSI (PB5), SCK (PB7) e SS (CS) (PB4) como saídas; 
      // MISO (PB6) como entrada;
      PORTB=0x00;
      DDRB=0b10110000;

      PORTC=0x00;
      DDRC=0x00;

      DDRD=0x00;

      TCCR0=0x00;
      TCNT0=0x00;
      OCR0=0x00;


      // Timer(s)/Counter(s) Interrupt(s) initialization
      TIMSK=0x00;

      // USART initialization
      // Communication Parameters: 8 Data, 1 Stop, No Parity
      // USART Receiver: Off
      // USART Transmitter: On
      // USART Mode: Asynchronous
      // USART Baud Rate: 19200
      UCSRA=0x00;
      UCSRB=0x08;
      UCSRC=0x86;
      UBRRH=0x00;
      UBRRL=0x2F;


      // SPI initialization
      // SPI enable, master, clk/16
      SPCR=0b01010001;

      // ADC initialization
      // ADC Clock frequency: 921,600 kHz
      // ADC Voltage Reference: Int., cap. on AREF
      // ADC Auto Trigger Source: ADC Stopped
      ADMUX = MUX_ADC0;
      ADCSRA = 0x84;

      spi_device *dev = max7219_init(0);

      while (1){


            max7219_set_data(dev, data_to_spi);
            delay_ms(20);
      }
}

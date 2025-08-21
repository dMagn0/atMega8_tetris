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
// #include <avr/io.h>
#include <delay.h>
#include <stdio.h>
#include <stdint.h>
#include "max7219.h"
#include <string.h>

#define MUX_ADC0_XAXIS 0b01000000
#define MUX_ADC1_YAXIS 0b01000001


unsigned int read_adc(unsigned char adc_input)
{
      ADMUX = adc_input;
      delay_us(1);
      
      ADCSRA|=0x40;
      
      while ((ADCSRA & 0x10)==0);
      ADCSRA|=0x10;
      return ADCW;
}

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

typedef struct {
    unsigned int x_axis;
    unsigned int y_axis;
    unsigned char stick_down;
} InputDoControle;

InputDoControle input = {0,0,0};

void get_input(){
      input.x_axis = read_adc(MUX_ADC0_XAXIS);
      input.y_axis = read_adc(MUX_ADC1_YAXIS);
      input.stick_down = STICK_DOWN;
}

uint8_t x_axis_table[] = {
            0b11110000,
            0b01110000,
            0b00110000,
            0b00010000,
            0b00000000,
            0b00010000,
            0b00011000,
            0b00011100,
            0b00011110,
            0b00011111
};

void cria_imagem(){

      uint8_t index = input.x_axis / 110;
      memset(data_to_spi, 0, sizeof(data_to_spi));
      
      if (index > 9) index = 9;
      data_to_spi[4] = x_axis_table[index];

      if(input.y_axis < 110){
            data_to_spi[7] += 0b00010000;
      }
      if(input.y_axis < 220){
            data_to_spi[6] = 0b00010000;
      }
      if(input.y_axis < 330){
            data_to_spi[5] = 0b00010000;
      }
      if(input.y_axis < 440){
            data_to_spi[4] |= 0b00010000;
      }
      if(input.y_axis > 550){
            data_to_spi[4] |= 0b00010000;
      }
      if(input.y_axis > 660){
            data_to_spi[3] = 0b00010000;
      }
      if(input.y_axis > 770){
            data_to_spi[2] = 0b00010000;
      }
      if(input.y_axis > 880){
            data_to_spi[1] = 0b00010000;
      }
      if(input.y_axis > 990){
            data_to_spi[0] = 0b00010000;
      }

}

void main(void)
{
      char i, j=0;

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
      ADMUX = MUX_ADC0_XAXIS;
      ADCSRA = 0x84;

      SPI_init();
      MAX7219_init();   // spi_device *dev = max7219_init(0);
      MAX7219_send(data_to_spi);    // max7219_set_data(dev, data_to_spi);
      delay_ms(150);

      for(i = 0; i < 8; i++){
            for(j=0;j<8;j++){
                  data_to_spi[j] = (data_to_spi[j] << 1);
            }
            MAX7219_send(data_to_spi);    // max7219_set_data(dev, data_to_spi);
            delay_ms(150);
      }

      while (1){

            get_input();

            cria_imagem();

            // printf("\r\nx_axis: %d\r\ny_axis: %d\r\nbutton: %d\r\n", input.x_axis, input.y_axis, input.stick_down);

            MAX7219_send(data_to_spi);    // max7219_set_data(dev, data_to_spi);
            delay_ms(18);
      }
}

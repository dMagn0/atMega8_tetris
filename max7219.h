#ifndef MAX7219_H
#define MAX7219_H

// #include <avr/io.h>
#include <stdint.h>

// Define os pinos SPI
#define SPI_DDR   DDRB
#define SPI_PORT  PORTB
#define MOSI      5
#define SCK       7
#define CS        4

// Funções públicas
void SPI_init(void);
void SPI_write(uint8_t data);
void MAX7219_send(uint8_t address, uint8_t data);
void MAX7219_init(void);
void MAX7219_display(uint8_t *data);

#endif

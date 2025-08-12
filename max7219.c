#include "max7219.h"

#define CS_LOW()  SPI_PORT &= ~(1<<CS)
#define CS_HIGH() SPI_PORT |=  (1<<CS)

void SPI_init(void) {
    // Define MOSI, SCK, CS como saída
    // SPI_DDR |= (1<<MOSI) | (1<<SCK) | (1<<CS);
    // Habilita SPI, modo mestre, clock fosc/16
    // SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
    CS_HIGH(); // Desativa o CS inicialmente
}

void SPI_write(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1<<SPIF)));
}

void MAX7219_send(uint8_t address, uint8_t data) {
    CS_LOW();
    SPI_write(address);
    SPI_write(data);
    CS_HIGH();
}

void MAX7219_init(void) {
    MAX7219_send(0x09, 0x00); // Decode mode: no decode
    MAX7219_send(0x0A, 0x0F); // Intensity: max
    MAX7219_send(0x0B, 0x07); // Scan limit: 8 digits
    MAX7219_send(0x0C, 0x01); // Shutdown: normal
    MAX7219_send(0x0F, 0x00); // Test: off
}

void MAX7219_display(uint8_t *data) {
    uint8_t i = 0;
    for (i = 0; i < 8; i++) {
        MAX7219_send(i + 1, data[i]);
    }
}

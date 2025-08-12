#ifndef AVR_UTILS_H
#define AVR_UTILS_H

#include <mega8.h>
#include <delay.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <spi.h>

#define ADC_VREF_TYPE 0b01000000

/*
    X_AXIS = ADC0 (PC0)
    Y_AXIS = ADC1 (PC1)
    S_DOWN = ADC2 (PC2)
*/
#define MUX_ADC0_XAXIS 0
#define MUX_ADC1_YAXIS 1
#define STICK_DOWN_PIN ~PINC.2

typedef struct {
    unsigned int x_axis;
    unsigned int y_axis;
    unsigned char stick_down;
} InputDoControle;

unsigned int read_adc(unsigned char);
void get_input(InputDoControle*){
void inicia_registradores();

#endif
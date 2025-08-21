#ifndef ENGINE_JOGO_H
#define ENGINE_JOGO_H

#include <mega8.h>
#include <delay.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "avr_utils.h"
#include "imagens.h"

void jogo(InputDoControle input_do_controle, uint8_t *data_to_spi);

#endif

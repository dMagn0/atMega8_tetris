#include "engine_jogo.h"

#define CENA_INIT 0
#define CENA_SELECT 1
#define CENA_CALIBRA 2
#define CENA_JOGO_COBRA 3

#define MENU_PARADO 0
#define MENU_CALIBRA_CONTROLE 1
#define MENU_JOGO_COBRA 2

#define DIREITA 0
#define BAIXO 1
#define ESQUERDA 2
#define CIMA 3

void reseta_marcadores(unsigned char* menu_auxiliar, uint16_t* count){
    *menu_auxiliar = 0;
    *count = 0;
    return;
}
void set_rng_seed(uint16_t contador){
    srand(contador);
    return;
}
void muda_fundo(uint8_t *data_to_spi, uint16_t contador){
    
    memcpy(data_to_spi, lista_de_imagens[contador%6], 8);
    return;
}
void desenha_tela_de_menu(uint8_t *data_to_spi, uint16_t val_menu){
    
    memcpy(data_to_spi, lista_de_imagens_menu[val_menu], 8);
    return;
}
void get_death_screen(uint8_t *data_to_spi){
    memcpy(data_to_spi, lista_de_imagens_menu[2], 8);
    return;
}

void jogo(InputDoControle *input_do_controle, uint8_t *data_to_spi){

    static unsigned char cena_principal = 0,menu_auxiliar = 0, i , j, k, fruta_x, fruta_y, bateu_flag;
    static uint16_t count = 0;
    uint16_t rng = 0;

    static uint8_t mapa_da_cobra[8][8];
    static uint8_t tamanho_da_cobra = 2;
    static uint8_t direcao_cobra = 0;

    switch(cena_principal){
        case CENA_INIT:
            if(count >= 300){

                if( count >= 460 ){
                    reseta_marcadores(&menu_auxiliar,&count);
                    menu_auxiliar = 0;
                    cena_principal = CENA_SELECT;
                    break;
                }

                if( count%20 == 19){
                    for(i = 0; i < 8; i++){
                        data_to_spi[i] = (data_to_spi[i] << 1); 
                    }
                }
            }

            count++;

        break;
        case CENA_SELECT:

            if(count >= 50){
                if(input_do_controle->x_axis >= 770){
                    menu_auxiliar = (menu_auxiliar+1)%3;
                    count = 0;
                }else if(input_do_controle->x_axis <= 250){
                    menu_auxiliar = (menu_auxiliar-1)%3;
                    count = 0;
                }
            }
            if ( count >= 65530 ){count = 0;}
            count ++;
            
            switch(menu_auxiliar){
                case MENU_PARADO:
                    if(count%240 == 0){
                        muda_fundo(data_to_spi, count);
                    }
                    if(count%20 == 19){
                        for(i = 0; i < 8; i++){
                            data_to_spi[i] = (data_to_spi[i] << 1); 
                        }
                    }

                break;
                case MENU_CALIBRA_CONTROLE:
                    if(count == 1){desenha_tela_de_menu(data_to_spi, 0);}
                    if(input_do_controle->stick_down == 1){
                        printf("calibr entra com input\r\n");
                        reseta_marcadores(&menu_auxiliar,&count);
                        cena_principal = CENA_INIT;
                    }
                break;
                case MENU_JOGO_COBRA:
                    if(count == 1){desenha_tela_de_menu(data_to_spi, 1);}
                    if(input_do_controle->stick_down == 1){
                        printf("entra com input\r\n");
                        set_rng_seed(count);
                        reseta_marcadores(&menu_auxiliar,&count);
                        cena_principal = CENA_JOGO_COBRA;
                    }
                break;
            }

        break;
        case CENA_CALIBRA:
        break;
        case CENA_JOGO_COBRA:
            //  0 1 2 3 4 5 6 7
            // [0,0,0,0,0,0,0,0]    0
            // [0,0,0,0,0,0,0,0]    1
            // [0,0,0,0,0,0,0,0]    2
            // [0,0,0,0,0,0,0,0]    3
            // [0,0,0,0,0,0,0,0]    4
            // [0,0,0,0,0,0,0,0]    5
            // [0,0,0,0,0,0,0,0]    6
            // [0,0,0,0,0,0,0,0]    7

            //inicia
            if(count == 0){
                for(i=0;i<8;i++){
                    for(j=0;j<8;j++){
                        mapa_da_cobra[i][j] = 0;
                    }
                }
                
                tamanho_da_cobra = 2;
                bateu_flag = 0;
                rng = rand()%4;
                switch (rng){
                    case DIREITA:
                        mapa_da_cobra[2][2] = 1;
                        mapa_da_cobra[2][3] = 2;
                        break;
                    case BAIXO:
                        mapa_da_cobra[5][2] = 1;
                        mapa_da_cobra[4][2] = 2;
                    break;
                    case ESQUERDA:
                        mapa_da_cobra[5][5] = 1;
                        mapa_da_cobra[5][4] = 2;
                    break;
                    case CIMA:
                        mapa_da_cobra[2][5] = 1;
                        mapa_da_cobra[3][5] = 2;
                    break;
                }
                direcao_cobra = rng;

                // gerar fruta
                while(1){
                    fruta_x = rand()%8;
                    fruta_y = rand()%8;
                    if(mapa_da_cobra[fruta_y][fruta_x] == 0){
                        break;
                    }
                }
            }


            if(bateu_flag == 1){
                if(count >=50){
                    get_death_screen(data_to_spi);
                }
                if(count >= 70){
                    cena_principal = CENA_INIT;
                    reseta_marcadores(&menu_auxiliar,&count);
                }
                count++;
                return;
            }

            switch(direcao_cobra){
                case DIREITA:
                    if(input_do_controle->y_axis >= 950){ // direita
                        direcao_cobra = CIMA;
                    }else if(input_do_controle->y_axis <= 80){
                        direcao_cobra = BAIXO;
                    }
                break;
                case BAIXO:
                    if(input_do_controle->x_axis >= 950){ // direita
                        direcao_cobra = DIREITA;
                    }else if(input_do_controle->x_axis <= 80){
                        direcao_cobra = ESQUERDA;
                    }
                break;
                case ESQUERDA:
                    if(input_do_controle->y_axis >= 950){ // direita
                        direcao_cobra = CIMA;
                    }else if(input_do_controle->y_axis <= 80){
                        direcao_cobra = BAIXO;
                    }
                break;
                case CIMA:
                    if(input_do_controle->x_axis >= 950){ // direita
                        direcao_cobra = DIREITA;
                    }else if(input_do_controle->x_axis <= 80){
                        direcao_cobra = ESQUERDA;
                    }
                break;
            }
            if(count >= 21){
                printf("move\r\n");
                for(i=0;i<8;i++){
                    for(j=0;j<8;j++){
                        if(mapa_da_cobra[i][j] == 0){
                            continue;
                        }
                        mapa_da_cobra[i][j] --;
                    }
                }

                k =0;
                for(i=0;i<8;i++){
                    for(j=0;j<8;j++){
                        if(mapa_da_cobra[i][j] == (tamanho_da_cobra -1)){
                            switch(direcao_cobra){
                                case DIREITA:
                                    if(j == 7){bateu_flag = 1;break;}
                                    if(mapa_da_cobra[i][j+1] != 0){bateu_flag = 1;break;}
                                    mapa_da_cobra[i][j+1] = tamanho_da_cobra;
                                break;
                                case BAIXO:
                                    if(i == 7){bateu_flag = 1;break;}
                                    if(mapa_da_cobra[i+1][j] != 0){bateu_flag = 1;break;}
                                    mapa_da_cobra[i+1][j] = tamanho_da_cobra;
                                break;
                                case ESQUERDA:
                                    if(j == 0){bateu_flag = 1;break;}
                                    if(mapa_da_cobra[i][j-1] != 0){bateu_flag = 1;break;}
                                    mapa_da_cobra[i][j-1] = tamanho_da_cobra;
                                break;
                                case CIMA:
                                    if(i == 0){bateu_flag = 1;break;}
                                    if(mapa_da_cobra[i-1][j] != 0){bateu_flag = 1;break;}
                                    mapa_da_cobra[i-1][j] = tamanho_da_cobra;
                                break;
                            }
                            k=1;
                            break;
                        }
                    }
                    if(k==1){
                        break;
                    }
                }

                if(bateu_flag == 0){

                    k=0;
                    for(i=0;i<8;i++){
                        for(j=0;j<8;j++){
                            if(mapa_da_cobra[i][j] == tamanho_da_cobra){
                                if((i == fruta_y) && (j == fruta_x)){
                                    tamanho_da_cobra++;
                                    for(i=0;i<8;i++){
                                        for(j=0;j<8;j++){
                                            if(mapa_da_cobra[i][j] == 0){
                                                continue;
                                            }
                                            mapa_da_cobra[i][j] ++;
                                        }
                                    }
                                    while(1){
                                        fruta_x = rand()%8;
                                        fruta_y = rand()%8;
                                        if(mapa_da_cobra[fruta_y][fruta_x] == 0){
                                            break;
                                        }
                                    }


                                }
                                k=1;
                                break;
                            }
                        }
                        if(k==1){break;}
                    }

                }

                count = 1;
            }
            count++;

            for(i=0;i<8;i++){
                data_to_spi[i] = 0xff;
                for(j=0;j<8;j++){
                    if(mapa_da_cobra[i][j] == 0){
                        data_to_spi[i] &= ~(1<<(7-j)); 
                    }
                }
            }
            if(count >=4 && count <= 16){
                data_to_spi[fruta_y] |= (1<<(7-fruta_x));
            }
            //transforma para data_to_spi
        break;

    }


    return;    
}

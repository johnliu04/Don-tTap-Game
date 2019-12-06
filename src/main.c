/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdlib.h>
			
extern void setup_dma_c3();
extern void setup_gpio();
extern void setup_spi1();
extern void setup_lcd();
extern void setup_tim2();
extern void setup_tim6();
extern void setup_dac();
extern void game();
extern void reset();

int main(void)
{
    reset();
    srand(42);

    setup_gpio();
    setup_spi1();
    setup_dma_c3();
    setup_lcd();
    setup_dac();
    setup_tim2();
    setup_tim6();
    while(1) {
        reset();
        game();
    }
}

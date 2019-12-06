#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <math.h>

extern int16_t arr[4];
extern uint16_t dispmem[34];

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void generic_lcd_startup(void) {
    nano_wait(100000000);   // Give it 100ms to initialize
    spi_cmd(0x38);              // 0011 NF00 N=1, F=0: two lines
    spi_cmd(0x0c);              // 0000 1DCB: display on, no cursor, no blink
    spi_cmd(0x01);              // clear entire display
    nano_wait(6200000);     // clear takes 6.2ms to complete
    spi_cmd(0x02);              // put the cursor in the home position
    spi_cmd(0x06);              // 0000 01IS: set display to increment
}

void setup_dma_c3() {
    // setup DMA Channel3 for SPI1_TX
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel3->CMAR = (uint32_t) (arr);
    DMA1_Channel3->CPAR = (uint32_t) (&SPI1->DR);
    DMA1_Channel3->CNDTR = 4;
    DMA1_Channel3->CCR |= DMA_CCR_DIR | DMA_CCR_CIRC | DMA_CCR_MINC;
    DMA1_Channel3->CCR &= ~(DMA_CCR_MSIZE | DMA_CCR_PSIZE);
    DMA1_Channel3->CCR |= DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0;
    DMA1_Channel3->CCR &= ~(DMA_CCR_PL_0 | DMA_CCR_PL_1);
    DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void setup_dma_c5() {
    // setup DMA Channel5 for SPI2_TX
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel5->CNDTR = 34;
    DMA1_Channel5->CPAR = (uint32_t) (&SPI2->DR);
    DMA1_Channel5->CMAR = (uint32_t) (dispmem);
    DMA1_Channel5->CCR |= DMA_CCR_DIR | DMA_CCR_CIRC;
    DMA1_Channel5->CCR |= DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0 | DMA_CCR_MINC;
    DMA1_Channel5->CCR &= ~(DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1 | DMA_CCR_PINC);
    DMA1_Channel5->CCR &= ~(DMA_CCR_PL_0 | DMA_CCR_PL_1);
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}


void setup_spi1() {
    // setup SPI1 for shift register controls
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 |= SPI_CR1_BR | SPI_CR1_MSTR;
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    SPI1->CR2 = SPI_CR2_DS | SPI_CR2_NSSP | SPI_CR2_SSOE | SPI_CR2_TXDMAEN;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void setup_spi2() {
    // setup SPI2 for LCD display
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    SPI2->CR1 &= ~SPI_CR1_SPE;
    SPI2->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_MSTR;
    SPI2->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1;
    SPI2->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    SPI2->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_3 | SPI_CR2_SSOE |
            SPI_CR2_NSSP | SPI_CR2_TXDMAEN;
    SPI2->CR1 |= SPI_CR1_SPE;
}

void setup_lcd() {
    // setup lcd screen
    setup_spi2();
    generic_lcd_startup();
    setup_dma_c5();
}

void setup_tim2() {
    // setup timer2 50kHz
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 240-1;
    TIM2->ARR = 4-1;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] |= 1 << TIM2_IRQn;
}

void setup_tim6() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC = 240-1;
    TIM6->ARR = 200-1;
    TIM6->DIER |= TIM_DIER_UIE;
    TIM6->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] |= 1 << TIM6_DAC_IRQn;
}

void setup_gpio() {
    // setup PA4 as analog
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER4;

    // setup PA0-3 as outputs, PA5-8 as inputs with pull-down resistors
    GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2
            | GPIO_MODER_MODER3 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6
            | GPIO_MODER_MODER7 | GPIO_MODER_MODER8);
    GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0
            | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0;
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6
            | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8);
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1 | GPIO_PUPDR_PUPDR6_1
            | GPIO_PUPDR_PUPDR7_1 | GPIO_PUPDR_PUPDR8_1;

    // setup PA15, PB3, PB5 for SPI1
    GPIOA->MODER &= ~GPIO_MODER_MODER15;
    GPIOA->MODER |= GPIO_MODER_MODER15_1;
    GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH7;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER5);
    GPIOB->MODER |= GPIO_MODER_MODER3_1 | GPIO_MODER_MODER5_1;
    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFRL3 | GPIO_AFRL_AFRL5);

    // setup PB12, PB13, PB15 for SPI2
    GPIOB->MODER &= ~(GPIO_MODER_MODER12 | GPIO_MODER_MODER13 | GPIO_MODER_MODER15);
    GPIOB->MODER |= GPIO_MODER_MODER12_1 | GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1;
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFRH7 | GPIO_AFRH_AFRH5 | GPIO_AFRH_AFRH4);
}


void setup_dac() {
    // setup dac, software trigger
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    DAC->CR &= ~DAC_CR_EN1;
    DAC->CR &= ~DAC_CR_BOFF1;
    DAC->CR |= DAC_CR_TEN1;
    DAC->CR |= DAC_CR_TSEL1;
    DAC->CR |= DAC_CR_EN1;
}


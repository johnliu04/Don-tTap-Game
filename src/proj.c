#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#define L1 262

#define L1hash 277

#define L2 294

#define L2hash 311

#define L3 330

#define L4 349

#define L4hash 370

#define L5 392

#define L5hash 415

#define L6 440

#define L6hash 466

#define L7 494


#define M1 523

#define M1hash 554

#define M2 587

#define M2hash 622

#define M3 659

#define M4 698

#define M4hash 740

#define M5 784

#define M5hash 831

#define M6 880

#define M6hash 932

#define M7 988


#define H1 1046

#define H1hash 1109

#define H2 1175

#define H2hash 1245

#define H3 1318

#define H4 1397

#define H4hash 1480

#define H5 1568

#define H5hash 1661

#define H6 1760

#define H6hash  1865

#define H7 1976


// const wave table
const short int wavetable[1000] = {
0, 205, 411, 617, 823, 1029, 1234, 1440,
1646, 1851, 2057, 2262, 2468, 2673, 2878, 3083,
3288, 3493, 3697, 3902, 4106, 4310, 4514, 4718,
4922, 5125, 5329, 5532, 5734, 5937, 6139, 6342,
6543, 6745, 6946, 7147, 7348, 7549, 7749, 7949,
8148, 8348, 8546, 8745, 8943, 9141, 9339, 9536,
9733, 9929, 10125, 10321, 10516, 10711, 10905, 11099,
11292, 11485, 11678, 11870, 12062, 12253, 12444, 12634,
12824, 13013, 13202, 13390, 13577, 13764, 13951, 14137,
14322, 14507, 14692, 14875, 15059, 15241, 15423, 15604,
15785, 15965, 16145, 16324, 16502, 16679, 16856, 17032,
17208, 17383, 17557, 17730, 17903, 18075, 18247, 18417,
18587, 18756, 18925, 19093, 19259, 19426, 19591, 19756,
19920, 20083, 20245, 20406, 20567, 20727, 20886, 21044,
21202, 21358, 21514, 21669, 21823, 21976, 22128, 22280,
22430, 22580, 22728, 22876, 23023, 23169, 23314, 23459,
23602, 23744, 23886, 24026, 24166, 24304, 24442, 24578,
24714, 24849, 24982, 25115, 25247, 25378, 25507, 25636,
25764, 25891, 26016, 26141, 26264, 26387, 26509, 26629,
26748, 26867, 26984, 27100, 27216, 27330, 27443, 27555,
27666, 27775, 27884, 27992, 28098, 28203, 28308, 28411,
28513, 28614, 28713, 28812, 28910, 29006, 29101, 29195,
29288, 29380, 29470, 29560, 29648, 29735, 29821, 29906,
29989, 30072, 30153, 30233, 30312, 30389, 30465, 30541,
30615, 30687, 30759, 30829, 30898, 30966, 31033, 31099,
31163, 31226, 31288, 31348, 31407, 31465, 31522, 31578,
31632, 31685, 31737, 31788, 31837, 31885, 31932, 31977,
32022, 32065, 32106, 32147, 32186, 32224, 32261, 32296,
32330, 32363, 32395, 32425, 32454, 32482, 32508, 32533,
32557, 32580, 32601, 32621, 32640, 32657, 32673, 32688,
32702, 32714, 32725, 32735, 32743, 32750, 32756, 32761,
32764, 32766, 32767, 32766, 32764, 32761, 32756, 32750,
32743, 32735, 32725, 32714, 32702, 32688, 32673, 32657,
32640, 32621, 32601, 32580, 32557, 32533, 32508, 32482,
32454, 32425, 32395, 32363, 32330, 32296, 32261, 32224,
32186, 32147, 32106, 32065, 32022, 31977, 31932, 31885,
31837, 31788, 31737, 31685, 31632, 31578, 31522, 31465,
31407, 31348, 31288, 31226, 31163, 31099, 31033, 30966,
30898, 30829, 30759, 30687, 30615, 30541, 30465, 30389,
30312, 30233, 30153, 30072, 29989, 29906, 29821, 29735,
29648, 29560, 29470, 29380, 29288, 29195, 29101, 29006,
28910, 28812, 28713, 28614, 28513, 28411, 28308, 28203,
28098, 27992, 27884, 27775, 27666, 27555, 27443, 27330,
27216, 27100, 26984, 26867, 26748, 26629, 26509, 26387,
26264, 26141, 26016, 25891, 25764, 25636, 25507, 25378,
25247, 25115, 24982, 24849, 24714, 24578, 24442, 24304,
24166, 24026, 23886, 23744, 23602, 23459, 23314, 23169,
23023, 22876, 22728, 22580, 22430, 22280, 22128, 21976,
21823, 21669, 21514, 21358, 21202, 21044, 20886, 20727,
20567, 20406, 20245, 20083, 19920, 19756, 19591, 19426,
19259, 19093, 18925, 18756, 18587, 18417, 18247, 18075,
17903, 17730, 17557, 17383, 17208, 17032, 16856, 16679,
16502, 16324, 16145, 15965, 15785, 15604, 15423, 15241,
15059, 14875, 14692, 14507, 14322, 14137, 13951, 13764,
13577, 13390, 13202, 13013, 12824, 12634, 12444, 12253,
12062, 11870, 11678, 11485, 11292, 11099, 10905, 10711,
10516, 10321, 10125, 9929, 9733, 9536, 9339, 9141,
8943, 8745, 8546, 8348, 8148, 7949, 7749, 7549,
7348, 7147, 6946, 6745, 6543, 6342, 6139, 5937,
5734, 5532, 5329, 5125, 4922, 4718, 4514, 4310,
4106, 3902, 3697, 3493, 3288, 3083, 2878, 2673,
2468, 2262, 2057, 1851, 1646, 1440, 1234, 1029,
823, 617, 411, 205, 0, -205, -411, -617,
-823, -1029, -1234, -1440, -1646, -1851, -2057, -2262,
-2468, -2673, -2878, -3083, -3288, -3493, -3697, -3902,
-4106, -4310, -4514, -4718, -4922, -5125, -5329, -5532,
-5734, -5937, -6139, -6342, -6543, -6745, -6946, -7147,
-7348, -7549, -7749, -7949, -8148, -8348, -8546, -8745,
-8943, -9141, -9339, -9536, -9733, -9929, -10125, -10321,
-10516, -10711, -10905, -11099, -11292, -11485, -11678, -11870,
-12062, -12253, -12444, -12634, -12824, -13013, -13202, -13390,
-13577, -13764, -13951, -14137, -14322, -14507, -14692, -14875,
-15059, -15241, -15423, -15604, -15785, -15965, -16145, -16324,
-16502, -16679, -16856, -17032, -17208, -17383, -17557, -17730,
-17903, -18075, -18247, -18417, -18587, -18756, -18925, -19093,
-19259, -19426, -19591, -19756, -19920, -20083, -20245, -20406,
-20567, -20727, -20886, -21044, -21202, -21358, -21514, -21669,
-21823, -21976, -22128, -22280, -22430, -22580, -22728, -22876,
-23023, -23169, -23314, -23459, -23602, -23744, -23886, -24026,
-24166, -24304, -24442, -24578, -24714, -24849, -24982, -25115,
-25247, -25378, -25507, -25636, -25764, -25891, -26016, -26141,
-26264, -26387, -26509, -26629, -26748, -26867, -26984, -27100,
-27216, -27330, -27443, -27555, -27666, -27775, -27884, -27992,
-28098, -28203, -28308, -28411, -28513, -28614, -28713, -28812,
-28910, -29006, -29101, -29195, -29288, -29380, -29470, -29560,
-29648, -29735, -29821, -29906, -29989, -30072, -30153, -30233,
-30312, -30389, -30465, -30541, -30615, -30687, -30759, -30829,
-30898, -30966, -31033, -31099, -31163, -31226, -31288, -31348,
-31407, -31465, -31522, -31578, -31632, -31685, -31737, -31788,
-31837, -31885, -31932, -31977, -32022, -32065, -32106, -32147,
-32186, -32224, -32261, -32296, -32330, -32363, -32395, -32425,
-32454, -32482, -32508, -32533, -32557, -32580, -32601, -32621,
-32640, -32657, -32673, -32688, -32702, -32714, -32725, -32735,
-32743, -32750, -32756, -32761, -32764, -32766, -32767, -32766,
-32764, -32761, -32756, -32750, -32743, -32735, -32725, -32714,
-32702, -32688, -32673, -32657, -32640, -32621, -32601, -32580,
-32557, -32533, -32508, -32482, -32454, -32425, -32395, -32363,
-32330, -32296, -32261, -32224, -32186, -32147, -32106, -32065,
-32022, -31977, -31932, -31885, -31837, -31788, -31737, -31685,
-31632, -31578, -31522, -31465, -31407, -31348, -31288, -31226,
-31163, -31099, -31033, -30966, -30898, -30829, -30759, -30687,
-30615, -30541, -30465, -30389, -30312, -30233, -30153, -30072,
-29989, -29906, -29821, -29735, -29648, -29560, -29470, -29380,
-29288, -29195, -29101, -29006, -28910, -28812, -28713, -28614,
-28513, -28411, -28308, -28203, -28098, -27992, -27884, -27775,
-27666, -27555, -27443, -27330, -27216, -27100, -26984, -26867,
-26748, -26629, -26509, -26387, -26264, -26141, -26016, -25891,
-25764, -25636, -25507, -25378, -25247, -25115, -24982, -24849,
-24714, -24578, -24442, -24304, -24166, -24026, -23886, -23744,
-23602, -23459, -23314, -23169, -23023, -22876, -22728, -22580,
-22430, -22280, -22128, -21976, -21823, -21669, -21514, -21358,
-21202, -21044, -20886, -20727, -20567, -20406, -20245, -20083,
-19920, -19756, -19591, -19426, -19259, -19093, -18925, -18756,
-18587, -18417, -18247, -18075, -17903, -17730, -17557, -17383,
-17208, -17032, -16856, -16679, -16502, -16324, -16145, -15965,
-15785, -15604, -15423, -15241, -15059, -14875, -14692, -14507,
-14322, -14137, -13951, -13764, -13577, -13390, -13202, -13013,
-12824, -12634, -12444, -12253, -12062, -11870, -11678, -11485,
-11292, -11099, -10905, -10711, -10516, -10321, -10125, -9929,
-9733, -9536, -9339, -9141, -8943, -8745, -8546, -8348,
-8148, -7949, -7749, -7549, -7348, -7147, -6946, -6745,
-6543, -6342, -6139, -5937, -5734, -5532, -5329, -5125,
-4922, -4718, -4514, -4310, -4106, -3902, -3697, -3493,
-3288, -3083, -2878, -2673, -2468, -2262, -2057, -1851,
-1646, -1440, -1234, -1029, -823, -617, -411, -205};

// step sizes for wavegen (currently arbitrary)
const int32_t step[16] =
{880.00/50*(1<<16), 1318.51/50*(1<<16), 493.88/50*(1<<16), 698.46/50*(1<<16),
987.77/50*(1<<16), 1396.91/50*(1<<16), 523.25/50*(1<<16), 783.99/50*(1<<16),
1046.50/50*(1<<16), 1567.98/50*(1<<16), 587.33/50*(1<<16), 1975.53/50*(1<<16),
1174.66/50*(1<<16), 440.00/50*(1<<16), 659.25/50*(1<<16), 1760.00/50*(1<<16)};



const int musictable[211] = {

H3, H2hash, H3, H2hash, H3, M7, H2hash, H1, M6, M1, M3, M6, M7, M3, M5hash, M7, H1, M3, H3, H2hash, //20

H3, H2hash, H3, M7, H2hash, H1, M6, M1, M3, M6, M7, M3, H1, M7, M6, H3, H2hash, H3, H2hash, H3, M7, H2hash, H1, // 23

M6, M1, M3, M6, M7, M3, M5hash, M7, H1, M3, H3, H2hash, H3, H2hash, H3, M7, H2hash, H1, M6, M1, M3, M6, // 22

M7, M3, H1, M7, M6, M7, H1, H2, H3, M5, H4, H3, H2, M4, H3, H2, H1, M3, H2, H1, // 20

M7, M3, H3, M3, H3, M3, H3, H2hash, H3, H2hash, H3, H2hash, H3, H2hash, H3, H2hash, H3, H2hash, H3, M7, H2hash, H1, // 22

M6, M1, M3, M6, M7, M3, H1, M7, M6, M7, H1, H2, H3, M5, H4, H3, H2, M4, H3, H2, // 20

H1, M3, H2, H1, M7, M3, H3, M3, H3, M3, H3, H2hash, H3, H2hash, H3, H2hash, H3, H2hash, H3, H2hash, // 20

H3, H2hash, H3, M7, H2hash, H1, M6, M1, M3, M6, M7, M3, M5hash, M7, H1, M3, H3, H2hash, H3, H2hash, H3, M7, H2hash, H1, // 24

M6, M1, M3, M6, M7, M3, H1, M7, M6, M1, M1, M1, M1, M4, M3, M2, M7, M6, // 18

M6, M5, M4, M3, M2, M1, L7, L6, L6, L5, L6, L7, M1, M2, M3, M3, M3, M4, L6, M1, M2, L7 // 22

};

/*
 * INDEXING
 * 0   4   8   12
 * 1   5   9   13
 * 2   6   10  14
 * 3   7   11  15
 */

int16_t pressed[16] = {0};   // 0 not pressed, pressed count to 100
int8_t light[16] = {0};     // 0 not lit, 1 lit
int8_t point[16] = {0};     // 0 not lit, 1 lit
int32_t offset = 0;   // offset for wavegen
int8_t history[16] = {0};   // history for key scan
int8_t col = 0;             // current column to drive for key scan
int16_t keyscan_ct = 0;     // key scan counter
int16_t cd_ct = 0;          // count down counter
int16_t gen_ct = 0;         // general counter
int8_t count;               // count time
int16_t score = 0;          // player score
int16_t arr[4] = {0};       // 16-bit arr to transmit to shift registers
int8_t status = 0;
int16_t high = 0;
int count_music = 0;

#define RED 0
#define GREEN 1
#define BLUE 2

void gameover();
int rand_gen();
void start_message();
void light_led(int8_t i, int8_t c);
void clear_led(int8_t i);
void saneryigo();


uint16_t dispmem[34] = {
        0x080 + 0,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x080 + 64,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
        0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220, 0x220,
};

void spi_cmd(char b) {
    while((SPI2->SR & SPI_SR_TXE) == 0);
    SPI2->DR = b;
}

void spi_data(char b) {
    while((SPI2->SR & SPI_SR_TXE) == 0);
    SPI2->DR = 0x200 + b;
}

void display1(const char *s) {
    // lcd line1 display
    int x;
    for(x=0; x<16; x+=1)
        if (s[x])
            dispmem[x+1] = 0x200 + s[x];
        else
            break;
    for(   ; x<16; x+=1)
        dispmem[x+1] = 0x220;
}

void display2(const char *s) {
    // lcd line2 display
    int x;
    for(x=0; x<16; x+=1)
        if (s[x])
            dispmem[x+18] = 0x200 + s[x];
        else
            break;
    for(   ; x<16; x+=1)
        dispmem[x+18] = 0x220;
}

void countdown() {
    char line[20];
    cd_ct++;
    if(cd_ct == 1000) {
        cd_ct = 0;
        if(count > 0) {
            count--;
        }
        sprintf(line, "TIME: %d ", count);
        display1(line);
        // if count == 0 end game
    }
}

void keyscan() {
    // keyscan
    int row = (GPIOA->IDR >> 5) & 0xf;
    int index = col << 2;
    history[index] = history[index] << 1;
    history[index] |= row & 0x1;
    history[index + 1] = history[index + 1] << 1;
    history[index + 1] |= (row >> 1) & 0x1;
    history[index + 2] = history[index + 2] << 1;
    history[index + 2] |= (row >> 2) & 0x1;
    history[index + 3] = history[index + 3] << 1;
    history[index + 3] |= (row >> 3) & 0x1;
    col++;
    if (col > 3)
        col = 0;
    GPIOA->ODR = 1 << col;
}

int get_key_press() {
    // detect key press
    while(1)
    {
        for(int i = 0; i < 16; i++) {
            if (count <= 0) return -1;
            if (history[i] == 1) {
                return i;
            }
        }
    }
}

int get_key_release() {
    // detect key release
    while(1)
    {
        for(int i = 0; i < 16; i++) {
            if (count <= 0) return -1;
            if (history[i] == -2) {
                return i;
            }
        }
    }
}

int get_key_pressed() {
    int key = get_key_press();
    int key2;
    do {
       key2 = get_key_release();
       if (key2 == -1) break;
    } while(key != key2);
    return key2;
}

void saneryigo() {
    char line1[20];
    sprintf(line1, " ");
    display2(line1);
    for (int i = 3; i > 0; i--) {
        gen_ct = 0;
        sprintf(line1, "       %d", i);
        display1(line1);
        while(gen_ct < 1000);
    }
    gen_ct = 0;
    sprintf(line1, "!!!!!!!GO!!!!!!!");
    display1(line1);
    while(gen_ct < 1000);
}

void game() {
    status = 0;
    for (int k = 0; k < 16; k++) {
        clear_led(k);
    }
    start_message(); // press the green button to start

    saneryigo();
    // generate 3 random LEDs to start
    light_led(rand_gen(), GREEN);
    light_led(rand_gen(), GREEN);
    light_led(rand_gen(), GREEN);
    char line[20];


    cd_ct = 999;
    status = 1;
    sprintf(line, "SCORE: %d ", score);
    display2(line);

    int i;
    int temp = 0;
    while(count > 0) {
        i = get_key_pressed();
        if (i == -1) break;
        // Increment timer
        if (temp >= 30){
            temp = 0;
            count += 10;
        }

        if (point[i] != 1) {
            clear_led(i);
            break;
        }
        else {
            count_music++;
            if (count_music > 210) count_music = 0;
            offset = 0;
            pressed[i] = 1;
            light_led(i, RED);
            light_led(rand_gen(), GREEN); // light a new random LED
            score++;
            sprintf(line, "SCORE: %d ", score);
            display2(line);
            temp++;
            point[i] = 0;
            //light_led(i, GREEN);
        }
    }
    gameover();
}

void start_message(){
    char line1[20];
    char line2[20];
    sprintf(line1, "Press the BLUE ");
    display1(line1);
    sprintf(line2, "button to START ");
    display2(line2);

    //srand(gen_ct);
    int start_button = rand() % 16;
    while (start_button < 4) start_button = rand() % 16;
    light_led(start_button, BLUE);
    int i;
    while(1){
        i = get_key_pressed();
        if(light[i] == 1){
            clear_led(i);
            return;
        }
    }
}

void gameover(){
    // all red
    status = 0;
    arr[0] = 0x8000;
    arr[1] = 0x4000;
    arr[2] = 0x2000;
    arr[3] = 0x1000;

    char line1[20];
    sprintf(line1, "GAME OVER");
    display1(line1);
    int i = 0;
    if (score > high) {
        high = score;
        char line1[20], line2[20];
        sprintf(line1, "!!!!NEW BEST!!!!");
        sprintf(line2, "HIGH SCORE: %d", high);
        display1(line1);
        display2(line2);
    }
    while (i < 9) {
        if(gen_ct >= 700){
            i++;
            gen_ct = 0;
            // toggle between red and none
            arr[0] ^= 0x924; // 0000 100 100 100 100
            arr[1] ^= 0x924;
            arr[2] ^= 0x924;
            arr[3] ^= 0x924;
        }
    }
}

// generate a random number each time
int rand_gen(){
    //srand(gen_ct);
    int number;
    do{
        number = rand() % 16;
    } while(light[number] == 1);
    light[number] = 1;
    return number;
}

void TIM6_DAC_IRQHandler(void) {
    TIM6->SR &= ~TIM_SR_UIF;
    keyscan();
    gen_ct++;
    if (gen_ct > 1200) gen_ct = 0;
    if (status) countdown();
}

void TIM2_IRQHandler(void) {
    // dac trigger at 100kHz
    TIM2->SR &= ~TIM_SR_UIF;
    DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;

    int i, sample = 0;
    // mix notes
    for (i = 0; i < 16; i++) {
        if (pressed[i] > 0) {
            pressed[i]++;
            offset += musictable[count_music] / 50 * (1 << 16);
            if (offset >= 1000 << 16) offset -= 1000 << 16;
            sample += wavetable[offset >> 16];
            if (pressed[i] == 15000) clear_led(i);
            if (pressed[i] > 15000) pressed[i] = 0;
        }
    }
    sample = sample / 16 + 2048;
    // clip and store
    if (sample > 4095) sample = 4095;
    else if (sample < 0) sample = 0;
    DAC->DHR12R1 = sample;
}

void light_led(int8_t i, int8_t c) {
    light[i] = 1;
    point[i] = 1;
    switch (i) {
        case 0:
            arr[0] |= 0x8000 | 0x0800 >> c;
            break;
        case 4:
            arr[0] |= 0x8000 | 0x0100 >> c;
            break;
        case 8:
            arr[0] |= 0x8000 | 0x0020 >> c;
            break;
        case 12:
            arr[0] |= 0x8000 | 0x0004 >> c;
            break;
        case 1:
            arr[1] |= 0x4000 | 0x0800 >> c;
            break;
        case 5:
            arr[1] |= 0x4000 | 0x0100 >> c;
            break;
        case 9:
            arr[1] |= 0x4000 | 0x0020 >> c;
            break;
        case 13:
            arr[1] |= 0x4000 | 0x0004 >> c;
            break;
        case 2:
            arr[2] |= 0x2000 | 0x0800 >> c;
            break;
        case 6:
            arr[2] |= 0x2000 | 0x0100 >> c;
            break;
        case 10:
            arr[2] |= 0x2000 | 0x0020 >> c;
            break;
        case 14:
            arr[2] |= 0x2000 | 0x0004 >> c;
            break;
        case 3:
            arr[3] |= 0x1000 | 0x0800 >> c;
            break;
        case 7:
            arr[3] |= 0x1000 | 0x0100 >> c;
            break;
        case 11:
            arr[3] |= 0x1000 | 0x0020 >> c;
            break;
        case 15:
            arr[3] |= 0x1000 | 0x0004 >> c;
            break;
    }
}

void clear_led(int8_t i) {
    light[i] = 0;
    point[i] = 0;
    switch (i) {
        case 0:
            arr[0] &= ~0x0E00;
            break;
        case 4:
            arr[0] &= ~0x01C0;
            break;
        case 8:
            arr[0] &= ~0x0038;
            break;
        case 12:
            arr[0] &= ~0x0007;
            break;
        case 1:
            arr[1] &= ~0x0E00;
            break;
        case 5:
            arr[1] &= ~0x01C0;
            break;
        case 9:
            arr[1] &= ~0x0038;
            break;
        case 13:
            arr[1] &= ~0x0007;
            break;
        case 2:
            arr[2] &= ~0x0E00;
            break;
        case 6:
            arr[2] &= ~0x01C0;
            break;
        case 10:
            arr[2] &= ~0x0038;
            break;
        case 14:
            arr[2] &= ~0x0007;
            break;
        case 3:
            arr[3] &= ~0x0E00;
            break;
        case 7:
            arr[3] &= ~0x01C0;
            break;
        case 11:
            arr[3] &= ~0x0038;
            break;
        case 15:
            arr[3] &= ~0x0007;
            break;
    }
}

void reset() {
    int i;
    for (i=0; i<16; i++) {
        light[i] = 0;
        pressed[i] = 0;
        offset = 0;
        history[i] = 0;
        point[i] = 0;
    }
    // pressed[1] = 1;
    for (i=0; i<4; i++) {
        arr[i] = 0;
    }
    col = 0;
    keyscan_ct = 0;
    cd_ct = 0;
    count = 11;
    score = 0;
}

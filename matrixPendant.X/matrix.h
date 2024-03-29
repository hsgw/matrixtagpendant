/* 
 * File:   matrix.h
 * Author: urkwtky
 *
 * Created on 2012/09/26, 2:04
 */

#ifndef MATRIX_H
#define	MATRIX_H

#define TRISA_DEFAULT 0b001000

#define TRISA_LED1  0b111100
#define TRISA_LED2  0b111001
#define TRISA_LED3  0b101011
#define TRISA_LED4  0b001111
#define TRISA_LED5  0b111100
#define TRISA_LED6  0b111001
#define TRISA_LED7  0b101011
#define TRISA_LED8  0b001111
#define TRISA_LED9  0b111010
#define TRISA_LED10 0b101101
#define TRISA_LED11 0b011011
#define TRISA_LED12 0b011101
#define TRISA_LED13 0b111010
#define TRISA_LED14 0b101101
#define TRISA_LED15 0b011011
#define TRISA_LED16 0b011101
#define TRISA_LED17 0b101110
#define TRISA_LED18 0b101110
#define TRISA_LED19 0b011110
#define TRISA_LED20 0b011110

#define PORTA_LED1  0b000001
#define PORTA_LED2  0b000010
#define PORTA_LED3  0b000100
#define PORTA_LED4  0b010000
#define PORTA_LED5  0b000010
#define PORTA_LED6  0b000100
#define PORTA_LED7  0b010000
#define PORTA_LED8  0b100000
#define PORTA_LED9  0b000001
#define PORTA_LED10 0b000010
#define PORTA_LED11 0b000100
#define PORTA_LED12 0b000010
#define PORTA_LED13 0b000100
#define PORTA_LED14 0b010000
#define PORTA_LED15 0b100000
#define PORTA_LED16 0b100000
#define PORTA_LED17 0b000001
#define PORTA_LED18 0b010000
#define PORTA_LED19 0b000001
#define PORTA_LED20 0b100000

#define PORTA_MASK  0b001000

const char TRISA_LED[20] = {
    TRISA_LED1,TRISA_LED2,TRISA_LED3,TRISA_LED4,TRISA_LED5,
    TRISA_LED6,TRISA_LED7,TRISA_LED8,TRISA_LED9,TRISA_LED10,
    TRISA_LED11,TRISA_LED12,TRISA_LED13,TRISA_LED14,TRISA_LED15,
    TRISA_LED16,TRISA_LED17,TRISA_LED18,TRISA_LED19,TRISA_LED20
};

const char PORTA_LED[20]= {
    PORTA_LED1,PORTA_LED2,PORTA_LED3,PORTA_LED4,PORTA_LED5,
    PORTA_LED6,PORTA_LED7,PORTA_LED8,PORTA_LED9,PORTA_LED10,
    PORTA_LED11,PORTA_LED12,PORTA_LED13,PORTA_LED14,PORTA_LED15,
    PORTA_LED16,PORTA_LED17,PORTA_LED18,PORTA_LED19,PORTA_LED20
};
#endif	/* MATRIX_H */
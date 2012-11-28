#include <pic.h>
#include "matrix.h"
#include "frame.h"

#define SWPIN PORTAbits.RA3

#define TMR1_RESET 25536
#define BOUNCE_TIME 2

#define AUTO_TIME 2000

//config
__CONFIG(
        FOSC_INTOSC & WDTE_OFF & PWRTE_ON & MCLRE_OFF & CP_OFF
        & CPD_OFF & BOREN_NSLEEP & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF
        );

__CONFIG(
        WRT_OFF & PLLEN_OFF & STVREN_ON & BORV_LO & LVP_OFF
        );

volatile static unsigned char ledCount = 0;
volatile static unsigned char delayCount = 0;
volatile static unsigned int autoModedelayCount = 0;

volatile static unsigned char swState = 1;
volatile static unsigned char swChange = 0;
volatile static unsigned char bounceCount = 0;
volatile static unsigned char duty = 0;

unsigned char buf[5] = {0x0F, 0x0F, 0x0F, 0x0F, 0x0F};

unsigned char text[21];
unsigned char textCode[21];
unsigned char textNum = 0;

volatile static unsigned char mode = 0;
volatile static unsigned char modeChange = 0;
volatile static unsigned char autoMode = 0;

/****************************
 *function
 ****************************/
void loadFrame(const unsigned char *frame) {
    buf[0] = frame[0] & 0xFF;
    buf[1] = frame[0] >> 4;
    buf[2] = frame[1] & 0x0F;
    buf[3] = frame[1] >> 4;
    buf[4] = frame[2] & 0x0F;
}

void loadFrameScroll(const unsigned char *frame) {
    buf[0] |= (frame[0] & 0x0F) << 4;
    buf[1] |= frame[0] & 0xF0;
    buf[2] |= (frame[1] & 0x0F) << 4;
    buf[3] |= frame[1] & 0xF0;
    buf[4] |= (frame[2] & 0x0F) << 4;
}



void readText() {
    textNum = EEPROM_READ(0x00);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            text[i * 8 + j] = EEPROM_READ(8 + 8 * i + j);
        }
    }
    for (int i = 17; i < 20; i++) {
        text[i] = EEPROM_READ(i);
    }
    text[20] = ' ';

    for (int i = 0; i < textNum; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') textCode[i] = text[i] - 55;
        else if (text[i] >= '0' && text[i] <= '9') textCode[i] = text[i] - 48;
        else textCode[i] = 51; //space
    }
}

void writeEEPROM() {
    EEPROM_WRITE(0x00, textNum);
    for (int i = 0; i < 21; i++) {
        EEPROM_WRITE(i + 8, text[i]);
    }
}

void putChar(unsigned char c) {
    while (TXIF == 0) {
        NOP();
    }
    TXREG = c;
}

//MODE*************************************

void animation(const unsigned char frameAnime[][3], unsigned char frameCount, unsigned char delay) {
    if(autoMode==1) autoModedelayCount = AUTO_TIME;
    while (1) {
        for (int i = 0; i < frameCount; i++) {
            loadFrame(frameAnime[i]);
            ledCount = 0;
            delayCount = delay;
            while (delayCount != 0) {
                if (modeChange == 1) {
                    modeChange = 0;
                    goto end;
                }
            }
        }
    }
end:
    ;
}

void textBlink() {
    if(autoMode==1) autoModedelayCount = AUTO_TIME;
    while (1) {
        for (int i = 0; i < textNum; i++) {
            loadFrame(frameChar[textCode[i]]);
            ledCount = 0;
            delayCount = 100;
            while (delayCount != 0) {
                if (modeChange == 1) {
                    modeChange = 0;
                    goto end;
                }
            }
        }
    }
end:
    ;
}

void textScroll() {
    if(autoMode==1) autoModedelayCount = AUTO_TIME;
    while (1) {
        for (int i = 0; i < textNum; i++) {
            loadFrameScroll(frameChar[textCode[i]]);
            for (int j = 0; j < 5; j++) {
                ledCount = 0;
                delayCount = 15;
                while (delayCount != 0) {
                    if (modeChange == 1) {
                        modeChange = 0;
                        goto end;
                    }
                }
                for(int k=0;k<5;k++){
                    buf[k] = buf[k] >> 1;
                }
            }
        }
    }
end:
    ;
}

/****************************
 *main
 ****************************/

void main() {
#ifdef OSC8MHZ
    OSCCON = 0b01110010; //OSC=16Mhz
#else
    OSCCON = 0b01111010; //OSC=16Mhz
#endif
    TRISAbits.TRISA3 = 0x01;
    ANSELA = 0x0;

    if (PORTAbits.RA3 == 0) {
        //serial
        RXDTSEL = 0x01;
        TXCKSEL = 0x01;
        TXSTA = 0b00100000;
        RCSTA = 0b10010000;
        SPBRG = 25; //16MHz 9600bps
        BRG16 = 0;
        //TRISA = 0b100000;
        TRISAbits.TRISA5 = 0x01;
        TRISAbits.TRISA4 = 0x00;

        PIR1bits.RCIF = 0x00;
        PIE1bits.RCIE = 0x01;

        INTCONbits.PEIE = 0x01;
        INTCONbits.GIE = 0x01;

        putChar('s');
        putChar(0x0D);
        putChar(0x0A);

        while (1) {
        }

    } else {
        //normal

        //timer0
        //OPTION_REG = 0b00000001;    //PS=4 255us
        OPTION_REG = 0b00000010; //PS=8 510us
        INTCONbits.TMR0IF = 0x00;
        TMR0bits.TMR0 = 0;
        INTCONbits.TMR0IE = 0x01;

        T1CON = 0b01100001; //1/FOSC*65535 PS=4 16ms
        PIR1bits.TMR1IF = 0x00;
        TMR1H = (TMR1_RESET >> 8);
        TMR1L = (TMR1_RESET & 0xFF);
        PIE1bits.TMR1IE = 0x01;

        INTCONbits.IOCIE = 0x01;
        IOCAPbits.IOCAP3 = 0x01;
        IOCANbits.IOCAN3 = 0x01;

        INTCONbits.PEIE = 0x01;
        INTCONbits.GIE = 0x01;

        readText();

        delayCount = 100;
        while (delayCount != 0) {
        NOP();
        }

        while (1) {
            switch (mode) {
                case 0:
                    autoMode = 1;
                    mode = 1;
                case 1:
                    animation(&frameTetris, 33, 25);
                    break;
                case 2:
                    animation(&frameSine, 8, 10);
                    break;
                case 3:
                    animation(&frameArrow, 4, 10);
                    break;
                case 4:
                    animation(&frameBox, 10, 10);
                    break;
                case 5:
                    textBlink();
                    break;
                case 6:
                    textScroll();
                default:
                    mode = 1;
                    break;
            }
        }
    }
}

void interrupt isr() {
    unsigned char tempLedState;
    if (INTCONbits.TMR0IF == 1) {
        INTCONbits.TMR0IF = 0;

        if (ledCount <= 3) tempLedState = (buf[0] & (1 << ledCount));
        else if (ledCount <= 7) tempLedState = (buf[1] & (1 << (ledCount - 4)));
        else if (ledCount <= 11) tempLedState = (buf[2] & (1 << (ledCount - 8)));
        else if (ledCount <= 15) tempLedState = (buf[3] & (1 << (ledCount - 12)));
        else if (ledCount <= 19) tempLedState = (buf[4] & (1 << (ledCount - 16)));

        PORTA &= PORTA_MASK;
        TRISA = TRISA_LED[ledCount];

        if (tempLedState != 0){
            PORTA |= PORTA_LED[ledCount];
        }

        ledCount++;
        if (ledCount == 20) ledCount = 0;
    }

    //timer1 10ms
    if (PIR1bits.TMR1IF == 1) {
        PIR1bits.TMR1IF = 0;
        TMR1H = (TMR1_RESET >> 8);
        TMR1L = (TMR1_RESET & 0xFF);
        if (delayCount != 0) delayCount--;
        if (bounceCount != 0)bounceCount--;
        if (autoMode==1){
            if(autoModedelayCount!=0){
                autoModedelayCount--;
            }else{
                modeChange = 1;
                mode++;
            }
        }
    }

    //RA3
    if (INTCONbits.IOCIF == 1) {
        INTCONbits.IOCIF = 0x00;
        IOCAF = 0x00;
        if (bounceCount == 0) {
            if (SWPIN == 0 && swState == 1) {
                mode++;
                if (mode == 7){
                    mode = 0;
                    autoMode = 1;
                }else{
                autoMode = 0;
                }
                bounceCount = BOUNCE_TIME;
                modeChange = 1;
            }
            swState = SWPIN;
        }
    }

    //serial RX
    if (PIR1bits.RCIF == 1) {
        char temp = RCREG;
        PIR1bits.RCIF = 0;
        if ((temp >= 'A' && temp <= 'Z') ||
                (temp >= '0' && temp <= '9') ||
                (temp == ' ')) {
            text[textNum] = temp;
            textNum++;
            putChar(temp);
        } else if (temp == '|') {
            text[textNum] = ' ';
            textNum++;
            for (int i = 0; i < textNum; i++) {
                putChar(text[i]);
            }
            writeEEPROM();
            putChar('d');
            INTCONbits.GIE = 0x00;
        } else {
            putChar('e');
        }

        if (textNum > 21) {
            putChar('o');
        }
    }
}
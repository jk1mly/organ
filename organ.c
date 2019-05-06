/*
*    PICマイコンテスト用サンプルプログラム
*    使用デバイス = PIC16F1503
*    動作仕様
*    クロック = 内部クロック(32MHz)
*    周辺接続 = RC2-RC3にLED(L=点灯)
*
*/

#define _XTAL_FREQ        16000000
#define  tone_C5          191
#define  tone_D5          170
#define  tone_E5          153
#define  tone_F5          144
#define  tone_G5          128
#define  tone_A5          113
#define  tone_B5          101
#define  tone_C6          95

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <pic.h>
#include <pic16f1503.h>
 
// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin) 
//#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
//#pragma config FOSC = ECH       // Oscillator Selection (ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin)
//#pragma config FOSC = HS        // Oscillator Selection (ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
//#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
//#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR) 
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
//#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
//#pragma config IOSCFS = 4MHZ    // Internal Oscillator Frequency Select (4 MHz)
#pragma config BOREN = OFF       // Brown-out Reset Selection bits (BOR enabled)
//#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
//#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
//#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
//#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2 
//#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
//#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
//#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
//#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
//#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled) 
#pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming enabled)

void us_loop(unsigned short int t)
{
   for(int lp=3; lp<t; lp++){
        asm( "NOP" );
        asm( "NOP" );
        asm( "NOP" );
        asm( "NOP" );
        asm( "NOP" );
        asm( "NOP" );
    }
}

// Speaker RC1,2
void tone(unsigned short int t)
{
   for(int lp=0; lp<(255 -64*(t>>7)); lp++){
        PORTC &= 0b11111001;
        PORTC |= 0b00000100;
        us_loop(t);
        PORTC &= 0b11111001;
        PORTC |= 0b00000010;
        us_loop(t);
    }
}

int main(int argc, char** argv) {

    OSCCON = 0b01111000;
    INTCON = 0b00000000;       // 全ての状態割り込みを禁止
    WDTCON = 0b00010110;       //初期状態 (2Sタイマ & 無効状態)
    ANSELA = 0b00000000;
    ANSELC = 0b00000000;
    TRISA  = 0b11111111;       //全て入力で初期化
    TRISC  = 0b11001000;       //RC0,1,2,4,5を出力に
    PORTC  = 0b00000000;
    WPUA   = 0b00111111;       //PullUp
    nWPUEN = 0;

    for(long lp=0; lp<3; lp++){
        tone(tone_C5); 
        us_loop(6000);
    }
    
    while (1) {
        unsigned char sw;
        unsigned char sc;

// RC0/RA
// Scan RC5,0
        sc = PORTC ;
        sc |= 0b00100001 ;
        sc &= 0b11111110 ;
        PORTC = sc ;
        asm( "NOP" );
        sw = PORTA ;
// Key RA0.1,4,5
        sw &= 0b00110011 ;
        sw |= 0b11001100 ;
        switch (sw) {
            case 0b11111110 : 
                tone(tone_C6 ); 
                break;
            case 0b11111101 : 
                tone(tone_B5 ); 
                break;
            case 0b11101111 : 
                tone(tone_D5 ); 
                break;
            case 0b11011111 : 
                tone(tone_E5 ); 
                break;
            default : 
                asm( "NOP" );
                break;
        }

// RC5/RA
// Scan RC5,0
        sc = PORTC ;
        sc |= 0b00100001 ;
        sc &= 0b11011111 ;
        PORTC = sc ;
        asm( "NOP" );
        sw = PORTA ;
// Key RA0.1,4,5
        sw &= 0b00110011 ;
        sw |= 0b11001100 ;
        switch (sw) {
            case 0b11111110 : 
                tone(tone_C5 ); 
                break;
            case 0b11111101 : 
                tone(tone_A5 ); 
                break;
            case 0b11101111 : 
                tone(tone_G5 ); 
                break;
            case 0b11011111 : 
                tone(tone_F5 ); 
                break;
            default : 
                asm( "NOP" );
                break;
        }
             
    }
    return 0;
}

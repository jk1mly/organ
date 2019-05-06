/*
*    PIC$B%^%$%3%s%F%9%HMQ%5%s%W%k%W%m%0%i%`(B
*    $B;HMQ%G%P%$%9(B = PIC16F1503
*    $BF0:n;EMM(B
*    $B%/%m%C%/(B = $BFbIt%/%m%C%/(B(32MHz)
*    $B<~JU@\B3(B = RC2-RC3$B$K(BLED(L=$BE@Et(B)
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
    INTCON = 0b00000000;       // $BA4$F$N>uBV3d$j9~$_$r6X;_(B
    WDTCON = 0b00010110;       //$B=i4|>uBV(B (2S$B%?%$%^(B & $BL58z>uBV(B)
    ANSELA = 0b00000000;
    ANSELC = 0b00000000;
    TRISA  = 0b11111111;       //$BA4$FF~NO$G=i4|2=(B
    TRISC  = 0b11001000;       //RC0,1,2,4,5$B$r=PNO$K(B
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

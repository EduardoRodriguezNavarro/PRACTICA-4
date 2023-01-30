#include "lib/include.h"
extern void Configurar_UART0(void)
{
    SYSCTL->RCGCUART  = (1<<4); //inicializar y habilitar el UART (RCGCUART) p. 902,344
    SYSCTL->RCGCGPIO |= (1<<2); //habilitar el reloj para el módulo GPIO (RCGCGPIO) p. 902,1351,340
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOC->AFSEL = (1<<5) | (1<<4); 
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOC->PCTL = (GPIOC->PCTL&0xFF00FFFF) | 0x00110000;    
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOC->DEN = (1<<5) | (1<<4);
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART4->CTL = (0<<9) | (0<<8) | (0<<0); //deshabilitar el UART -> UART control (UARTCTL) p. 918

    // UART Integer Baud-Rate Divisor (UARTIBRD) p. 914 ***
    /*
    BRD = 20,000,000 / (16*57600) = 21.7
    UARTFBRD[DIVFRAC] = integer(.7 * 64 + 0.5) = 45.3 se redondea hacia arriba 
    */
    UART4->IBRD = 21;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART4->FBRD = 46; 
    //  UART Line Control (UARTLCRH) pag.916
    UART4->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART4->CC =(0<<0);  
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART4->CTL = (1<<9) | (1<<8) | (1<<0); 
}


extern void printChar(char c) 
{
    while((UART4->FR & (1<<5)) != 0 ); 
    UART4->DR = c;
}

extern void printString(char *string) 
{
    while(*string)
    {
        printChar(*(string++));
    }
}





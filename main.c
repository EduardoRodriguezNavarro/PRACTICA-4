/*Usando el modulo 0 y 1 , configurar la tarjeta a la frecuencia asignada para adquirir 6 señales analógicas a una velocidad de 1 millón de muestras 
por segundo, por los canales asignados  y guardar los valores en un arreglo  para ser enviados con un botones  externos asociado al gpio D a través del protocolo de 
 comunicación asíncrona a una velocidad todo esto usando interrupciones.
 */
    
#include "lib/include.h"

extern void integer_to_char(uint16_t number)
{
    char number_in_string[5];
    int i = 3;
    int j = 0; 
    int n_digit;  
    while (i >= 0){
        n_digit = (int)number / (int)(pow(10, i));
        number_in_string[j] = n_digit + '0';
        number = number - (n_digit * pow(10, i)); 
        i = i - 1; 
        j++;
    }
    number_in_string[4] = '\n';
    printString(number_in_string);
}

int main(void)
{
    //________________________________________________________VARIABLES 
    uint16_t Result[6]; 
    uint16_t valor1; 
    uint16_t valor2;
    uint16_t valor3;
    uint16_t valor4;
    uint16_t valor5;
    uint16_t valor6;
    
    //FUNCION QUE CONGUGURA VELOCIDAD DEL CLK
    Configurar_PLL(_20MHZ);  
   //FUNCION QUE CONFIGURA ADC
    Configura_Reg_ADC(); 
    Configurar_UART0(); 

    
    while(1) 
    {
        //EN ESTA SECCIÓ LLAMAMOS Y HACEMS LA CONVERIÓN POR PROCESADOR 
        ADC0_InSeq1(Result); 
        ADC1_InSeq2(Result);
        ADC1_InSeq3(Result);
        
        valor1 = (uint16_t)(Result[0]);
        valor2 = (uint16_t)(Result[1]);
        valor3 = (uint16_t)(Result[2]);
        valor4 = (uint16_t)(Result[3]);
        valor5 = (uint16_t)(Result[4]);
        valor6 = (uint16_t)(Result[5]);

        //PARA PODER HACER ENVIO DEL VALOR, HAY QUE CONVERTIRLO A CHAR, PARA ELLO LAS FUNCIONES DE CONVERSION
        integer_to_char(valor1); 
        integer_to_char(valor2); 
        integer_to_char(valor3); 
        integer_to_char(valor4);
        integer_to_char(valor5);
        integer_to_char(valor6);

     
    }
}

#include "lib/include.h"

extern void Configura_Reg_ADC(void)
{
    SYSCTL->RCGCADC = (1<<0)|(1<<1); // HABILITAMOS EL MODULO 0, 1 DEL CLK DE ADC /  P.352 
    //                     F     E      D       C      B     A      
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(1<<3)|(0<<2)|(1<<1)|(1<<0); //HABILITAMOS PUERTOS DEL RELOJ, LOS INDICADOS ARRIBA / P.340, P.801
    
    //p. 663 (GPIODIR) habilita los pines como I/O / 0 ENTRADA, 1 SALIDA
    GPIOE->DIR &= ~(1<<2); //PE2
    GPIOE->DIR &= ~(1<<1); // PE1
    GPIOB->DIR &= ~(1<<4); //PB4
    GPIOD->DIR &= ~(1<<3); //PD3 
    GPIOD->DIR &= ~(1<<1); //PD1
    GPIOD->DIR &= ~(1<<0); //PD0
    
    //HABILITAMOS FUNCIONES ALTERNATIVAS / P.672
    GPIOE->AFSEL = (1<<2) | (1<<1);
     //             PE2       PE1
    GPIOD->AFSEL = (1<<3) | (1<<1) | (1<<0);
    //              PD3       PD1      PD0
    GPIOB->AFSEL = (1<<4); 
    //              PB4
    
    //APAGAMOS EL MODO DIGITAL EN LOS PINES SELECCIONADOS PARA RECIBIR LAS SEÑALES ANALOG. / P.683
    GPIOE->DEN = (0<<2) | (0<<1);  
    GPIOD->DEN = (0<<3) | (0<<1) | (0<<0); 
    GPIOB->DEN = (0<<4); 
    
    //registro combinado con el GPIOAFSEL y la tabla p. 1351, p. 688 (GPIOPCTL) 
    //CON AYUDA DE LOS 0'S Y SEGUN SU POSICIÓN ES QUE LE IDNCIAMOS EL PIN  
    GPIOE->PCTL = GPIOE->PCTL & (0xFFFFF00F); 
    //                              E2, E1   
    GPIOD->PCTL = GPIOD->PCTL & (0xFFFF0F00);
    //              PD3       PD1      PD0
    GPIOB->PCTL = GPIOB->PCTL & (0xFFF0FFFF);
     //              PB4
    
    // LE IDICAMOS QUE TRABAJAREMOS CON EL MODO ANALOGICO / p. 687 
    GPIOE->AMSEL = (1<<2) | (1<<1); 
    GPIOD->AMSEL = (1<<3) | (1<<1) | (1<<0); 
    GPIOB->AMSEL = (1<<4); 
    
    //LE INDICAMOS LA VELOCIDAD CON LA QUE CONVERITRÁ POR CADA SEGUNDO/ P.891
    ADC0->PC = (1<<2)|(1<<1)|(1<<0); // 1X10^6 MUESTRAS / S
    ADC1->PC = (1<<2)|(1<<1)|(1<<0);
    
    //este registro (ADCSSPRI) configura la prioridad de los secuenciadores p. 841
    ADC0->SSPRI = 0x3102; // EL SECUENCIADOR 1 TIENE LA MAYOR PRIORIDAD 
    ADC1->SSPRI = 0x1023; //EL SECUENCIADRO 2 TIENE LA MAYOR PRIORIDAD, LUEGO LE SIGUE EL 3 
    
    //(ADCACTSS) este registro controla la activación de los secuenciadores p. 821
    ADC0->ACTSS  = (0<<3) | (0<<2) | (0<<1) | (0<<0); //APAGAMOS PARA CONFIGURAR, RECORDAR LA LOGICA
    ADC1->ACTSS  = (0<<3) | (0<<2) | (0<<1) | (0<<0);
    
    //este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger) p. 834
    ADC0->EMUX = (0x0000);
    ADC1->EMUX = (0x0000);
    
    //este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado p. 867
    ADC0->SSMUX1 = 0x0246;
   //CHANEL       2  | 4 |  6 -> MODULO  0
    ADC1->SSMUX2 = 0x00A1; 
    //CHANEL       1 | 10 -> MOD. 1
    ADC1->SSMUX3 = 0x0007; 
    //CHANEL        7 ->  MOD. 1
    
    //este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción p. 868
    ADC0->SSCTL1 = 0x00000644; 
    ADC1->SSCTL2 = 0x00000064;
    ADC1->SSCTL3 = 0x00000006;
    
    //*enable ADC Interrupt = INTERRUMPE LA CONVERSIÓN  P. 825
    ADC0->IM |= (1<<1); //* Unmask ADC0 sequence 2 interrupt2 P.1082*
    ADC1->IM |= (1<<2);
    ADC1->IM |= (1<<3);
    
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores/ASÍ ACTIVAMOS SECUANCIADORES
    ADC0->ACTSS |= (0<<3) | (0<<2) | (1<<1) | (0<<0); 
    ADC1->ACTSS |= (1<<3) | (1<<2) | (0<<1) | (0<<0);
    
    ADC0->PSSI |= (0<<1);
    ADC1->PSSI |= (0<<2) | (0<<3);

    ADC0->ISC = 2; 
    ADC1->ISC = 12;
}

//FUNCION DONDE HACEMOS LA ADQUISICÓN -> ADC 0
extern void ADC0_InSeq1(uint16_t Result[6])
{
       ADC0->PSSI = 0x00000002;//HABILIATMOS MODO DE CONFIG 
       while((ADC0->RIS&0x02)==0){}; // espera al convertidor
       Result[0] = ADC0->SSFIFO1&0xFFF; // leer  el resultado almacenado en la pila
       Result[1] = ADC0->SSFIFO1&0xFFF;   //FIFO ALMACERNARA EL VALOR DE CONVERSIÓN
       Result[2] = ADC0->SSFIFO1&0xFFF;
       ADC0->ISC = 0x0002; // conversión finalizada
}
//FUNCION DONDE HACEMOS LA ADQUISICÓN 
extern void ADC1_InSeq2(uint16_t Result[6])
{
       ADC1->PSSI = 0x00000004;
       while((ADC1->RIS&0x04)==0){}; 
       Result[3] = ADC1->SSFIFO2&0xFFF; 
       Result[4] = ADC1->SSFIFO2&0xFFF;
       ADC1->ISC = 0x0004;  
}
//FUNCION DONDE HACEMOS LA ADQUISICÓN 
extern void ADC1_InSeq3(uint16_t Result[6])
{
       ADC1->PSSI = 0x00000008;  
       while((ADC1->RIS&0x08)==0){}; 
       Result[5] = ADC1->SSFIFO3&0xFFF; 
       ADC1->ISC = 0x0008;
}

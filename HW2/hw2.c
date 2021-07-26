/*
 * hw2.c
 *
 * Created: 6/13/2021 8:02:39 PM
 * Author: ramin rowshan
 */

#include <io.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>

int counter = 0;
int start = 0;
float countToSecond = 1000.0;
int time = 60;
int initIntrupt1 = 0;
int initIntrupt2 = 0;



void myPrint(unsigned char* word, int size);


void LCD_cmd(unsigned char cmd);
void init_LCD(void);
void LCD_write(unsigned char data);
void init_LCD(void)
{
    LCD_cmd(0x38);         // 8-bit mode                   
    delay_ms(1);
    LCD_cmd(0x01);         // clear the screen                     
    delay_ms(1);
    LCD_cmd(0x0E);         // turn on the cursor                
    delay_ms(1);
    LCD_cmd(0x80);         // move cursor to the first place of the first row   
    delay_ms(1);
    return;
}

void LCD_cmd(unsigned char cmd)
{
    PORTB=cmd;
    PORTC=0x04;              // Register Select = 0, Read/Write = 0, Enable = 1
    delay_ms(1);
    PORTC=0x00;              // Enable = 0
    delay_ms(10);
    return;
}

void LCD_write(unsigned char data)
{
    PORTB= data;
    PORTC=0x05;               // Register Select = 1, Read/Write = 0, Enable = 1
    delay_ms(1);
    PORTC=0x01;				 // Enable = 0
    delay_ms(10);
    return ;
}


void init()
{
    DDRC = 0xff ;
    DDRB = 0xff ; 
 
    DDRD = (0<< DDD0)  | (0<< DDD1 ) | (0<< DDD2 ) | (0<< DDD3 ) | (0<< DDD4 ) | (0<< DDD5 ) | (0<< DDD6 ) | (0<< DDD7 );
    
    DDRA =  (0<< DDA0 ) | (0<< DDA1 ) | (0<< DDA2 ) | (0<< DDA3 ) | (0<< DDA4 ) | (0<< DDA5 ) | (0<< DDA6 ) | (0<< DDA7 );

    GICR = (1<<INT0) | (1<<INT1);
    MCUCR = (1<<ISC01) | (0<<ISC00) | (1<<ISC11) | (0<<ISC10);
    
    TCCR0 = (0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
    TCNT0 = 0x83;
    OCR0 = 0x00;
    TIMSK|= (1<<TOIE0);
    
    #asm ("sei")
    
    init_LCD();

}


char* convertToStr(char* str,int numb)
{
    sprintf(str, "%02d", numb);
    return  str;
}

interrupt [EXT_INT0] void enable(void){

    if(!start && initIntrupt1)
    {  
    
        start = 1;
        time = 60;
        countToSecond = 1000.0;
    }
    
    initIntrupt1 = 1;

    
}


interrupt [EXT_INT1] void finish(void){
    unsigned char str[4] ;


    if(start && initIntrupt2)
    {  
    
       countToSecond /= 2;
       
       if(PINA == 0)
       {
          start = 0;
          
          myPrint("defuse",6);
          
       }
    }
    
    initIntrupt2 = 1;

    
}

interrupt [TIM0_OVF] void timer(void){
    
    TCNT0 = 0x83;
    
    if(start){
        counter++;
        if(counter>=(countToSecond)){
            unsigned char str[4] ;
            myPrint(convertToStr(str,time),2);
            counter = 0; 
            time--;
        }
        
        if(time == 0)
        {
         start = 0;
         myPrint("booom",5);
         start = 0;
         
        }
    }
    
}





void myPrint(unsigned char* word, int size) {

     unsigned int  i = 0;
    
    init_LCD();
    
    
    

    for (i = 0; i < size;i++)
    {
        LCD_write(word[i]);
        delay_us(5);
    }
   
}





void main(void)
{

    init();
    delay_ms(100);

    while (1)
        {
        delay_ms(1000);


        }
}

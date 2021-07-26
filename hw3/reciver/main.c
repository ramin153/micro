#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void init();
void init()
{
    SREG |= (1 << SREG_I);
    DDRD = 0X00;
    DDRA = 0xff;
    DDRC = 0xff;

    //reciver enable
    //reciver intrupt enable
    UCSRB = (1<<RXCIE) | (1<<RXEN) | (0<<UCSZ2);
    //asyn 2x
    UCSRA = (1<<U2X);


    //select right UCSRC
    //asyn
    //paritiy disable
    //1 bit for stop bit
    // 8 bit =>  UCSZ2=0   UCSZ1=1   UCSZ0=1
    UCSRC = (1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0);
    
    UBRRH = 0x00;
    UBRRL = 0x01;

    sei();
}

int main(void) {

    init();
    PORTA = 0;
    

    while (1)
    {
        
    }
    
}
ISR(USART_RXC_vect) {

    unsigned char data = UDR;

    if (data > 0x2B)
    {
        PORTA = 0x01;
    }else if (data < 0x11)
    {
       PORTA = 0x02;
    }else 
    {
        PORTA = 0x00;
    }
    
    
    
    
    
}

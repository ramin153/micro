#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void send();

void init();
void init()
{
    SREG |= (1 << SREG_I);
    DDRD = 0Xff;
    DDRA = 0x00;
    DDRC = 0xff;
    //select adc0
    //we need only 8 bit so we set ADLAR = 1 ==> 8 most valuable bit will be in ADCH
    // use avcc pin is ref
    ADMUX = (0<< REFS1 )| (1<< REFS0 )| (1<< ADLAR ) | ( 0<< MUX4 ) | (0<< MUX3 ) | (0<< MUX2 ) | (0<< MUX1 ) | (0<< MUX0 ) ;
    // enable adc
    // auto trigger
    //enable intrupt
    //prescale = 128 (become slow enough that usart work fine(always reciving data from adc is slower than sending it)  )
    ADCSRA = (1 << ADPS2)| (1<< ADPS1 ) | (1<< ADPS0 ) | (1 << ADIE)| (1<< ADATE ) | (1<< ADSC ) | (1<< ADEN ) ;
    // free running
    SFIOR = ( 0<< ADTS2 ) | (0<< ADTS1 ) | (0<< ADTS0 );

    //transmit enable
    UCSRB =  (1<< TXEN)|(0<<UCSZ2);
    //asyn 2x
    UCSRA = (1<<U2X);


    //select right UCSRC
    //asyn
    //paritiy disable
    //1 bit for stop bit
    // 8 bit =>  UCSZ2=0   UCSZ1=1   UCSZ0=1
    UCSRC = (1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0);

    UBRRH = 0x00;
    UBRRL = 0x1;
    
    

    sei();
}

int main(void) {


    init();
    
    

    while (1)
    {
        
    }
    
}

ISR(ADC_vect) {
    //send data read by acd
    char data = ADCH;
    if(((UCSRA >> UDRE) & 1) != 0){
    UDR = data;
    
    }
    
    
    
}





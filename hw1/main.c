#include <avr/io.h>
#include <util/delay.h>
#include <Lcd.h>
void myPrint(unsigned char* word, int size);
void readInput(unsigned int* digit) ;
int key_pressed(void) {
    PORTA = PORTA | (1<<PORTA1) | (1<<PORTA2) | (1<<PORTA3);
    PORTA &= ~(1<<PORTA0);
    if(!((PINA >> PINA4) & 1)) return 1;
    if(!((PINA >> PINA5) & 1)) return 2;
    if(!((PINA >> PINA6) & 1)) return 3;
    if(!((PINA >> PINA7) & 1)) return 10;

    PORTA = PORTA | (1<<PORTA0) | (1<<PORTA2) | (1<<PORTA3);
    PORTA &= ~(1<<PORTA1);
    if(!((PINA >> PINA4) & 1)) return 4;
    if(!((PINA >> PINA5) & 1)) return 5;
    if(!((PINA >> PINA6) & 1)) return 6;
    if(!((PINA >> PINA7) & 1)) return 11;

    PORTA = PORTA | (1<<PORTA0) | (1<<PORTA1) | (1<<PORTA3);
    PORTA &= ~(1<<PORTA2);
    if(!((PINA >> PINA4) & 1)) return 7;
    if(!((PINA >> PINA5) & 1)) return 8;
    if(!((PINA >> PINA6) & 1)) return 9;
    if(!((PINA >> PINA7) & 1)) return 12;

    PORTA = PORTA | (1<<PORTA0) | (1<<PORTA1) | (1<<PORTA2);
    PORTA &= ~(1<<PORTA3);
    if(!((PINA >> PINA4) & 1)) return 15;
    if(!((PINA >> PINA5) & 1)) return 0;
    if(!((PINA >> PINA6) & 1)) return 14;
    if(!((PINA >> PINA7) & 1)) return 13;
    
    return 16; // if no key is pressed it'll return 16
}


void show_pressed_key(int key) {
    if(key==0)  LCD_write('0');
    if(key==1)  LCD_write('1');
    if(key==2)  LCD_write('2');
    if(key==3)  LCD_write('3');
    if(key==4)  LCD_write('4');
    if(key==5)  LCD_write('5');
    if(key==6)  LCD_write('6');
    if(key==7)  LCD_write('7');
    if(key==8)  LCD_write('8');
    if(key==9)  LCD_write('9');
    if(key==10) LCD_write('/');
    if(key==11) LCD_write('*');
    if(key==12) LCD_write('+');
    if(key==13) LCD_write('-');
    if(key==14) LCD_write('=');
    if(key==15) LCD_cmd(0x01); // Clear the lcd
}


int main(void) {


    

    DDRB = (1<< DDB0 ) | (1<< DDB1 ) | (1<< DDB2 ) | (1<< DDB3 ) | (1<< DDB4 ) | (1<< DDB5 ) | (1<< DDB6 ) | (1<< DDB7 ) ;
    DDRD = (1<< DDD0 ) | (1<< DDD1 ) | (1<< DDD2 ) | (0<< DDD3 ) | (0<< DDD4 ) | (0<< DDD5 ) | (0<< DDD6 ) | (1<< DDD7 ) ;
    DDRA =  (1<< DDA0 ) | (1<< DDA1 ) | (1<< DDA2 ) | (1<< DDA3 ) | (0<< DDA4 ) | (0<< DDA5 ) | (0<< DDA6 ) | (0<< DDA7 );

    PORTD = (0<< PORTD0)  | (0<< PORTD1 ) | (0<< PORTD2 ) | (0<< PORTD3 ) | (0<< PORTD4 ) | (0<< PORTD5 ) | (0<< PORTD6 ) | (0<< PORTD7 );
    PORTA=0XF0;


    DDRC = (1<< DDC0 ) | (1<< DDC1 ) | (1<< DDC2 ) | (1<< DDC3 ) | (1<< DDC4 ) | (1<< DDC5 ) | (1<< DDC6 ) | (1<< DDC7 ) ;
    PORTC = (0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);




    unsigned char seg[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

    unsigned int myDigit[4],
                 myRepeatDigit[4];

    

    init_LCD();
    unsigned char i = 0,
        wrong[6] = "wrong",
        right[6] = "right",
        myInput[6] = "input",
        repeate[7] = "repeat",
        click[6] = "click";

    
    myPrint(myInput,5);
    _delay_ms(1000);

    readInput(myDigit);
    _delay_ms(1000);

    int isNotDone = 1;

    while (isNotDone)
    {
        myPrint(repeate,6);
        _delay_ms(1000);
        readInput(myRepeatDigit);
        _delay_ms(500);
        if (myDigit[0] == myRepeatDigit[0] && myDigit[1] == myRepeatDigit[1] &&
         myDigit[2] == myRepeatDigit[2] && myDigit[3] == myRepeatDigit[3])
        {
            myPrint(right,5);
            _delay_ms(1000);
            isNotDone = 0;
        }else
        {
             myPrint(wrong,5);
            _delay_ms(1000);
        }
        

    }
    



    _delay_ms(1000);
    myPrint(click,5);


    i = 0;
    int sevenSegmeng = 0;
    PORTC = seg[sevenSegmeng];
    sevenSegmeng = 1;
    PIND |= 1<<PIND3;
     _delay_ms(20);
    

     while (1) { 
        _delay_ms(5);

        if (((PIND >> PIND3) & 1) == 1 ) {
            PORTD &= ~(1<<PORTD7);
            i=0;
           
           
        }else  {
            PORTD |= (1<<PORTD7);
            if (i == 0)
            {
                i=1;


                PORTC = seg[sevenSegmeng];
                sevenSegmeng +=1;
                if (sevenSegmeng > 9)
                {
                   sevenSegmeng = 0; /* code */
                }
                
            }
            

        }
    }

    
}

void myPrint(unsigned char* word, int size) {

    LCD_cmd(0x01);         // clear the screen                     
    _delay_ms(1);



    LCD_cmd(0x14);
    LCD_cmd(0x14); 
    LCD_cmd(0x14); 
    int i = 0;
    while (i < size)
    {
        LCD_write(word[i]);
        _delay_us(10);
        i++;
    }
   
    

}

void readInput(unsigned int* digit) {

    int i = 0;
    int key;
    init_LCD();
    while (i <4)
    {
         do
            key = key_pressed();
        while(key==16); // if no key was pressed key_pressed will return 16
        digit[i] = key;
        show_pressed_key(key); 
        
        _delay_ms(200);
        i++;
    }
    

}


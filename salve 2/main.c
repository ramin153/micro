#include <avr/io.h>
#include <lcd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char light_voltage[8];
void init();
void setOCR(int num);
void setOCR(int num)
{
    OCR0 = 255-((255*num)/3);
}

void init()
{
    DDRB = 0b1000;
    TCCR0 |=(1<<WGM00)|(1<<WGM01)|(1<<COM00)|(1<<COM01)|(1<<CS00)|(0<<CS02)|(1<<CS01)|(0<<CS00);
    OCR0 = 255;
}
int main() {
    // SPI initialization
    SPCR |= (1<<SPE); // SPI Enable
    SPCR |= (0<<CPOL); // SPI Clock Polarity: Low
    SPCR |= (0<<CPHA); // SPI Clock Phase: Leading edge sample / Trailing Edge setup
    SPCR |= (1<<SPR1) | (1<<SPR0); // SPI Clock Rate: f/128 = 62.5 KHz
    SPSR |= (0<<SPI2X);
    SPCR |= (0<<DORD); // SPI Data Order: MSB First
    SPCR |= (0<<MSTR); // SPI Type: Slave

    // LCD initialization
    DDRC = 0XFF;
    LCD_Init();
    init();
    while(1) {
        LCD_Clear();
        SPDR = '0'; // Slave data doesn't matter for master
        while (((SPSR >> SPIF) & 1) == 0); // Wait till get data from master
        // sprintf(light_voltage,"T=%d", SPDR);
        setOCR(SPDR);
        itoa(SPDR, light_voltage, 10); // We need ascii code to show them on LCD
        
        for(int i = 0; i < strlen(light_voltage); i++) {
            LCD_Char(light_voltage[i]);
        }
        _delay_ms(1000); // Some wait too see data, before LCD_Clear()
    }
}
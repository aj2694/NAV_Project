#define F_CPU 12000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<stdlib.h>
#include<avr/interrupt.h>

//USART_BAUDRATE 9600

#define LCD_DATA PORTA
#define ctrl PORTB
#define en PB2
#define rw PB1
#define rs PB0

void LCD_cmd(unsigned char cmd);
void init_LCD(void);
void LCD_write(unsigned char data);
void LCD_write_string(unsigned char* str);
void gps_test(void);

#define UDRE 5
#define RXC 7
#define DATA_REGISTER_EMPTY (1<<UDRE)

int p=0,q=0,i,j,buffercount,gps,k;
static char data,rec;
unsigned char Rcd_Data, start;

static char set_buffer[5],DisplayBuffer[15],set_buffer1[5],ReceiveBuffer[110],data,lat_buffer[12],lon_buffer[12],temp[10];

ISR(USART_RXC_vect)
{
    data=UDR;
    rec=1;
}

void init_LCD(void)
{
    LCD_cmd(0x38);
    _delay_ms(1);
    LCD_cmd(0x01);
    _delay_ms(1);
    LCD_cmd(0x0E);
    _delay_ms(1);
    LCD_cmd(0x80);
    _delay_ms(1);
    return;
}
void LCD_cmd(unsigned char cmd)
{
    LCD_DATA=cmd;
    ctrl=(0<<rs)|(0<<rw)|(1<<en);
    _delay_ms(40);
    ctrl=(0<<rs)|(0<<rw)|(0<<en);
    return;
}
void LCD_write(unsigned char data)
{
    LCD_DATA=data;
    ctrl=(1<<rs)|(0<<rw)|(1<<en);
    _delay_ms(40);
    ctrl=(1<<rs)|(0<<rw)|(0<<en);
    return;
}
void LCD_write_string(unsigned char *str)
{

    int i=0;
    while(str[i]!='\0')
    {
        LCD_write(str[i]);
        i++;
    }
    return;
}

void gps_test(void)
{

    unsigned int i=0;
    buffercount=0;
    start=0,rec=0,gps=0;
    while(data!='C');
    buffercount=0;
    rec=0;
    for(i=0;i<39;i++)
    {

        while(rec!=1);
        {
            rec=0;
            ReceiveBuffer[buffercount]=data;
            bufferdata++;
            start=1;

        }
    }
}
void TransByte(char Data)
{

    while((UCSRA&DATA_REGISTER_EMPTY)==0)
        UDR=Data;
}
int main(void)
{
    DDRA=0xFF;
    DDRB=0x07;
    init_LCD();
    _delay_ms(50);
    LCD_write_string(" ");
    DDRD=0x72;
    PORTD=0x00;
    SFIOR=0x00;
    UCSRA=0x02;
    UCSRB=0x98;
    UCSRC=0x86;
    UBRRH=0x00;
    UBRRL=0x0C;
    DDRD=1<<3;
    sei();
    p=0;
    buffercount=0;
    while(1)
    {

        gps_test();
        if(start==0)
        {

            start=0;
            LCD_write_string("NO GPS");

        }
        if(start==1)
        {
            if(ReceieveBuffer[12]=='A')
            {

                LCD_cmd(0x01);
                p=0;
                if(i=13;i<=22;i++)
                {

                    lat_buffer[p]=ReceiveBuffer[i];
                    p++;
                }
                LCD_cmd(0x80);
                LCD_write(' ');
                LCD_write('L');
                LCD_write('A');
                LCD_write('T');
                LCD_write(':');
                for(i=1;i<10;i++)
                    {
                        LCD_write(lat_buffer[i]);
                    }
                p=0;
                if(i=26;i<=36;i++)
                {

                    lon_buffer[p]=ReceiveBuffer[i];
                    p++;
                }
                    LCD_cmd(0xC0);
                    LCD_write(' ');
                    LCD_write('L');
                    LCD_write('O');
                    LCD_write('N');
                    LCD_write(':');
                 for(i=1;i<10;i++)
                    {
                        LCD_write(lon_buffer[i]);
                    }
            }
            if(ReceiveBuffer[12]=='V')
            {
                LCD_cmd(0x01);
                LCD_cmd(0x80);
                LCD_write_string("GPS under network");
                LCD_cmd(0xc0);
                LCD_write_string("Search");
            }
        }
    }

}

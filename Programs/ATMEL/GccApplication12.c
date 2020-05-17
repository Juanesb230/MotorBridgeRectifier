#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include "lcd.h"
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

char mensaje[32];
uint8_t contador=0,menu=0,opcion=0,prender=0;
int angulo=138,contador1=0,i=66,an,apagar=0,cont=0,prendido=0;

void conf_micro(void);

int main(void)
{
	TCNT0=0;
	OCR0A=i;
	conf_micro();
	lcd_clrscr();
	lcd_gotoxy(0,0);
	sprintf(mensaje,"  ELECTRONICA  \n  DE POTENCIA  ");
	lcd_puts(mensaje);
	_delay_ms(3000);
	
	while(1)
	{
		if (apagar==1)
		{
			do
			{
				i++;
				an=2.727*i;
				lcd_clrscr();
				lcd_gotoxy(0,0);
				sprintf(mensaje,"Apagando \nangulo=%d",an);
				lcd_puts(mensaje);
				OCR0A=i;
				_delay_ms(500);
			} while (i<=65);
			i=66;
			apagar=0;
		}
		if(contador==0)
		{
			lcd_clrscr();
			lcd_gotoxy(0,0);
			sprintf(mensaje,"   Conf. del  \n angulo  disparo ");
			lcd_puts(mensaje);
			_delay_ms(200);
		}
		else if(contador==1)
		{
			lcd_clrscr();
			lcd_gotoxy(0,0);
			sprintf(mensaje,"   Conf. por   \n   velocidad");
			lcd_puts(mensaje);
			_delay_ms(200);
		}
		
		while(opcion==1)
		{
			lcd_clrscr();
			lcd_gotoxy(0,0);
			sprintf(mensaje,"Elija el angulo \nangulo=%d",angulo);
			lcd_puts(mensaje);
			_delay_ms(200);			
			if (prender==1)
			{
				int angulo1=angulo;
				if (i>=angulo1*0.367)
				{
					do 
					{
						an=i*2.727;
						lcd_clrscr();
						lcd_gotoxy(0,0);
						sprintf(mensaje,"Elija el angulo \nangulo=%d",an);
						lcd_puts(mensaje);
						_delay_ms(500);
						OCR0A=i;
						i--;
					} while (i>angulo1*0.367);
				}else 
				{
					do
					{
						an=i*2.727;
						lcd_clrscr();
						lcd_gotoxy(0,0);
						sprintf(mensaje,"Elija el angulo \nangulo=%d",an);
						lcd_puts(mensaje);
						_delay_ms(500);
						OCR0A=i;
						i++;
					} while (i<=angulo1*0.367);
				}
			}
			prender=0;
			prendido=1;
		}
		while(opcion==2)
		{
			if (contador1==0)
			{
				lcd_clrscr();
				lcd_gotoxy(0,0);
				sprintf(mensaje,"Elegir velocidad \nBaja");
				lcd_puts(mensaje);
				_delay_ms(200);
			}else if (contador1==1)
			{
				lcd_clrscr();
				lcd_gotoxy(0,0);
				sprintf(mensaje,"Elegir velocidad \nMedia");
				lcd_puts(mensaje);
				_delay_ms(200);
			}else if (contador1==2)
			{
				lcd_clrscr();
				lcd_gotoxy(0,0);
				sprintf(mensaje,"Elegir velocidad \nAlta");
				lcd_puts(mensaje);
				_delay_ms(200);
			}
		}
		while (opcion==3)
		{
			an=2.727*i;
			lcd_clrscr();
			lcd_gotoxy(0,0);
			sprintf(mensaje,"Velocidad Baja \nangulo=%d",an);
			lcd_puts(mensaje);
			_delay_ms(200);
			if (prender==1)
			{
				do
				{
					an=2.727*i;
					lcd_gotoxy(0,1);
					sprintf(mensaje,"angulo=%d \n",an);
					lcd_puts(mensaje);
					OCR0A=i;
					i--;
					_delay_ms(500);
				} while (i>=58);
			}
			prender=0;
			prendido=1;
		}
		while (opcion==4)
		{
			an=2.727*i;
			lcd_clrscr();
			lcd_gotoxy(0,0);
			sprintf(mensaje,"Velocidad Media \nangulo=%d",an);
			lcd_puts(mensaje);
			_delay_ms(200);
			if (prender==1)
			{
				do
				{
					an=2.727*i;
					lcd_gotoxy(0,1);
					sprintf(mensaje,"angulo=%d \n",an);
					lcd_puts(mensaje);
					_delay_ms(500);
					OCR0A=i;
					i--;
				} while (i>54);
			}
			prender=0;
			prendido=1;
			
		}
		while (opcion==5)
		{
			an=2.727*i;
			lcd_clrscr();
			lcd_gotoxy(0,0);
			sprintf(mensaje,"Velocidad Alta \nangulo=%d",an);
			lcd_puts(mensaje);
			_delay_ms(200);
			if (prender==1)
			{
				do
				{
					an=2.727*i;
					lcd_gotoxy(0,1);
					sprintf(mensaje,"angulo=%d \n",an);
					lcd_puts(mensaje);
					_delay_ms(500);
					OCR0A=i;
					i--;
				} while (i>50);
			}
			prender=0;
			prendido=1;
		}
	}
}

void conf_micro(void)
{
	DDRB=0;
	PORTB=255;
	DDRD=0b00010000;
	PORTD=0b11101111;
	EICRA=0b00001010;
	EIMSK=0b00000011;
	TCCR0A=0b00000010;
	TCCR0B=0b00000101;
	TIMSK0=0b00000010;
	lcd_init(LCD_DISP_ON);
	sei();
}

ISR(INT0_vect)
{
		TCNT0=0;
		PORTD=0;
		OCR0A=i;
		//if (prender==1)
		//{
			//cont++;
		//}else cont=0;
}

ISR(TIMER0_COMPA_vect)
{
	
	if (apagar==1 || prendido==1 || prender==1)
	{
		PORTD=0b00010000;
	}
	else PORTD = 0;
}

ISR(INT1_vect)
{
	menu=PINB & 0b00000111;
	if(menu==4 && opcion==1)
	{
		OCR0A=i;
		prender=1;
	}
	else if(menu==4 && contador==0 && opcion==0)opcion=1;
	else if(menu==4 && contador==1 && opcion==0)opcion=2;
	else if(menu==4 && contador1==0 && opcion==2)
	{
		OCR0A=i;
		prender=1;
		opcion=3;
	}
	else if(menu==4 && contador1==1 && opcion==2)
	{
		OCR0A=i;
		opcion=4;
		prender=1;
	}
	else if(menu==4 && contador1==2 && opcion==2)
	{
		OCR0A=i;
		opcion=5;
		prender=1;
	}
	else if(menu==3)
	{
		apagar=1;
		opcion=0;
		prender=0;
		angulo=138;
		contador1=0;
		cont=0;
		prendido=0;
	}
	else if(menu==6 && opcion==1)
	{
		angulo--;
		if (angulo<138)angulo=138;

	}
	else if(menu==5 && opcion==1)
	{
		angulo++;
		if (angulo>180)angulo=180;
	}
	else if(menu==5 && opcion==2)
	{
		contador1++;
		if(contador1>2)contador1=0;
	}
	else if(menu==6 && opcion==2)
	{
		contador1--;
		if(contador1<0)contador1=2;
	}
	else if(menu==6 || menu==5)
	{
		contador++;
		if (contador>1)contador=0;
	}
}
#include<reg52.h>

sbit PWM_pin=P2^0;
void init_port(void);
void init_PWM(void);

void main(){
		init_port();
		init_PWM();
		while(1);
}
void init_port(){
	P2=0x00;
}
void init_PWM(){
	TMOD&=0x00;
	TMOD|=0x01;
	TH0=0x00;
	TL0=0x00;
	EA=1;
	ET0=1;
	TR0=1;
}

void timer0_ISR(void) interrupt 1
{
		if(PWM_pin)
			PWM_pin=0;
		else
			PWM_pin=1;
	}		
		
		


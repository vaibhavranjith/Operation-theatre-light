#include <reg52.h>
#include <math.h>
#define PWM_Freq_Num   1

sbit rs=P1^3;
sbit rw=P1^4;															//variables to control the  LCD screen
sbit e= P1^5;															//enable of LCD screen
sbit trig1=P1^0;
sbit trig2=P1^1;
sbit PWM_pin1	=	P0^0;
sbit PWM_pin2	= P2^0;//P0^4;
sbit btn_1=P3^5;
sbit btn_2=P3^6;
sbit btn_3=P3^7;

int PWM = 0; 
unsigned char hi=0;
unsigned char lo=0;
int dist[]={0,0};
int repeat=1;															//will be reset in ISR
unsigned char ch[]="DISTANCE";						//string to print on LCD screen
unsigned char ch1;												//character to print the distance
unsigned int i,j,k;												//loop control
int timer=0;															
unsigned char d0;
unsigned char d1;
unsigned char d2;
unsigned int temp = 0;
int button=0;
int br_ar[]= {0,26,51,77,102,128,153,178.5,204,229};
int mod=1;
int br_mod=3;
int sensor_no=1;

void delay(unsigned int);									//delay used for giving the edge for enable on LCD
void cmd(unsigned char);									//function used to pass command to LCD screen
void dat(unsigned char);									//to send dat to print on LCD
void printmeasure(int);										//function for printing the diastance 
void portinitialize(void); 
void measure1();
void measure2();
void InitTimer0(void);										//TIMER0 used for measuring distance
void InitTimer1(void);										//TIMER1 used for trigering the HC-SR04
																					//delay for ultrasonic sensor triger
void write_PWM(int);


void main(){
	portinitialize();
	IT0 = 1;   // Configure interrupt 0 for falling edge on /INT0 (P3.2)
	EX0 = 1;   // Enable EX0 Interrupt
	EA = 1;    // Enable Global Interrupt Flag
	EX1 =1;
	IT1=1;

	
	while(1){
		measure1();
		measure2();
		if(dist[0]<=20|| dist[1]<=20){
			if(mod!=9)
				write_PWM(br_ar[br_mod]+50);
			else if(mod==9)
				write_PWM(255);
		}
		else
				write_PWM(br_ar[br_mod]);
		}
	
}

void write_PWM(int a){
	 
		//PWM = a;              		// Make 50% duty cycle of PWM
		for(i=0;i<120;i++)
		{

				switch(mod){
					case 0:	PWM_pin2=0;
									for(j=0;j<(255-a);j++)
										PWM_pin1=0;
									 for(j=0;j<a;j++)
										PWM_pin1=1;
									break;
					case 1:	PWM_pin1=0;
									for(j=0;j<(255-a);j++)
										PWM_pin2=0;														//software PWM production
									for(j=0;j<a;j++)
										PWM_pin2=1;
									break;
					case 2:for(j=0;j<(255-a);j++){
									PWM_pin1=0;
								 PWM_pin2=0;
									}	
								  for(j=0;j<a;j++){
										PWM_pin1=1;
										PWM_pin2=1;
									}
								 break;
				}
			}
				

}


 void portinitialize(){
	 P3=0xFF;																				//P3:input P1:output P2:output P0:output
	 P1=0x00;
	 P2=0x00;
	 P0=0x00;
 }
 


void measure1(){
			sensor_no=1;
			repeat=1;
			InitTimer0();
		  trig1=1;
			for(i=0;i<1200;i++);
			trig1=0;
			TR0=1;
			IT0 = 1;   // Configure interrupt 0 for falling edge on /INT0 (P3.2)
			EX0 = 1;   // Enable EX0 Interrupt
			EA = 1;
			while(repeat){
			}
				
} 

void measure2(){
			sensor_no=2;
			repeat=1;
			InitTimer0();
		  trig2=1;
			for(i=0;i<1200;i++);
			trig2=0;
			TR0=1;
			IT0 = 1;   // Configure  0 for falling edge on /INT0 (P3.2)
			EX0 = 1;   // Enable EX0 Interrupt
			EA = 1;
			while(repeat){
			}
}




void InitTimer0(void){
	TMOD &= 0xF0;    // Clear 4bit field for timer0
	TMOD |= 0x01;    // Set timer0 in mode 1 = 16bit mode
	
	TH0 = 0x00;      // First time value
	TL0 = 0x00;      // Set arbitrarily zero	
}


void dist_timer(void) interrupt 0
{
	TR0=0;
	TF0=0;
	timer&=0x00;
	hi=TH0;
	lo=TL0;
	timer=(hi<<7)|lo	;		  				
	repeat=0;
	switch(sensor_no){
		case 1:
						dist[0]=(int)(((timer/1000)*0.342)/2);
						break;
		case 2:
						dist[1]=(int)(((timer/1000)*0.342)/2);
						break;
	}
}	

void button_press(void) interrupt 2															//interrupt function to POLL the port 
{
		if(btn_1==0 && btn_2==1 && btn_3==1)
		button=1;
		if(btn_1==1 && btn_2==0 && btn_3==1)
		button=2;
		if(btn_1==1 && btn_2==1 && btn_3==0)
		button=3;
		switch(button){
			case 3:mod++;
						 if(mod==3)
							 mod=0;
						 break;
			case 2:br_mod--;
							if(br_mod<=0)
								br_mod==0;
							break;
			case 1:br_mod++;
							 if(br_mod>=9)
								 br_mod=9;
							 break;
				}				 
}

void printmeasure(int dist)														// function to print characters on an LCD 16x4 screen 
{
	cmd(0x38);
	cmd(0x01);
	cmd(0x0c);
	cmd(0x83);
	cmd(0x06);
	
	
	for(i=0;ch[i]!='\0';i++)
	    dat(ch[i]);

		cmd(0xc3);
	 temp =dist;
	 d0=((temp%10)+48);
		temp=temp/10;
	 d1=((temp%10)+48);
		temp=temp/10;
	 d2=((temp%10)+48);

		dat(d2);
		dat(d1);
		dat(d0);
	
}
void delay(unsigned int t)
{
	unsigned int i,j;
	e=1;
	for(i=0;i<t;i++)
	for(j=0;j<1275;j++);// 
	e=0;
}

void cmd(unsigned char ch)
{
    
	rs=0;
	rw=0;
	P2=ch;
	delay(1);
}
void dat(unsigned char ch)
{
	rs=1;
	rw=0;
	P2=ch;
	delay(1);
}

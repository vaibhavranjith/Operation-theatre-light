#include<reg52.h>
sbit a= P3^0;
sbit b= P3^1;
sbit c= P3^2;
sbit d= P3^3;
void main()
{
unsignedchar n[10]={0×40,0xF9,0×24,0×30,0×19,0×12,0×02,0xF8,0xE00,0×10};
unsigned int i,j;
a=b=c=d=1;
while(1)
{
for(i=0;i<10;i++)
{
P2=n[i];
for(j=0;j<60000;j++);
}
}
}